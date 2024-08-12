#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#include "TransmissionUart.h"

bool TransmissionUartReceiver::StartReceiver(const char* portRecv) {
    fdReceiver = open(portRecv
        , O_RDWR  /* Open Rend and Write */
        | O_NOCTTY /* No Controlling Terminal */
        );

    if (fdReceiver == -1) {
        std::cerr << "TransmissionUartReceiver: Error opening Receiver port" << std::endl;
        return false;
    }

    return true;
}

void TransmissionUartReceiver::StopReceiver() {
    close(fdReceiver);
}


bool TransmissionUartReceiver::SendByte(const unsigned char byte) {
    if(!fdReceiver) {
        std::cerr << "TransmissionUartReceiver: port not initialized. " << std::endl;
        return false;
    }

    int n = write(fdReceiver, &byte, 1 /* write one byte each time*/);
    if (n < 0) {
        std::cerr << "TransmissionUartReceiver: Write error" << std::endl;
        return false;
    }

    return true;
}

unsigned char TransmissionUartReceiver::ReceiveByte() {
    unsigned char byte = '\0';
    if(!fdReceiver) {
        std::cerr << "TransmissionUartReceiver: port is not initialized. " << std::endl;
        return byte;
    }

    int n = read(fdReceiver, &byte, 1 /* read one byte each time*/);

    if (n < 0) {
        std::cerr << "TransmissionUartReceiver: Read error" << std::endl;
        return '\0';
    }

    return byte;
}

bool TransmissionUartSender::StartSender(const char* portSender) {
    fdSender = open(portSender
        , O_RDWR  /* Open Rend and Write */
        | O_NOCTTY /* No Controlling Terminal */
        );

    if (fdSender == -1) {
        std::cerr << "TransmissionUartSender: Error opening Sender port" << std::endl;
        return false;
    }

    return true;
}

void TransmissionUartSender::StopSender() {
    close(fdSender);
}

bool TransmissionUartSender::SendByte(const unsigned char byte) {
    if(!fdSender) {
        std::cerr << "TransmissionUartSender: port is not initialized. " << std::endl;
        return false;
    }

    int n = write(fdSender, &byte, 1 /* write one byte each time*/);
    if (n < 0) {
        std::cerr << "TransmissionUartSender: Write error" << std::endl;
        return false;
    }

    return true;
}

unsigned char TransmissionUartSender::ReceiveByte() {
    unsigned char byte = '\0';
    if(!fdSender) {
        std::cerr << "TransmissionUartSender: port is not initialized. " << std::endl;
        return byte;
    }

    int n = read(fdSender, &byte, 1 /* read one byte each time*/);

    if (n < 0) {
        std::cerr << "TransmissionUartSender: Read error" << std::endl;
        return '\0';
    }

    return byte;
}
