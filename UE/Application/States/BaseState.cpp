#include "BaseState.hpp"

namespace ue
{
BaseState::BaseState(Context& context, const std::string& name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Unexpected: handleTimeout");
}

void BaseState::handleDisconnected()
{
    logger.logError("Unexpected: handleDisconnected");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Unexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Unexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Unexpected: handleAttachReject");
}

void BaseState::handleSms(const Sms& sms)
{
    logger.logError("Unexpected: handleSms");
}

void BaseState::handleComposeSms()
{
    logger.logError("Unexpected: handleComposeSms");
}

void BaseState::handleSendSms(const Sms& sms)
{
    logger.logError("Unexpected: handleSendSms");
}

void BaseState::handleShowSmsList()
{
    logger.logError("Unexpected: handleShowSmsList");
}

void BaseState::handleShowSms(IUeGui::IListViewMode::Selection) {
    logger.logError("Unexpected: handleShowSms");
}

void BaseState::handleCallRequest(common::PhoneNumber from)
{
    logger.logError("Unexpected: handleCallRequest");
}

void BaseState::handleCallAccept()
{
    logger.logError("Unexpected: handleCallAccept");
}

void BaseState::handleCallDrop()
{
    logger.logError("Unexpected: handleCallDrop");
}

void BaseState::handleSmsDrop()
{
    logger.logError("Unexpected: handleSmsDrop");
}

void BaseState::handleUnknownRecipient()
{
    logger.logError("Unexpected: handleUnknownRecipient");
}

void BaseState::handleReceiveCallDrop(common::PhoneNumber from)
{
    logger.logError("Unexpected: handleCallDropped");
}

}  // namespace ue
