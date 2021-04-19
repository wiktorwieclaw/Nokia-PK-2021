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
    auto [_, smsState] = messages[0];
    ASSERT_EQ(smsState, SmsState::NotViewed);
}

TEST_F(SmsDbTestSuite, shallAddSms)
{
    const common::PhoneNumber receiverPhoneNumber = common::PhoneNumber{113};
    objectUnderTest.addSms(receiverPhoneNumber,"example");

    const auto& messages = objectUnderTest.getSmsMessages();
    auto [sms,smsState] = messages[0];
    ASSERT_EQ(sms.text,"example");
    ASSERT_EQ(smsState,SmsState::Send);
}

}  // namespace ue