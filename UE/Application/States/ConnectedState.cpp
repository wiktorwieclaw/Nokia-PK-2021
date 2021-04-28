#include "ConnectedState.hpp"

#include "NotConnectedState.hpp"
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
    context.smsDb.addReceivedSms(sms);
}

void ConnectedState::handleShowSmsList()
{
     const auto& smsMessages = context.smsDb.getSmsMessages();
     context.user.viewSmsList(smsMessages);
}

void ConnectedState::handleShowSms(IUeGui::IListViewMode::Selection indexOfSms) {
    context.smsDb.updateSmsState(indexOfSms);
    const auto& retrievedSms = context.smsDb.getSms(indexOfSms);
    context.user.viewSms(retrievedSms);
}

}  // namespace ue
