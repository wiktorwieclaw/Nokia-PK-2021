#pragma once

#include <gmock/gmock.h>

#include "Ports/IUserPort.hpp"
#include "Sms.hpp"

namespace ue
{
class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

    MOCK_METHOD(void, handleShowSmsList, (), (final));
    MOCK_METHOD(void, handleShowSms, (IUeGui::IListViewMode::Selection), (final));
    MOCK_METHOD(void, handleCallAccept, (), (final));
    MOCK_METHOD(void, handleCallDrop, (), (final));
    MOCK_METHOD(void, handleComposeSms, (), (final));
    MOCK_METHOD(void, handleSendSms, (const Sms&), (final));
    MOCK_METHOD(void, handleSmsDrop, (), (final));
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
    MOCK_METHOD(void, showNewSmsToEdit, (), (final));
    MOCK_METHOD(void, viewSmsList, (gsl::span<const Sms> messages), (final));
    MOCK_METHOD(void, viewSms, (const Sms&), (final));
    MOCK_METHOD(void, showCallRequest, (common::PhoneNumber from), (final));
    MOCK_METHOD(void, showTalking, (), (final));
};

}  // namespace ue
