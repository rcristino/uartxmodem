#pragma once

#include <mutex>
#include <queue>

#include "ITransmission.h"

class TransmissionPrintOut : public ITransmission {
public:
    TransmissionPrintOut() = default;

    bool SendByte(const unsigned char byte) override ;
    unsigned char ReceiveByte() override ;
    void Print();

private:
    std::queue<unsigned char> m_queue;
    std::mutex m_queueMtx; 
};
