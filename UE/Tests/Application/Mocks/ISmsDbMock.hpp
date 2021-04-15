#pragma once

#include <gmock/gmock.h>

#include "ISmsDb.hpp"

namespace ue
{

class ISmsDbMock : public ISmsDb
{
public:
    ISmsDbMock() = default;
    ~ISmsDbMock() override = default;

    MOCK_METHOD(void, addReceivedSms, (const Sms&), (final));
};

} // namespace ue