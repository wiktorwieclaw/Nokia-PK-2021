#pragma once

#include "IUeGui.hpp"
#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{
class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showNewSmsNotification() override;
    void viewSmsList(gsl::span<const Sms>) override;
    void viewSms(const Sms&) override;
    void showCallRequest(common::PhoneNumber from) override;
    void showTalking() override;
    void showNewSmsToEdit() override;
    void showPartnerNotAvailable() override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
};

}  // namespace ue
