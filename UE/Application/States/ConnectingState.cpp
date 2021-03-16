#include "ConnectingState.hpp"

namespace ue
{

ConnectingState::ConnectingState(Context &context, common::BtsId btsId)
    : BaseState(context, "ConnectingState")
{
    context.bts.sendAttachRequest(btsId);
    using namespace std::chrono_literals;
    context.timer.startTimer(500ms);
}

}
