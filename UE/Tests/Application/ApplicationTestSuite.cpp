#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "Application.hpp"
#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/ISmsDbMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Sms.hpp"

namespace ue
{
using namespace ::testing;
using namespace std::chrono_literals;

class ApplicationTestSuite : public Test
{
protected:
    const common::BtsId BTS_ID{42};
    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    StrictMock<ISmsDbMock> smsDbMock;

    Expectation expectShowNotConnected = EXPECT_CALL(userPortMock, showNotConnected());
    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock,
                                smsDbMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{};

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
    ApplicationConnectingTestSuite();
    void doConnecting();
};

ApplicationConnectingTestSuite::ApplicationConnectingTestSuite()
{
    doConnecting();
}

void ApplicationConnectingTestSuite::doConnecting()
{
    EXPECT_CALL(userPortMock, showConnecting());
    EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
    EXPECT_CALL(timerPortMock, startTimer(500ms));
    objectUnderTest.handleSib(BTS_ID);
}

TEST_F(ApplicationNotConnectedTestSuite, shallSendAttachRequestOnSib)
{
    // everything in constructor
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnAttachReject)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnAttachTimeout)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleTimeout();
}

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    static constexpr auto callingNumber = common::PhoneNumber{200};

    ApplicationConnectedTestSuite();
    void doConnected();
    void doTalking();
    void doHandleCallRequest(common::PhoneNumber from);
};

ApplicationConnectedTestSuite::ApplicationConnectedTestSuite()
{
    doConnected();
}

void ApplicationConnectedTestSuite::doConnected()
{
    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleAttachAccept();
}

TEST_F(ApplicationConnectedTestSuite, shallShowConnectedOnAttachAccept)
{
    // see test-suite constructor
}

TEST_F(ApplicationConnectedTestSuite, shallShowNotConnectedOnDisconnectFromBts)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleDisconnected();
}

TEST_F(ApplicationConnectedTestSuite, shallReattach)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleDisconnected();

    doConnecting();
    doConnected();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSms)
{
    Sms sms{PHONE_NUMBER, "example sms message", SmsState::NotViewed};

    EXPECT_CALL(userPortMock, showNewSmsNotification());
    EXPECT_CALL(smsDbMock, addMessage(sms));
    objectUnderTest.handleSms(sms);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleComposeSms)
{
    EXPECT_CALL(userPortMock, showNewSmsToEdit());
    objectUnderTest.handleComposeSms();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSendSms)
{
    const Sms& sms{common::PhoneNumber{113},"example"};
    EXPECT_CALL(btsPortMock, sendSms(sms));
    EXPECT_CALL(smsDbMock, addMessage(sms));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleSendSms(sms);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSmsDrop)
{
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleSmsDrop();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleUnknowedRecipientSms)
{
    EXPECT_CALL(smsDbMock, getNumberOfMessages());
    EXPECT_CALL(smsDbMock, setMessageState(_, SmsState::Failed));
    objectUnderTest.handleUnknownRecipient(common::MessageId::Sms);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleUnknowedRecipientCall)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showPartnerNotAvailable());
    // EXPECT_CALL(userPortMock, showConnected()); // todo see how to expect call in another thread
    objectUnderTest.handleUnknownRecipient(common::MessageId::CallRequest);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleShowSmsList)
{
    gsl::span<const Sms> messages;

    EXPECT_CALL(smsDbMock, getAllMessages()).WillOnce(Return(messages));
    EXPECT_CALL(userPortMock, viewSmsList(messages));

    objectUnderTest.handleShowSmsList();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleShowIncomingSms)
{
    const auto index = IUeGui::IListViewMode::Selection{0};
    Sms sms{PHONE_NUMBER, "example sms message", SmsState::NotViewed};

    EXPECT_CALL(smsDbMock, getMessage(index)).WillOnce(ReturnRef(sms));
    EXPECT_CALL(smsDbMock, setMessageState(index, SmsState::Viewed));
    EXPECT_CALL(userPortMock, viewSms(sms));

    objectUnderTest.handleShowSms(index);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleShowOutgoingSms)
{
    const auto index = IUeGui::IListViewMode::Selection{0};
    Sms sms{PHONE_NUMBER, "example sms message", SmsState::Sent};

    EXPECT_CALL(smsDbMock, getMessage(index)).WillOnce(ReturnRef(sms));
    EXPECT_CALL(userPortMock, viewSms(sms));

    objectUnderTest.handleShowSms(index);
}

