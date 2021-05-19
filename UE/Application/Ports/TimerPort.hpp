#pragma once

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace ue
{
class TimerPort : public ITimerPort
{
public:
    explicit TimerPort(common::ILogger& logger);

    void start(ITimerEventsHandler& handler);
    void stop();

    // ITimerPort interface
    void startTimer(Duration duration) override;
    void stopTimer() override;

private:
    common::PrefixedLogger logger;
    ITimerEventsHandler* handler = nullptr;
    bool running = false;
};

}  // namespace ue
