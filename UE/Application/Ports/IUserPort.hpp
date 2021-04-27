#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue
{
class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleCallAccept(common::PhoneNumber to) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showNewSmsNotification() = 0;
    virtual void showCallRequest(common::PhoneNumber from) = 0;
    virtual void showTalking() = 0;
};

}  // namespace ue
