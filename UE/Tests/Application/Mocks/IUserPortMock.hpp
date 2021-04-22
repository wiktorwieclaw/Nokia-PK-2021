#pragma once

#include <gmock/gmock.h>

#include "Ports/IUserPort.hpp"

namespace ue
{
class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

    MOCK_METHOD(void, handleShowSmsList, (), (final));
    MOCK_METHOD(void, handleShowSms, (const unsigned), (final));
};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showNewSmsNotification, (), (final));
    MOCK_METHOD(void, viewSmsList, (const SmsMessages&), (final));
    MOCK_METHOD(void, viewSms, (const Sms&), (final));
};

}  // namespace ue
