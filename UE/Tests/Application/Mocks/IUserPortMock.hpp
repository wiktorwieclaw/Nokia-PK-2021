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
    MOCK_METHOD(void, handleShowSms, (IUeGui::IListViewMode::Selection), (final));
    MOCK_METHOD(void, handleCallAccept, (common::PhoneNumber to), (final));
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
    MOCK_METHOD(void, viewSmsList, (const ISmsDb::SmsMessages&), (final));
    MOCK_METHOD(void, viewSms, (const Sms&), (final));
    MOCK_METHOD(void, showCallRequest, (common::PhoneNumber from), (final));
    MOCK_METHOD(void, showTalking, (), (final));
};

}  // namespace ue
