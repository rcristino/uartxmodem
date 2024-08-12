#pragma once

#include <vector>

// types
using XModemByteArray = std::vector<unsigned char>;
enum class XModemState { IDLE, TRANSMISSION, FINISHED, ERROR };

// Constants
constexpr unsigned char XMODEM_SOH = 0x01;    // Start of Header
constexpr unsigned char XMODEM_EOT = 0x04;    // End of Transmission
constexpr unsigned char XMODEM_ACK = 0x06;    // Acknowledge
constexpr unsigned char XMODEM_NACK = 0x15;    // Negative Acknowledge
constexpr unsigned char XMODEM_ETB = 0x17;    // Transmission Finished
constexpr unsigned char XMODEM_CAN = 0x18;    // Cancel
constexpr unsigned char XMODEM_C = 0x43;      // ASCII 'C'
constexpr size_t XMODEM_PKG_SIZE = 128; // Package max size to be sent (block)
