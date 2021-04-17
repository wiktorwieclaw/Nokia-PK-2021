#pragma once

#include <gmock/gmock.h>

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
};

}  // namespace ue