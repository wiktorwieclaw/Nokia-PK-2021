#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/ITransportMock.hpp"
#include "Ports/BtsPort.hpp"

namespace ue
{
using namespace ::testing;

class BtsPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{13121981LL};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsEventsHandlerMock> handlerMock;
    StrictMock<common::ITransportMock> transportMock;
    common::ITransport::MessageCallback messageCallback;
    common::ITransport::DisconnectedCallback disconnectCallback;

    BtsPort objectUnderTest{loggerMock, transportMock, PHONE_NUMBER};

    BtsPortTestSuite()
    {
        EXPECT_CALL(transportMock, registerMessageCallback(_))
            .WillOnce(SaveArg<0>(&messageCallback));
        EXPECT_CALL(transportMock, registerDisconnectedCallback(_))
            .WillOnce(SaveArg<0>(&disconnectCallback));
        objectUnderTest.start(handlerMock);
    }
    ~BtsPortTestSuite() override
    {
        EXPECT_CALL(transportMock, registerMessageCallback(IsNull()));
        EXPECT_CALL(transportMock, registerDisconnectedCallback(IsNull()));
        objectUnderTest.stop();
    }
};

TEST_F(BtsPortTestSuite, shallRegisterHandlersBetweenStartStop)
{
}

TEST_F(BtsPortTestSuite, shallIgnoreWrongMessage)
{
    common::OutgoingMessage wrongMsg{};
    wrongMsg.writeBtsId(BTS_ID);
    messageCallback(wrongMsg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleDisconnected)
{
    EXPECT_CALL(handlerMock, handleDisconnected());
    disconnectCallback();
}

TEST_F(BtsPortTestSuite, shallHandleSib)
{
    EXPECT_CALL(handlerMock, handleSib(BTS_ID));
    common::OutgoingMessage msg{common::MessageId::Sib,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeBtsId(BTS_ID);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleAttachAccept)
{
    EXPECT_CALL(handlerMock, handleAttachAccept());
    common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(true);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleAttachReject)
{
    EXPECT_CALL(handlerMock, handleAttachReject());
    common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(false);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallSendAttachRequest)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });
    objectUnderTest.sendAttachRequest(BTS_ID);
    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::AttachRequest, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(common::PhoneNumber{}, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(BTS_ID, reader.readBtsId()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallSendSms)
{
    common::BinaryMessage msg;
    const common::PhoneNumber receiverPhoneNumber = common::PhoneNumber{113};
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce([&msg](auto param) { msg = std::move(param); return true; });
    objectUnderTest.sendSms(receiverPhoneNumber,"example");
    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::Sms, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(receiverPhoneNumber, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ("example", reader.readRemainingText()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

}  // namespace ue
