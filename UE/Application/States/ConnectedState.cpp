#include "ConnectedState.hpp"

#include "NotConnectedState.hpp"
#include "TalkingState.hpp"
#include "Sms.hpp"

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
    context.smsDb.setMessageState(indexOfSms, SmsState::Viewed);
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

void ConnectedState::handleCallDrop(common::PhoneNumber to)
{
    context.timer.stopTimer();
    context.bts.sendCallDropped(to);
    context.user.showConnected();
}

}  // namespace ue
