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
    void handleCallRequest(common::PhoneNumber from) final;

    // IUserEventsHandler interface
    void handleCallAccept(common::PhoneNumber to) final;
    void handleCallDrop(common::PhoneNumber to) final;

    // IUserEventsHandler interface
    void handleComposeSms() final;
    void handleSendSms(const Sms& sms) final;
    void handleSmsDrop() final;
};

}  // namespace ue
