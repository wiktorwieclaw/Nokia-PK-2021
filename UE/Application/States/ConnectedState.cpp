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

void ConnectedState::handleShowSmsList()
{
     auto& smsMessages = context.smsDb.getSmsMessages();
     context.user.viewSmsList(smsMessages);
}

void ConnectedState::handleShowSms(const unsigned indexOfSms) {
    context.smsDb.updateSmsState(indexOfSms);
    auto& retrivedSms = context.smsDb.getSms(indexOfSms);
    context.user.viewSms(retrivedSms);
}

}  // namespace ue
