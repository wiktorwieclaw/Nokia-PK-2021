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

    MOCK_METHOD(void, addMessage, (const Sms&), (final));
    MOCK_METHOD(gsl::span<const Sms>, getAllMessages, (), (final));
    MOCK_METHOD(void, setMessageState, (gsl::index i, SmsState state), (final));
    MOCK_METHOD(const Sms&, getMessage, (gsl::index i), (final));
};

inline bool operator==(gsl::span<const Sms> lhs, gsl::span<const Sms> rhs)
{
    return lhs.data() == rhs.data() && lhs.size() == rhs.size();
}

}  // namespace ue