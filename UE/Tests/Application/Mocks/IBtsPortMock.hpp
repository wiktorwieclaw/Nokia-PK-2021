#pragma once

#include <gmock/gmock.h>

#include "Ports/IBtsPort.hpp"

namespace ue
{
class Sms;

class IBtsEventsHandlerMock : public IBtsEventsHandler
{
public:
    IBtsEventsHandlerMock();
    ~IBtsEventsHandlerMock() override;

    MOCK_METHOD(void, handleDisconnected, (), (final));
    MOCK_METHOD(void, handleSib, (common::BtsId), (final));
    MOCK_METHOD(void, handleAttachAccept, (), (final));
    MOCK_METHOD(void, handleAttachReject, (), (final));
    MOCK_METHOD(void, handleSms, (const Sms& sms), (final));
    MOCK_METHOD(void, handleReceiveCallRequest, (common::PhoneNumber from), (final));
    MOCK_METHOD(void, handleReceiveCallAccept, (common::PhoneNumber from, common::PhoneNumber to), (final));

};

class IBtsPortMock : public IBtsPort
{
public:
    IBtsPortMock();
    ~IBtsPortMock() override;

    MOCK_METHOD(void, sendAttachRequest, (common::BtsId), (final));
    MOCK_METHOD(void, sendCallAccepted, (common::PhoneNumber to), (final));
    MOCK_METHOD(void, sendCallDropped, (common::PhoneNumber to), (final));
    MOCK_METHOD(void, sendSms, (const Sms&), (final));
    MOCK_METHOD(void, sendCallRequest, (common::PhoneNumber from, common::PhoneNumber to), (final));
};

}  // namespace ue
