#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
public:
    explicit TalkingState(Context& context);

    // IUserEventsHandler interface
    void handleCallDrop(common::PhoneNumber to) override;

    // IBtsEventsHandler interface
    void handleReceiveCallDrop(common::PhoneNumber from) override;
};

}  // namespace ue
