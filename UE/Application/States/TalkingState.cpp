#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{
TalkingState::TalkingState(Context& context)
    : BaseState(context, "TalkingState")
{}

void TalkingState::handleCallDropWhenTalking(common::PhoneNumber to)
{
    context.bts.sendCallDropped(to);
    context.setState<ConnectedState>();
}

}  // namespace ue