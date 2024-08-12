#pragma once

#include <mutex>

#include "IXModem.h"

class XModem : IXModem {
public:
    // Constructor
    XModem() = default;

    bool SendByte(const unsigned char byte) override ;
    unsigned char ReceiveByte() override ;
    bool SendPackage(const size_t packageNumber) override ;
    unsigned char ReceivePackage() override ;
    unsigned char CalCRC(unsigned char *ptr, int count) override;

    XModemByteArray m_data;
    std::mutex m_dataMtx;
};
