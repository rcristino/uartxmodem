#include <iostream>
#include <string>

#include "XModem.h"

constexpr int MaxNumPackage(255);

bool XModem::SendPackage(const size_t packageNumber) {
    XModemByteArray package(XMODEM_PKG_SIZE, 0);

    std::lock_guard<std::mutex> lock(m_dataMtx);
    // Copy data into the package (up to PACKAGE_SIZE bytes)
    size_t start = (packageNumber - 1) * XMODEM_PKG_SIZE;
    size_t end = std::min(start + XMODEM_PKG_SIZE, m_data.size());
    for (size_t i = start; i < end; ++i) {
        package[i - start] = m_data[i];
    }

    // Send Start of Header
    SendByte(XMODEM_SOH);
    SendByte(static_cast<unsigned char>(packageNumber));
    SendByte(static_cast<unsigned char>(MaxNumPackage - packageNumber));

    // Send the package data
    for (const auto& byte : package) {
        SendByte(byte);
    }

    // Send checksum
    SendByte(static_cast<unsigned char>(CalCRC(package.data(), package.size())));

    return true;
}

unsigned char XModem::ReceivePackage() {
    unsigned char receivedFirstByte = ReceiveByte();

    switch (receivedFirstByte) {
        case XMODEM_SOH: {
            unsigned char receivedPkgNumOne = ReceiveByte();
            unsigned char receivedPkgNumTwo = ReceiveByte();
            // package number check
            if ((receivedPkgNumOne + receivedPkgNumTwo) == MaxNumPackage) {
                XModemByteArray package(XMODEM_PKG_SIZE, 0);
                for (size_t i = 0; i < XMODEM_PKG_SIZE; ++i) {
                    package[i] = ReceiveByte();
                }
                int receivedCRC =  static_cast<int>(ReceiveByte());
                int resultCRC = CalCRC(package.data(), package.size());

                if (receivedCRC == resultCRC) {
                    std::lock_guard<std::mutex> lock(m_dataMtx);
                    m_data.insert(m_data.end(), package.begin(), package.end());
                    return receivedFirstByte;
                }
            }
            break;
        }
        case XMODEM_C:
        case XMODEM_EOT:
        case XMODEM_ETB: {
            return receivedFirstByte;
            break;
        }
        default: {
            return '\0';
        }
    }
    return '\0';
}

// TODO change return two bytes instead
unsigned char XModem::CalCRC(unsigned char *ptr, int count) {
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

bool XModem::SendByte(const unsigned char byte) {
    // For actual communication implementation
    throw std::runtime_error("SendByte is not implemented");
    return false;
}

unsigned char XModem::ReceiveByte() {
    // For actual communication implementation
    throw std::runtime_error("ReceiveByte is not implemented");
    return XMODEM_NACK;
}
