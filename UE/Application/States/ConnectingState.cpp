#include "ConnectingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

ConnectingState::ConnectingState(Context &context, common::BtsId btsId)
    : BaseState(context, "ConnectingState")
{
    context.user.showConnecting();
    context.bts.sendAttachRequest(btsId);
    using namespace std::chrono_literals;
    context.timer.startTimer(500ms);
}

void ConnectingState::handleTimeout()
{
}

void ConnectingState::handleAttachAccept()
{
    context.timer.stopTimer();
    context.setState<ConnectedState>();
}

void ConnectingState::handleAttachReject()
{
}

}


