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
    //Todo delete this test adding sms
    common::PhoneNumber phoneNumber;
    phoneNumber.value = 122;
    Sms sms1{phoneNumber,"test122"};
    phoneNumber.value = 125;
    Sms sms2{phoneNumber,"test125"};
    context.smsDb.addReceivedSms(sms1);
    context.smsDb.addReceivedSms(sms2);

    context.user.viewSmsList(context.smsDb.getSmsMessages());
}

void ConnectedState::handleShowSms(const unsigned indexOfSms) {
    context.smsDb.updateSmsState(indexOfSms);
    context.user.viewSms(context.smsDb.getSms(indexOfSms));
}

}  // namespace ue
