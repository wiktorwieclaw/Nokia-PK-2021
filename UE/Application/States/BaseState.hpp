#pragma once

#include "Context.hpp"
#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace ue
{
class Sms;

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // IUserEventsHandler interface
    void handleShowSmsList() override;
    void handleShowSms(IUeGui::IListViewMode::Selection) override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSms(const Sms& sms) override;
    void handleReceiveCallRequest(common::PhoneNumber from) override;

    // IUserEventsHandler interface
    void handleSendCallAccept(common::PhoneNumber to) override;
    void handleSendCallDrop(common::PhoneNumber to) override;

    // IUserEventsHandler interface
    void handleComposeSms() override;
    void handleSendSms(const Sms& sms) override;

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}  // namespace ue
