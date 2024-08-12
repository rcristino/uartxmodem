#pragma once

#include <memory>

#include "ITransmission.h"
#include "XModem.h"

class XModemSender: public XModem, IXModemSender {
public:
    // Constructor
    XModemSender(const std::shared_ptr<ITransmission>& commsPtr);

    bool SendBuffer(const XModemByteArray& data);
private:
    void SendEndTransmission() override;
    void SendFinishTransmission() override;
    bool ReceiveACK() override;
    bool SendByte(const unsigned char byte) override;
    unsigned char ReceiveByte() override;

    XModemState m_State{XModemState::IDLE};
    std::shared_ptr<ITransmission> m_commsPtr;
};
