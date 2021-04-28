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
    const Sms sms{phoneNumber, "example text"};
    objectUnderTest.addReceivedSms(sms);

    const auto& messages = objectUnderTest.getSmsMessages();
    auto& [_, smsState] = messages[0];
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
    const Sms sms{phoneNumber, "example text"};
    objectUnderTest.addReceivedSms(sms);

    const auto& messagesBeforeUpdate = objectUnderTest.getSmsMessages();
    auto&[sms1, smsStateBeforeUpdate] = messagesBeforeUpdate[0];
    ASSERT_EQ(smsStateBeforeUpdate, SmsState::NotViewed);

    objectUnderTest.updateSmsState(0);

    const auto& messagesAfterUpdate = objectUnderTest.getSmsMessages();
    auto& [sms2, smsStateAfterUpdate] = messagesAfterUpdate[0];
    ASSERT_EQ(smsStateAfterUpdate, SmsState::Viewed);
}

}  // namespace ue