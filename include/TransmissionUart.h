#pragma once

#include <mutex>
#include <queue>

#include "ITransmission.h"

class TransmissionUartReceiver : public ITransmission {
public:
    TransmissionUartReceiver() = default;

    bool SendByte(const unsigned char byte) override ;
    unsigned char ReceiveByte() override ;

    bool StartReceiver(const char* portRecv);
    void StopReceiver();

private:
    int fdReceiver{0};
};

class TransmissionUartSender : public ITransmission {
public:
    TransmissionUartSender() = default;

    bool SendByte(const unsigned char byte) override ;
    unsigned char ReceiveByte() override ;

    bool StartSender(const char* portSender);
    void StopSender();

private:
    int fdSender{0};
};
