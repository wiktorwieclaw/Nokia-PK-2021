#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
public:
    explicit TalkingState(Context& context);

    // IUserEventsHandler interface
    void handleCallDropWhenTalking(common::PhoneNumber to) override;
};

}  // namespace ue
