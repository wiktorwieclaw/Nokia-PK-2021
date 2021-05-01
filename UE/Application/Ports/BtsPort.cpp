#include "BtsPort.hpp"

#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"
#include "Sms.hpp"

namespace ue
{
BtsPort::BtsPort(common::ILogger& logger, common::ITransport& transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
      transport(transport),
      phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler& handler)
{
    transport.registerMessageCallback([this](BinaryMessage msg) { handleMessage(msg); });
    transport.registerDisconnectedCallback([this]() { handleDisconnected(); });
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleDisconnected()
{
    handler->handleDisconnected();
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (msgId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0U;
            if (accept)
            {
                handler->handleAttachAccept();
            }
            else
            {
                handler->handleAttachReject();
            }
            break;
        }
        case common::MessageId::Sms:
        {
            const auto sms = Sms{from, reader.readRemainingText()};
            handler->handleSms(sms);
        }
        default:
            logger.logError("unknown message: ", msgId, ", from: ", from);
        }
    }
    catch (std::exception const& ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}

void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendSms(const Sms& sms)
{
    common::OutgoingMessage msg{common::MessageId::Sms, phoneNumber, sms.correspondent};
    msg.writeText(sms.text);
    transport.sendMessage(msg.getMessage());
}

}  // namespace ue
