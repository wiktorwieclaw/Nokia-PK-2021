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

    // IUserEventsHandler interface
    void handleComposeSms() final;
    void handleSendSms() final;
};

}  // namespace ue
