#pragma once

#include "Messages/BtsId.hpp"
#include "Sms.hpp"

namespace ue
{

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleDisconnected() = 0;
    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleSms(const Sms& sms) = 0;
};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
};

}
