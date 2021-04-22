#include "ConnectedState.hpp"

#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{
ConnectedState::ConnectedState(Context& context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}

void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::handleSms(const Sms& sms)
{
    context.user.showNewSmsNotification();
    context.smsDb.addReceivedSms(sms);
}

void ConnectedState::handleCallRequest(common::PhoneNumber from)
{
    using namespace std::chrono_literals;
    context.user.showCallRequest(from);
    context.timer.startTimer(30s);
}

void ConnectedState::handleCallAccept(common::PhoneNumber to)
{
    context.bts.sendCallAccepted(to);
    context.user.showTalking();
    context.timer.stopTimer();
    context.setState<TalkingState>();
}

}  // namespace ue
