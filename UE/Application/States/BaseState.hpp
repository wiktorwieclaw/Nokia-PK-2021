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
    void handleCallAccept(common::PhoneNumber to) override;
    void handleCallDrop(common::PhoneNumber to) override;
    void handleComposeSms() override;
    void handleSendSms(const Sms& sms) override;
    void handleSmsDrop() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSms(const Sms& sms) override;
    void handleCallRequest(common::PhoneNumber from) override;

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}  // namespace ue
