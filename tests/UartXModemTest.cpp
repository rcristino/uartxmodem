#include <gtest/gtest.h>
#include <memory.h>

#include "XModemReceiver.h"
#include "XModemSender.h"
#include "MockTransmission.h"


unsigned char CalCRC(unsigned char *ptr, int count) {
    int  crc;
    unsigned char i;
    crc = 0;
    while (--count >= 0)
    {
        crc = crc ^ (int) *ptr++ << 8;
        i = 8;
        do
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        } while(--i);
    }
    return (crc);
}

/**
 * Test Case: SendData
 * 
 * Check the the data that is sent over the communication works as expected 
 */
TEST(UartXModemTest, SendData) {
    std::shared_ptr<MockTransmission> mockTransmission = std::make_shared<MockTransmission>();
    XModemByteArray data(XMODEM_PKG_SIZE, 0);
    data[0] = 'a';
    data[1] = 'b';
    data[2] = 'c';

    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .WillOnce(testing::Return(XMODEM_C))
        .WillRepeatedly(testing::Return(XMODEM_ACK));
    
    EXPECT_CALL(*mockTransmission, SendByte(XMODEM_SOH))
        .Times(2) // for XMODEM_SOH and packageNumber which is =1
        .WillRepeatedly(testing::Return(true));

    EXPECT_CALL(*mockTransmission, SendByte(254))
        .WillOnce(testing::Return(true));

    EXPECT_CALL(*mockTransmission, SendByte('a'))
        .WillOnce(testing::Return(true));

    EXPECT_CALL(*mockTransmission, SendByte('b'))
        .WillOnce(testing::Return(true));

    EXPECT_CALL(*mockTransmission, SendByte('c'))
        .WillOnce(testing::Return(true));

    EXPECT_CALL(*mockTransmission, SendByte(static_cast<unsigned char>(0)))
        .WillRepeatedly(testing::Return(true));

    EXPECT_CALL(*mockTransmission, SendByte(CalCRC(data.data(), data.size())))
        .WillRepeatedly(testing::Return(true));

    EXPECT_CALL(*mockTransmission, SendByte(XMODEM_EOT))
        .WillOnce(testing::Return(true));

    EXPECT_CALL(*mockTransmission, SendByte(XMODEM_ETB))
        .WillOnce(testing::Return(true));

    XModemSender sender(mockTransmission);
    bool result = sender.SendBuffer(data);

    EXPECT_EQ(true, result);
}

/**
 * Test Case: ReceiveData
 * 
 * Check the the data which has been sent is fetched correctly
 */
TEST(UartXModemTest, ReceiveData) {
    std::shared_ptr<MockTransmission> mockTransmission = std::make_shared<MockTransmission>();
    XModemByteArray data(XMODEM_PKG_SIZE, 0);
    data[0] = 'a';
    data[1] = 'b';
    data[2] = 'c';

    testing::Sequence seq;

    EXPECT_CALL(*mockTransmission, SendByte(XMODEM_C))
        .WillOnce(testing::Return(true));
    
    EXPECT_CALL(*mockTransmission, SendByte(XMODEM_ACK))
        .WillRepeatedly(testing::Return(true));

    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .InSequence(seq)
        .WillOnce(testing::Return(XMODEM_SOH));
    
    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .InSequence(seq)
        .WillOnce(testing::Return(static_cast<int>(1)));

    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .InSequence(seq)
        .WillOnce(testing::Return(static_cast<int>(254)));

    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .InSequence(seq)    
        .WillOnce(testing::Return(data[0]));

    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .InSequence(seq)
        .WillOnce(testing::Return(data[1]));

    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .InSequence(seq)
        .WillOnce(testing::Return(data[2]));

     EXPECT_CALL(*mockTransmission, ReceiveByte())
        .Times(125)
        .InSequence(seq)   
        .WillRepeatedly(testing::Return(static_cast<unsigned char>(0)));

    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .InSequence(seq)
        .WillOnce(testing::Return(CalCRC(data.data(), data.size())));

    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .InSequence(seq)
        .WillOnce(testing::Return(XMODEM_EOT));

    EXPECT_CALL(*mockTransmission, ReceiveByte())
        .InSequence(seq)
        .WillOnce(testing::Return(XMODEM_ETB));
    
    XModemReceiver receiver(mockTransmission);
    auto result = receiver.ReceiveBuffer();

    EXPECT_EQ(data, result);
}

// Main function for running all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}