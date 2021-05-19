#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
public:
    explicit TalkingState(Context& context);

    // IUserEventsHandler interface
    void handleCallDrop() override;

    // IBtsEventsHandler interface
    void handleReceiveCallDrop() override;
    void handleUnknownRecipient() override;
};

}  // namespace ue
