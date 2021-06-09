#include "Application.hpp"

#include "States/NotConnectedState.hpp"

namespace ue
{
Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger& iLogger,
                         IBtsPort& bts,
                         IUserPort& user,
                         ITimerPort& timer,
                         ISmsDb& smsDb)
    : context{iLogger, bts, user, timer, smsDb},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleDisconnected()
{
    context.state->handleDisconnected();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleSms(const Sms& sms)
{
    context.state->handleSms(sms);
}

void Application::handleShowSmsList()
{
    context.state->handleShowSmsList();
}

void Application::handleShowSms(IUeGui::IListViewMode::Selection indexOfSms)
{
    context.state->handleShowSms(indexOfSms);
}

void Application::handleReceiveCallRequest(common::PhoneNumber from)
{
    context.state->handleReceiveCallRequest(from);
}

void Application::handleSendCallAccept()
{
    context.state->handleSendCallAccept();
}

void Application::handleSendCallDrop()
{
    context.state->handleSendCallDrop();
}

void Application::handleComposeSms()
{
    context.state->handleComposeSms();
}

void Application::handleSendSms(const Sms& sms)
{
    context.state->handleSendSms(sms);
}
void Application::handleStartDial()
{
    context.state->handleStartDial();
}
void Application::handleSendCallRequest(common::PhoneNumber from, common::PhoneNumber to)
{
    context.state->handleSendCallRequest(from, to);
}
void Application::handleReceiveCallAccept(common::PhoneNumber from)
{
    context.state->handleReceiveCallAccept(from);
}

void Application::handleSmsDrop()
{
    context.state->handleSmsDrop();
}

void Application::handleUnknownRecipient(common::MessageId failingMessageId)
{
    context.state->handleUnknownRecipient(failingMessageId);
}

void Application::handleReceiveCallDrop()
{
    context.state->handleReceiveCallDrop();
}
void Application::handleSendCallResignation(common::PhoneNumber correspondent)
{
    context.state->handleSendCallResignation(correspondent);
}

void Application::handleSendCallTalk(const std::string& message)
{
    context.state->handleSendCallTalk(message);
}
void Application::handleReceiveCallMessage(const std::string& text)
{
    context.state->handleReceiveCallMessage(text);
}

}  // namespace ue