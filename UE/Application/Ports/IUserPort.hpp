#pragma once

#include <gsl/span>

#include "Messages/PhoneNumber.hpp"
#include "UeGui/IListViewMode.hpp"

namespace ue
{
class Sms;

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleShowSmsList() = 0;
    virtual void handleShowSms(IUeGui::IListViewMode::Selection) = 0;
    virtual void handleCallAccept(common::PhoneNumber to) = 0;
    virtual void handleCallDrop(common::PhoneNumber to) = 0;
    virtual void handleComposeSms() = 0;
    virtual void handleSendSms(const Sms& sms) = 0;
    virtual void handleCallDropWhenTalking(common::PhoneNumber to) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showNewSmsNotification() = 0;
    virtual void viewSmsList(gsl::span<Sms const>) = 0;
    virtual void viewSms(const Sms&) = 0;
    virtual void showCallRequest(common::PhoneNumber from) = 0;
    virtual void showTalking() = 0;
    virtual void showNewSmsToEdit() = 0;
};

}  // namespace ue
