#pragma once

#include <optional>

#include "BaseState.hpp"

namespace ue
{
class ConnectedState : public BaseState
{
public:
    explicit ConnectedState(Context& context);

    // IBtsEventsHandler interface
public:
    void handleDisconnected() final;
    void handleSms(const Sms& sms) final;
    void handleShowSmsList() final;
    void handleShowSms(IUeGui::IListViewMode::Selection) final;
    void handleCallRequest(common::PhoneNumber from) final;
    void handleSmsDrop() final;
    void handleUnknownRecipient() final;

    // IUserEventsHandler interface
    void handleComposeSms() final;
    void handleSendSms(const Sms& sms) final;
    void handleSendCallAccept() final;
    void handleSendCallDrop() final;
    void handleStartDial() override;
    void handleSendCallRequest(common::PhoneNumber from, common::PhoneNumber to) override;

    // ITimerEventsHandler
    void handleTimeout() override;

private:
    std::optional<common::PhoneNumber> callingNumber;
};

}  // namespace ue
