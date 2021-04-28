#include <gtest/gtest.h>

#include "Messages/PhoneNumber.hpp"
#include "Sms.hpp"
#include "SmsDb.hpp"

namespace ue
{
using namespace ::testing;

class SmsDbTestSuite : public Test
{
protected:
    static constexpr common::PhoneNumber phoneNumber{112};

    SmsDb objectUnderTest;
};

TEST_F(SmsDbTestSuite, shallAddReceivedSms)
{
    objectUnderTest.addReceivedSms(Sms{phoneNumber, "example text"});

    const auto& messages = objectUnderTest.getSmsMessages();
    const auto& [_, smsState] = messages[0];
    ASSERT_EQ(smsState, SmsState::NotViewed);
}

TEST_F(SmsDbTestSuite, shallgetSms)
{
    const Sms sms{phoneNumber, "example text"};
    objectUnderTest.addReceivedSms(sms);

    const auto& smsFromDb = objectUnderTest.getSms(0);
    ASSERT_EQ(smsFromDb, sms);
}

TEST_F(SmsDbTestSuite, shallUpdateSmsState)
{
    objectUnderTest.addReceivedSms(Sms{phoneNumber, "example text"});

    objectUnderTest.updateSmsState(0);
    const auto& messages = objectUnderTest.getSmsMessages();
    const auto& [_, smsState] = messages[0];

    ASSERT_EQ(smsState, SmsState::Viewed);
}


}  // namespace ue