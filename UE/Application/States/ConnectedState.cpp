#include "ConnectedState.hpp"

#include "NotConnectedState.hpp"

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

void ConnectedState::handleComposeSms()
{
    context.user.showNewSmsToEdit();
}

void ConnectedState::handleSendSms()
{
    auto [phoneNumber,smsText] = context.user.getSmsData();

    context.smsDb.addSms(phoneNumber,smsText);
    context.bts.sendSms(phoneNumber,smsText);
    context.user.showConnected();
}

}  // namespace ue
