#pragma once

#include <vector>
#include "SmsDb.hpp"
#include "UeGui/IListViewMode.hpp"

namespace ue
{
class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleShowSmsList() = 0;
    virtual void handleShowSms(IUeGui::IListViewMode::Selection) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showNewSmsNotification() = 0;
    virtual void viewSmsList(const ISmsDb::SmsMessages&) = 0;
    virtual void viewSms(const Sms&) = 0;
};

}  // namespace ue
