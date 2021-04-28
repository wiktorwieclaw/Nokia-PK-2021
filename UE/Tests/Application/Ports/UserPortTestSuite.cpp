#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Messages/PhoneNumber.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Ports/UserPort.hpp"

namespace ue
{
using namespace ::testing;

class UserPortTestSuite : public Test
{
protected:

    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IUserEventsHandlerMock> handlerMock;
    StrictMock<IUeGuiMock> guiMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    StrictMock<ITextModeMock> textModeMock;

    UserPort objectUnderTest{loggerMock, guiMock, PHONE_NUMBER};

    UserPortTestSuite()
    {
        EXPECT_CALL(guiMock, setTitle(HasSubstr(to_string(PHONE_NUMBER))));
        objectUnderTest.start(handlerMock);
    }
    ~UserPortTestSuite() override
    {
        objectUnderTest.stop();
    }
};

TEST_F(UserPortTestSuite, shallStartStop)
{
}

TEST_F(UserPortTestSuite, shallShowNotConnected)
{
    EXPECT_CALL(guiMock, showNotConnected());
    objectUnderTest.showNotConnected();
}

TEST_F(UserPortTestSuite, shallShowConnecting)
{
    EXPECT_CALL(guiMock, showConnecting());
    objectUnderTest.showConnecting();
}

TEST_F(UserPortTestSuite, shallShowMenuOnConnected)
{
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));
    EXPECT_CALL(guiMock, setAcceptCallback(_));
    objectUnderTest.showConnected();
}

TEST_F(UserPortTestSuite, shallViewSmsList)
{
    using SmsMessages = std::vector<std::pair<Sms, SmsState>>;
    const SmsMessages smsMessages;

    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(0));
    EXPECT_CALL(guiMock, setAcceptCallback(_));
    EXPECT_CALL(guiMock, setRejectCallback(_));
    objectUnderTest.viewSmsList(smsMessages);
}

TEST_F(UserPortTestSuite, shallViewSms)
{
    const Sms sms{common::PhoneNumber{112},"example"};

    EXPECT_CALL(guiMock, setViewTextMode()).WillOnce(ReturnRef(textModeMock));
    EXPECT_CALL(textModeMock, setText(sms.text));
    EXPECT_CALL(guiMock, setRejectCallback(_));
    objectUnderTest.viewSms(sms);
}



}  // namespace ue
