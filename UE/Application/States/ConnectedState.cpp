#include "ConnectedState.hpp"

#include <chrono>
#include <thread>

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
    const auto& retrievedSms = context.smsDb.getMessage(indexOfSms);

    if (retrievedSms.state == SmsState::NotViewed)
    {
        context.smsDb.setMessageState(indexOfSms, SmsState::Viewed);
    }

    context.user.viewSms(retrievedSms);
}

void ConnectedState::handleReceiveCallRequest(common::PhoneNumber from)
{
    using namespace std::chrono_literals;
    context.user.showCallRequest(from);
    callingNumber = from;
    context.timer.startTimer(30s);
}

void ConnectedState::handleSendCallAccept()
{
    try
    {
        context.bts.sendCallAccepted(callingNumber.value());
        context.user.showTalking();
        context.timer.stopTimer();
        context.setState<TalkingState>(callingNumber.value());
    }
    catch (std::exception& e)
    {
        logger.logError(std::string{"ConnectedState::handleSendCallAccept(): "} + e.what());
    }
}

void ConnectedState::handleSendCallDrop()
{
    context.timer.stopTimer();

    try
    {
        context.bts.sendCallDropped(callingNumber.value());
    }
    catch (std::exception& e)
    {
        logger.logError(std::string{"ConnectedState::handleSendCallDrop(): "} + e.what());
    }

    callingNumber.reset();
    context.user.showConnected();
}

void ConnectedState::handleTimeout()
{
    try
    {
        context.bts.sendCallDropped(callingNumber.value());
    }
    catch (std::exception& e)
    {
        logger.logError(std::string{"ConnectedState::handleTimeout(): "} + e.what());
    }
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
void ConnectedState::handleStartDial()
{
    context.user.showEnterPhoneNumber();
}
void ConnectedState::handleSendCallRequest(common::PhoneNumber from, common::PhoneNumber to)
{
    context.user.showDialing(to);
    context.bts.sendCallRequest(from, to);
    using namespace std::chrono_literals;
    context.timer.startTimer(60s);
}
void ConnectedState::handleReceiveCallAccept(common::PhoneNumber from)
{
    callingNumber = from;
    context.user.showTalking();
    context.timer.stopTimer();
    try
    {
        context.setState<TalkingState>(callingNumber.value());
    }
    catch (std::exception& e)
    {
        logger.logError(std::string{"ConnectedState::handleReceiveCallAccept(): "} + e.what());
    }
}

void ConnectedState::handleSmsDrop()
{
    context.user.showConnected();
}

void ConnectedState::handleUnknownRecipient(common::MessageId failingMessageId)
{
    using namespace std::chrono_literals;
    using common::MessageId;

    switch (failingMessageId)
    {
    case MessageId::CallRequest:
    {
        context.timer.stopTimer();
        context.user.showPartnerNotAvailable();

        std::thread([this] {
            std::this_thread::sleep_for(2s);
            context.user.showConnected();
        }).detach();

        break;
    }
    case MessageId::Sms:
    {
        const auto numberOfMessages = context.smsDb.getNumberOfMessages();
        const auto index = gsl::narrow_cast<gsl::index>(numberOfMessages) - 1;
        context.smsDb.setMessageState(index, SmsState::Failed);
        break;
    }
    default:
    {
        logger.logError("Unhandled unknown recipient - messageID: "+to_string(failingMessageId));
    }
    }
}

void ConnectedState::handleSendCallResignation(common::PhoneNumber correspondent)
{
    context.timer.stopTimer();
    context.bts.sendCallDropped(correspondent);
    context.user.showConnected();
}

void ConnectedState::handleReceiveCallDrop()
{
    context.user.showConnected();
    context.timer.stopTimer();
}

}  // namespace ue
