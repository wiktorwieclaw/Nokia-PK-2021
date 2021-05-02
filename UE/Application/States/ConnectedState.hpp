#pragma once

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
    void handleReceiveCallRequest(common::PhoneNumber from) final;

    // IUserEventsHandler interface
    void handleSendCallAccept(common::PhoneNumber to) final;
    void handleSendCallDrop(common::PhoneNumber to) final;

    // IUserEventsHandler interface
    void handleComposeSms() final;
    void handleSendSms(const Sms& sms) final;
};

}  // namespace ue
