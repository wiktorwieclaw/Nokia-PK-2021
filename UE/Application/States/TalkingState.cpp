#include "TalkingState.hpp"

#include <thread>

#include "ConnectedState.hpp"

using namespace std::chrono_literals;

namespace ue
{
TalkingState::TalkingState(Context& context)
    : BaseState(context, "TalkingState")
{
    // time not given in specification
    context.timer.startTimer(30s);
}

void TalkingState::handleUnknownRecipient()
{
    context.timer.stopTimer();
    context.user.showPartnerNotAvailable();
    std::this_thread::sleep_for(2s);
    context.setState<ConnectedState>();
}

}  // namespace ue