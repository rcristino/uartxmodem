#include <gtest/gtest.h>
#include <future>

#include "TransmissionUart.h"

#define RECEIVER_PORT "/dev/ttyV0" // UART device file
#define SENDER_PORT "/dev/ttyV1" // UART device file

/**
 * Test Case: SendBytes and ReceiveBytes
 * 
 * Send and receive bytes via UART
 */
TEST(UartXModemTest, SendAndReiveBytes) {
    
    TransmissionUartReceiver receiver;
    TransmissionUartSender sender;

    receiver.StartReceiver(RECEIVER_PORT);
    sender.StartSender(SENDER_PORT);

    std::future<unsigned char> resultReceiver = std::async(std::launch::async, [&receiver]() {
        unsigned char result = receiver.ReceiveByte();
        receiver.SendByte('b');
        return result;
    });

    std::future<unsigned char> resultSender = std::async(std::launch::async, [&sender]() {
        // give time for the receiver to listen the port
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        sender.SendByte('a');
        unsigned char value = sender.ReceiveByte();
        return value;
    });
    
    std::future_status statusReceiver = resultReceiver.wait_for(std::chrono::seconds(3));
    std::future_status statusSender = resultSender.wait_for(std::chrono::seconds(3));

    if (statusReceiver == std::future_status::ready) {
        unsigned char value = resultReceiver.get();
        EXPECT_EQ('a', value);
    }

     if (statusSender == std::future_status::ready) {
        unsigned char value = resultSender.get();
        EXPECT_EQ('b', value);
    }
}

// Main function for running all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}