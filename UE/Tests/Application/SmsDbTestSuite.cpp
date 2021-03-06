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
    objectUnderTest.addMessage(Sms{phoneNumber, "example text"});

    const auto& sms = objectUnderTest.getMessage(0);
    ASSERT_EQ(sms.state, SmsState::NotViewed);
}

TEST_F(SmsDbTestSuite, shallgetSms)
{
    const Sms sms{phoneNumber, "example text"};
    objectUnderTest.addMessage(sms);

    const auto& smsFromDb = objectUnderTest.getMessage(0);
    ASSERT_EQ(smsFromDb, sms);
}

TEST_F(SmsDbTestSuite, shallUpdateSmsState)
{
    objectUnderTest.addMessage(Sms{phoneNumber, "example text"});

    objectUnderTest.setMessageState(0, SmsState::Viewed);
    const auto& sms = objectUnderTest.getMessage(0);

    ASSERT_EQ(sms.state, SmsState::Viewed);
}

TEST_F(SmsDbTestSuite, shallAddSentSms)
{
    const auto receiverPhoneNumber = common::PhoneNumber{113};
    objectUnderTest.addMessage(Sms{receiverPhoneNumber, "example", SmsState::Sent});

    const auto& sms = objectUnderTest.getMessage(0);
    ASSERT_EQ(sms.text, "example");
    ASSERT_EQ(sms.state, SmsState::Sent);
}

TEST_F(SmsDbTestSuite, shallGetNumberOfMessages)
{
    objectUnderTest.addMessage(Sms{phoneNumber, "example text",SmsState::Sent});
    objectUnderTest.addMessage(Sms{phoneNumber, "example text",SmsState::Sent});

    ASSERT_EQ(objectUnderTest.getNumberOfMessages(),2);
}

}  // namespace ue