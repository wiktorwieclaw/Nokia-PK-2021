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

    // IUserEventsHandler interface
    void handleShowSmsList() override;
    void handleShowSms(IUeGui::IListViewMode::Selection) override;
    void handleSmsDrop() override;
    void handleSendCallAccept() override;
    void handleSendCallDrop() override;
    void handleComposeSms() override;
    void handleSendSms(const Sms& sms) override;
    void handleStartDial() override;
    void handleSendCallRequest(common::PhoneNumber from, common::PhoneNumber to) override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSms(const Sms& sms) override;
    void handleReceiveCallRequest(common::PhoneNumber from) override;
    void handleReceiveCallAccept(common::PhoneNumber from) override;
    void handleUnknownRecipient(common::MessageId failingMessageId) override;
    void handleReceiveCallDrop() override;


private:
    Context context;
    common::PrefixedLogger logger;
};

}  // namespace ue
