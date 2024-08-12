#pragma once

#include "XModemDefinitions.h"

class IXModem {
public:
    // Send byte (communication)
    virtual bool SendByte(const unsigned char byte) = 0;

    // Receive byte (communication)
    virtual unsigned char ReceiveByte() = 0;

    // Send package (bytes block)
    virtual bool SendPackage(const size_t packageNumber) = 0;

    // Receive package (bytes block). Returns the first received byte
    virtual unsigned char ReceivePackage() = 0;

    // Algorithm to check data integrity during transmission
    virtual unsigned char CalCRC(unsigned char *ptr, int count) = 0;
};

class IXModemReceiver {
public:
    // Function to receive buffer
    virtual XModemByteArray ReceiveBuffer() = 0;

    // Inform Sender to start transmission
    virtual void SendStartTransmission() = 0;

    // inform Sender last package has been successful received
    virtual void SendACK() = 0;

    // inform Sender last package has not been received
    virtual void SendNACK() = 0;

};

class IXModemSender {
public:
    // Function to send buffer
    virtual bool SendBuffer(const XModemByteArray& data)= 0;

    // Inform Receiver the current transmission has ended
    virtual void SendEndTransmission() = 0;

    // Inform Receiver all transmissions have been finished
    virtual void SendFinishTransmission() = 0;

    // Get the feedback ACK or NACK from Receiver
    virtual bool ReceiveACK() = 0;
};
