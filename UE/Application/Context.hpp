#pragma once

#include <memory>

#include "IEventsHandler.hpp"
#include "ISmsDb.hpp"
#include "Logger/ILogger.hpp"

namespace ue
{
struct Context
{
    common::ILogger& logger;
    IBtsPort& bts;
    IUserPort& user;
    ITimerPort& timer;
    ISmsDb& smsDb;
    std::unique_ptr<IEventsHandler> state{};

    template <typename State, typename... Arg>
    void setState(Arg&&... arg)
    {
        state = std::make_unique<State>(*this, std::forward<Arg>(arg)...);
    }
};

}  // namespace ue
