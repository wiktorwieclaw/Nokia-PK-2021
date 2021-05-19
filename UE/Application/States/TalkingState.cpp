#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{
TalkingState::TalkingState(Context& context)
    : BaseState(context, "TalkingState")
{}

void TalkingState::handleCallDrop(common::PhoneNumber to)
{
    context.bts.sendCallDropped(to);
    context.setState<ConnectedState>();
}

void TalkingState::handleReceiveCallDrop(common::PhoneNumber from)
{
    context.setState<ConnectedState>();
}

}  // namespace ue