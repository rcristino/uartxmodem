#include <iostream>

#include "XModemReceiver.h"

XModemReceiver::XModemReceiver(const std::shared_ptr<ITransmission>& commsPtr) {
    m_commsPtr = commsPtr;
}

XModemByteArray XModemReceiver::ReceiveBuffer() {
    unsigned char firstByte;
    XModemByteArray data;

    while(m_State != XModemState::FINISHED) {
        switch (m_State) {
            case XModemState::IDLE: {
                data.clear();
                SendStartTransmission();
                m_State = XModemState::TRANSMISSION;
                break;
            }
            case XModemState::TRANSMISSION: {
                size_t packageNumber = 1;
                bool transmissionComplete = false;

                while (!transmissionComplete) {
                    unsigned char firstByte = ReceivePackage();
                    switch (firstByte)
                    {
                        case XMODEM_SOH: {
                            std::cout << "XMODEM_SOH received" << std::endl;
                            SendACK();
                            ++packageNumber;
                            break;
                        }
                        case XMODEM_EOT: {
                            std::cout << "XMODEM_EOT received" << std::endl;
                            SendACK();
                            transmissionComplete = true;

                            {
                                std::lock_guard<std::mutex> lock(m_dataMtx);
                                data.insert(data.end(), m_data.begin(), m_data.end());
                            }

                            m_State = XModemState::FINISHED;
                            break;
                        }
                        default: {
                            std::cerr << "Failed to receive package." << std::endl;
                            SendNACK();
                            break;
                        }
                    }
                }
                break;
            }
            case XModemState::ERROR:
            default: {
                throw std::runtime_error("ERROR: Unkown error ocurred!");
                break;
            }
        }
    }

    unsigned char endCmd = ReceiveByte();
    if (endCmd == XMODEM_ETB) {
        SendACK();
    }
    else {
        std::cerr << "No termination command received." << std::endl;
    }

    return data;
}

void XModemReceiver::SendStartTransmission() {
    std::cout << "send XMODEM_C" << std::endl;
    SendByte(XMODEM_C);
}

void XModemReceiver::SendACK() {
    std::cout << "send XMODEM_ACK" << std::endl;
    SendByte(XMODEM_ACK);
}

void XModemReceiver::SendNACK() {
    std::cout << "send XMODEM_NACK" << std::endl;
    SendByte(XMODEM_NACK);
}

bool XModemReceiver::SendByte(const unsigned char byte) {
    return  m_commsPtr->SendByte(byte);
}

unsigned char XModemReceiver::ReceiveByte() {
    return m_commsPtr->ReceiveByte();;
}

