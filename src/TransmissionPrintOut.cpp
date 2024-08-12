#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "TransmissionPrintOut.h"

bool TransmissionPrintOut::SendByte(const unsigned char byte) {
    std::lock_guard<std::mutex> lock(m_queueMtx);
    m_queue.push(byte);
    return true;
}

unsigned char TransmissionPrintOut::ReceiveByte() {
    unsigned char receivedByte = '\0';

    while(receivedByte == '\0') {    
        { // mutex guard
            std::lock_guard<std::mutex> lock(m_queueMtx);
            if(!m_queue.empty()) {
                receivedByte = m_queue.front();
                m_queue.pop(); // consumed value
                return receivedByte;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return receivedByte;
}

void TransmissionPrintOut::Print() {
    std::cout << "Received<int>: ";

    std::lock_guard<std::mutex> lock(m_queueMtx);

    while (!m_queue.empty()) {
        std::cout << static_cast<int>(m_queue.front()) << ":";
    }

    std::cout << std::endl;
}
