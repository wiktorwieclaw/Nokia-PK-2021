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
    //ToDo extracting data from gui
    //Temporary
    const common::PhoneNumber receiverPhoneNumber{112};
    const std::string textExample = "example";

    context.smsDb.addSms(receiverPhoneNumber,textExample);
    context.bts.sendSms(receiverPhoneNumber,textExample);
    context.user.showConnected();
}

}  // namespace ue
