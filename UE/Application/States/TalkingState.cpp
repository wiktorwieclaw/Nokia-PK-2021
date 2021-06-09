#include "TalkingState.hpp"

#include <chrono>
#include <thread>

#include "ConnectedState.hpp"

using namespace std::chrono_literals;

namespace ue
{
TalkingState::TalkingState(Context& context, PhoneNumber callingNumber)
    : BaseState(context, "TalkingState"),
      callingNumber{callingNumber}
{
    // max time with no activity
    context.timer.startTimer(120s);
}

void TalkingState::handleUnknownRecipient(common::MessageId failingMessageId)
{
    using common::MessageId;

    if (failingMessageId == MessageId::CallTalk)
    {
        context.timer.stopTimer();
        context.user.showPartnerNotAvailable();

        std::thread([this] {
            std::this_thread::sleep_for(2s);
            context.setState<ConnectedState>();
        }).detach();

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

void TalkingState::handleSendCallTalk(const std::string& message)
{
    context.timer.stopTimer();
    context.timer.startTimer(120s);
    context.bts.sendCallTalkMessage(message, callingNumber);
}
void TalkingState::handleReceiveCallMessage(const std::string& text)
{
    context.timer.stopTimer();
    context.timer.startTimer(120s);
    context.user.showReceivedCallMessage(text);
}

void TalkingState::handleTimeout()
{
    context.bts.sendCallDropped(callingNumber);
    context.setState<ConnectedState>();
}

}  // namespace ue