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
    Sms sms{PHONE_NUMBER, "example sms message"};

    EXPECT_CALL(userPortMock, showNewSmsNotification());
    EXPECT_CALL(smsDbMock, addReceivedSms(sms));
    objectUnderTest.handleSms(sms);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleShowSmsList)
{
    ISmsDb::SmsMessages messages;

    EXPECT_CALL(smsDbMock, getSmsMessages()).WillOnce(ReturnRef(messages));
    EXPECT_CALL(userPortMock, viewSmsList(messages));

    objectUnderTest.handleShowSmsList();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleShowSms)
{
    const IUeGui::IListViewMode::Selection indexOfSms = 0;
    Sms sms{PHONE_NUMBER, "example sms message"};

    EXPECT_CALL(smsDbMock, updateSmsState(_));
    EXPECT_CALL(smsDbMock, getSms(_)).WillOnce(ReturnRef(sms));
    EXPECT_CALL(userPortMock, viewSms(sms));

    objectUnderTest.handleShowSms(indexOfSms);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    EXPECT_CALL(userPortMock, showCallRequest(_));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest.handleCallRequest(PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallAccepted)
{
    EXPECT_CALL(btsPortMock, sendCallAccepted(_));
    EXPECT_CALL(userPortMock, showTalking());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleCallAccept(common::PhoneNumber{});
}

}  // namespace ue
