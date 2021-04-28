#pragma once

#include <gmock/gmock.h>

#include "Sms.hpp"
#include "ISmsDb.hpp"

namespace ue
{
class Sms;

class ISmsDbMock : public ISmsDb
{
public:
    ISmsDbMock();
    ~ISmsDbMock() override;

    MOCK_METHOD(void, addReceivedSms, (const Sms&), (final));
    MOCK_METHOD(const SmsMessages&, getSmsMessages, (), (final));
    MOCK_METHOD(void, updateSmsState, (SmsMessages::size_type), (final));
    MOCK_METHOD(const Sms&, getSms, (SmsMessages::size_type), (final));
};

}  // namespace ue