#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
public:
    explicit TalkingState(Context& context, PhoneNumber callingNumber);

    // IUserEventsHandler interface
    void handleSendCallDrop() override;
    void handleSendCallTalk(const std::string& message) override;

    // IBtsEventsHandler interface
    void handleReceiveCallDrop() override;
    void handleUnknownRecipient(common::MessageId failingMessageId) override;
    void handleReceiveCallMessage(const std::string& text) override;

    // ITimerPort interface
    void handleTimeout() override;

private:
    common::PhoneNumber callingNumber;
};

}  // namespace ue
