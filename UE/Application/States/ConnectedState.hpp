#pragma once

#include "BaseState.hpp"

namespace ue
{
class ConnectedState : public BaseState
{
public:
    explicit ConnectedState(Context& context);

public:
    // IUserEventsHandler interface
    void handleShowSmsList() final;
    void handleShowSms(IUeGui::IListViewMode::Selection) final;
    void handleSendCallAccept(common::PhoneNumber to) final;
    void handleSendCallDrop(common::PhoneNumber to) final;
    void handleComposeSms() final;
    void handleSendSms(const Sms& sms) final;
    void handleStartDial() override;
    void handleSendCallRequest(common::PhoneNumber from, common::PhoneNumber to) override;

    // IBtsEventsHandler interface
    void handleDisconnected() final;
    void handleSms(const Sms& sms) final;
    void handleReceiveCallRequest(common::PhoneNumber from) final;
    void handleReceiveCallAccept(common::PhoneNumber from, common::PhoneNumber to) override;
};

}  // namespace ue
