#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{
class Sms;

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleDisconnected() = 0;
    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleSms(const Sms& sms) = 0;
    virtual void handleReceiveCallRequest(common::PhoneNumber from) = 0;
    virtual void handleReceiveCallDrop() = 0;
    virtual void handleUnknownRecipient() = 0;
    virtual void handleReceiveCallAccept(common::PhoneNumber from, common::PhoneNumber to) = 0;
};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
    virtual void sendSms(const Sms &sms) = 0;
    virtual void sendCallAccepted(common::PhoneNumber to) = 0;
    virtual void sendCallDropped(common::PhoneNumber to) = 0;
    virtual void sendCallRequest(common::PhoneNumber from, common::PhoneNumber to) = 0;
};

}  // namespace ue
