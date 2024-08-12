#pragma once

class ITransmission {
public:
    // Send byte over the chosen transportation
    virtual bool SendByte(const unsigned char byte) = 0;

    // Receive byte over the chosen transportation
    virtual unsigned char ReceiveByte() = 0;
};
