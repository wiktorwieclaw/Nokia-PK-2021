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
    context.smsDb.setMessageState(indexOfSms, SmsState::Viewed);
    const auto& retrievedSms = context.smsDb.getMessage(indexOfSms);
    context.user.viewSms(retrievedSms);
}

void ConnectedState::handleCallRequest(common::PhoneNumber from)
{
    using namespace std::chrono_literals;
    context.user.showCallRequest(from);
    callingNumber = from;
    context.timer.startTimer(30s);
}

void ConnectedState::handleCallAccept()
{
    context.bts.sendCallAccepted(callingNumber.value());
    context.user.showTalking();
    context.timer.stopTimer();
    context.setState<TalkingState>();
}

void ConnectedState::handleCallDrop()
{
    context.timer.stopTimer();
    context.bts.sendCallDropped(callingNumber.value());
    callingNumber.reset();
    context.user.showConnected();
}

void ConnectedState::handleTimeout()
{
    context.bts.sendCallDropped(callingNumber.value());
    callingNumber.reset();
    context.user.showConnected();
}

void ConnectedState::handleComposeSms()
{
    context.user.showNewSmsToEdit();
}

void ConnectedState::handleSendSms(const Sms& sms)
{
    context.smsDb.addMessage(sms);
    context.bts.sendSms(sms);
    context.user.showConnected();
}

void ConnectedState::handleSmsDrop()
{
    context.user.showConnected();
}

void ConnectedState::handleUnknownRecipient()
{
    const auto numberOfMessages = context.smsDb.getNumberOfMessages();
    const auto index = gsl::narrow_cast<gsl::index>(numberOfMessages) - 1;
    context.smsDb.setMessageState(index, SmsState::Failed);
}

}  // namespace ue
