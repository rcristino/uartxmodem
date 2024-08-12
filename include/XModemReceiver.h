#pragma once

#include <memory>

#include "ITransmission.h"
#include "XModem.h"

class XModemReceiver : public XModem, IXModemReceiver {
public:
    // Constructor
    XModemReceiver(const std::shared_ptr<ITransmission>& commsPtr);

    XModemByteArray ReceiveBuffer();
private:
    void SendStartTransmission() override;
    void SendACK() override;
    void SendNACK() override;
    bool SendByte(const unsigned char byte) override ;
    unsigned char ReceiveByte() override ;

    XModemState m_State{XModemState::IDLE};
    std::shared_ptr<ITransmission> m_commsPtr;
};
