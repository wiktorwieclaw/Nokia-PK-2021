#include "TimerPort.hpp"

#include <thread>

namespace ue
{
TimerPort::TimerPort(common::ILogger& logger)
    : logger(logger, "[TIMER PORT]")
{}

void TimerPort::start(ITimerEventsHandler& handler)
{
    logger.logDebug("Started");
    this->handler = &handler;
}

void TimerPort::stop()
{
    logger.logDebug("Stopped");
    handler = nullptr;
}

void TimerPort::startTimer(Duration duration)
{
    using namespace std::chrono_literals;
    logger.logDebug("Start timer: ", duration.count(), "ms");

    constexpr auto interval = 100ms;
    running = true;

    future = std::async([this, interval, numCycles = duration / interval] {
        for (auto i = decltype(numCycles){0}; i < numCycles; ++i)
        {
            std::this_thread::sleep_for(interval);

            if (!running)
            {
                return;
            }
        }

        handler->handleTimeout();
        running = false;
    });
}

void TimerPort::stopTimer()
{
    running = false;
    future.get();
}

}  // namespace ue
