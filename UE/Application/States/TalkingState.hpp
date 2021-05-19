#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
public:
    explicit TalkingState(Context& context);
    void handleUnknownRecipient() override;
};

}  // namespace ue
