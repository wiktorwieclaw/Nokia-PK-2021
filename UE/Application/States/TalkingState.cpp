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

void TalkingState::handleUnknownRecipient(common::MessageId failingMessageId)
{
    using common::MessageId;

    if (failingMessageId == MessageId::CallTalk) {
        context.timer.stopTimer();
        context.user.showPartnerNotAvailable();
        context.setState<ConnectedState>();
        return;
    }

    logger.logError("Unknown failing message ID");
}

void TalkingState::handleSendCallDrop()
{
    context.bts.sendCallDropped(callingNumber);
    context.setState<ConnectedState>();
}

void TalkingState::handleReceiveCallDrop()
{
    context.user.showCallEndedByPartner();
    context.setState<ConnectedState>();
}

}  // namespace ue