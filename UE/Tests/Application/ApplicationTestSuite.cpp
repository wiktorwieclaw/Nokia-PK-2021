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
    ApplicationConnectedTestSuite();
    void doConnected();
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
    EXPECT_CALL(btsPortMock, sendSms(_));
    EXPECT_CALL(smsDbMock, addSentSms(_));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleSendSms(sms);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleShowSmsList)
{
    gsl::span<const Sms> messages;

    EXPECT_CALL(smsDbMock, getAllMessages()).WillOnce(Return(messages));
    EXPECT_CALL(userPortMock, viewSmsList(messages));

    objectUnderTest.handleShowSmsList();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleShowSms)
{
    const IUeGui::IListViewMode::Selection index = 0;
    Sms sms{PHONE_NUMBER, "example sms message", SmsState::NotViewed};

    EXPECT_CALL(smsDbMock, setMessageState(index, SmsState::Viewed));
    EXPECT_CALL(smsDbMock, getMessage(index)).WillOnce(ReturnRef(sms));
    EXPECT_CALL(userPortMock, viewSms(sms));

    objectUnderTest.handleShowSms(index);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    EXPECT_CALL(userPortMock, showCallRequest(_));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest.handleCallRequest(PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallAccepted)
{
    constexpr common::PhoneNumber to{200};
    EXPECT_CALL(btsPortMock, sendCallAccepted(to));
    EXPECT_CALL(userPortMock, showTalking());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleCallAccept(to);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallDropped)
{
    constexpr common::PhoneNumber to{200};
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallDropped(to));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleCallDrop(to);
}

}  // namespace ue
