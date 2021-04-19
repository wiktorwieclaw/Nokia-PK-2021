#pragma once

#include "Context.hpp"
#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{
using common::ILogger;
using common::PhoneNumber;

class Application : public IEventsHandler
{
public:
    Application(PhoneNumber phoneNumber,
                ILogger& iLogger,
                IBtsPort& bts,
                IUserPort& user,
                ITimerPort& timer,
                ISmsDb& smsDB);
    ~Application() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSms(const Sms& sms) override;

    //IUserEventsHandler interface
    void handleComposeSms() override;
    void handleSendSms() override;

private:
    Context context;
    common::PrefixedLogger logger;
};

}  // namespace ue
