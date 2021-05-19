#include "ConnectedState.hpp"

#include "NotConnectedState.hpp"
#include "Sms.hpp"
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
    context.smsDb.addMessage(sms);
}

void ConnectedState::handleShowSmsList()
{
    const auto& smsMessages = context.smsDb.getAllMessages();
    context.user.viewSmsList(smsMessages);
}

void ConnectedState::handleShowSms(IUeGui::IListViewMode::Selection indexOfSms)
{
    if (const auto state = context.smsDb.getMessage(indexOfSms).state;
        state == SmsState::NotViewed)
    {
        context.smsDb.setMessageState(indexOfSms, SmsState::Viewed);
    }
    const auto& retrievedSms = context.smsDb.getMessage(indexOfSms);
    context.user.viewSms(retrievedSms);
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
