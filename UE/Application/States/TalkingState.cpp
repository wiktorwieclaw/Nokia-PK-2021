#include <chrono>
#include <thread>

#include "TalkingState.hpp"
#include "ConnectedState.hpp"

using namespace std::chrono_literals;

namespace ue
{
TalkingState::TalkingState(Context& context, PhoneNumber callingNumber)
    : BaseState(context, "TalkingState"), callingNumber{callingNumber}
{
    // time not given in specification
    context.timer.startTimer(30s);
}

void TalkingState::handleUnknownRecipient()
{
    context.timer.stopTimer();
    context.user.alertUser("Partner not available");
    context.setState<ConnectedState>();
}

void TalkingState::handleSendCallDrop()
{
    context.bts.sendCallDropped(callingNumber);
    context.setState<ConnectedState>();
}

void TalkingState::handleReceiveCallDrop()
{
    context.user.alertUser("Call ended by partner");
    context.setState<ConnectedState>();
}

}  // namespace ue