void ApplicationConnectedTestSuite::doHandleCallRequest(common::PhoneNumber from)
{
    EXPECT_CALL(userPortMock, showCallRequest(_));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest.handleReceiveCallRequest(from);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    doHandleCallRequest(callingNumber);
}

void ApplicationConnectedTestSuite::doTalking()
{
    doHandleCallRequest(callingNumber);
    EXPECT_CALL(btsPortMock, sendCallAccepted(callingNumber));
    EXPECT_CALL(userPortMock, showTalking());
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(timerPortMock, startTimer(30000ms)); // time not given in specification
    objectUnderTest.handleSendCallAccept();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSendCallAccepted)
{
    doTalking();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSendCallDropped)
{
    doHandleCallRequest(callingNumber);
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallDropped(callingNumber));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleSendCallDrop();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallTimeout)
{
    doHandleCallRequest(callingNumber);
    EXPECT_CALL(btsPortMock, sendCallDropped(callingNumber));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleStartDial)
{
    EXPECT_CALL(userPortMock, showEnterPhoneNumber());
    objectUnderTest.handleStartDial();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSendCallRequest)
{
    constexpr common::PhoneNumber enteredPhoneNumber{200};
    EXPECT_CALL(btsPortMock, sendCallRequest(PHONE_NUMBER,enteredPhoneNumber));
    EXPECT_CALL(timerPortMock, startTimer(60000ms));
    EXPECT_CALL(userPortMock, showDialing(enteredPhoneNumber));
    objectUnderTest.handleSendCallRequest(PHONE_NUMBER, enteredPhoneNumber);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReceiveCallAccepted)
{
    EXPECT_CALL(userPortMock, showTalking());
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(timerPortMock, startTimer(_)); // from talking state constructor
    objectUnderTest.handleReceiveCallAccept(callingNumber);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSendCallResignation)
{
    constexpr common::PhoneNumber correspondent{200};
    EXPECT_CALL(timerPortMock, stopTimer);
    EXPECT_CALL(btsPortMock, sendCallDropped(correspondent));
    EXPECT_CALL(userPortMock, showConnected);
    objectUnderTest.handleSendCallResignation(correspondent);
}


struct ApplicationTalkingTestSuite : ApplicationConnectedTestSuite
{
    ApplicationTalkingTestSuite();
};

ApplicationTalkingTestSuite::ApplicationTalkingTestSuite()
{
    ApplicationConnectedTestSuite::doTalking();
}

TEST_F(ApplicationTalkingTestSuite, ShallHandleUnknownRecipient) {
    EXPECT_CALL(timerPortMock, stopTimer);
    EXPECT_CALL(userPortMock, showPartnerNotAvailable());
    //EXPECT_CALL(userPortMock, showConnected); // todo see how to expect call in another thread
    objectUnderTest.handleUnknownRecipient(common::MessageId::CallTalk);
}

TEST_F(ApplicationTalkingTestSuite, shallHandleSendCallDrop)
{
    EXPECT_CALL(btsPortMock,sendCallDropped(_));
    EXPECT_CALL(userPortMock,showConnected());
    objectUnderTest.handleSendCallDrop();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleReceiveCallDrop)
{
    EXPECT_CALL(userPortMock,showConnected());
    EXPECT_CALL(userPortMock, showCallEndedByPartner());
    objectUnderTest.handleReceiveCallDrop();
}

}  // namespace ue
