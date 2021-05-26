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
    void handleCallTalk(std::string_view message) override;

    // IBtsEventsHandler interface
    void handleReceiveCallDrop() override;
    void handleUnknownRecipient(common::MessageId failingMessageId) override;

private:
    common::PhoneNumber callingNumber;
};

}  // namespace ue
