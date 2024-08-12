#include <iostream>

#include "XModemSender.h"

XModemSender::XModemSender(const std::shared_ptr<ITransmission>& commsPtr) {
    m_commsPtr = commsPtr;
}

bool XModemSender::SendBuffer(const XModemByteArray& data) {
    unsigned char firstByte;

    while(m_State != XModemState::FINISHED) {
        switch (m_State) {
            case XModemState::IDLE: {
                {
                    std::lock_guard<std::mutex> lock(m_dataMtx);
                    m_data = data;
                }
                firstByte = ReceiveByte();
                if (firstByte == XMODEM_C) {
                    std::cout << "XMODEM_C received" << std::endl;
                    m_State = XModemState::TRANSMISSION;
                }
                break;
            }
            case XModemState::TRANSMISSION: {
                size_t packageNumber = 1;
                bool transmissionComplete = false;
                size_t dataSent = 0;

                while (!transmissionComplete) {
                    if (!SendPackage(packageNumber)) {
                        // TODO set to error after N attempts
                        std::cerr << "Failed to send package." << std::endl;
                        break;
                    }

                    if (ReceiveACK()) {
                        ++packageNumber;
                        dataSent += XMODEM_PKG_SIZE;
                    } else {
                        std::cerr << "Did not receive ACK. Resending package." << std::endl;
                        break;
                    }

                    {
                        std::lock_guard<std::mutex> lock(m_dataMtx);
                        if (dataSent >= m_data.size()) {
                            SendEndTransmission();
                            if (!ReceiveACK()) {
                                std::cerr << "Did not receive ACK from end of transmission. Resending package." << std::endl;
                                break;
                            }
                            transmissionComplete = true;
                            m_State = XModemState::FINISHED;
                        }
                    }
                    
                }
                break;
            }
            case XModemState::ERROR:
            default: {
                // TODO remove exception and handle the error properly
                throw std::runtime_error("ERROR: Unkown error ocurred!");
                break;
            }
        }
    }

    SendFinishTransmission();
    if (!ReceiveACK()) {
        std::cerr << "Did not receive ACK from finished transmission" << std::endl;
        return false;
    }

    return true;
}

bool XModemSender::ReceiveACK() {
    unsigned char response = ReceiveByte();
    if(response == XMODEM_ACK) {
        std::cout << "ACK received" << std::endl;
        return true;
    }

    std::cout << "NACK received" << std::endl;
    return false;
}

void XModemSender::SendEndTransmission() {
    std::cout << "Send XMODEM_EOT" << std::endl;
    SendByte(XMODEM_EOT);
}

void XModemSender::SendFinishTransmission() {
    std::cout << "Send XMODEM_ETB" << std::endl;
    SendByte(XMODEM_ETB);
}

bool XModemSender::SendByte(const unsigned char byte) {
    return m_commsPtr->SendByte(byte);
}

unsigned char XModemSender::ReceiveByte() {
    return m_commsPtr->ReceiveByte();;
}
