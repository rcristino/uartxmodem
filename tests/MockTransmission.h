#include <gmock/gmock.h>

#include "ITransmission.h"

// Mock class for ITransmission
class MockTransmission : public ITransmission {
public:
    // Mock method for SendByte
    MOCK_METHOD(bool, SendByte, (const unsigned char byte), (override));

    // Mock method for ReceiveByte
    MOCK_METHOD(unsigned char, ReceiveByte, (), (override));
};