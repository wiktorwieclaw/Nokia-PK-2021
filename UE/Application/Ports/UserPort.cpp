#include "UserPort.hpp"

#include "UeGui/ICallMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "Sms.hpp"

namespace ue
{
UserPort::UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler& handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
}

void UserPort::stop()
{
    handler = nullptr;
}

void UserPort::showNotConnected()
{
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    gui.showConnecting();
}

void UserPort::showConnected()
{
    auto& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");

    gui.setAcceptCallback([this, &menu] {
        const auto [isSelected, index] = menu.getCurrentItemIndex();

        if (isSelected)
        {
            switch (index)
            {
            case 0:
                handler->handleComposeSms();
                break;
            case 1:
                handler->handleShowSmsList();
                break;
            default:
                break;
            }
        }
    });
}

void UserPort::showNewSmsNotification()
{
    gui.showNewSms();
}

void UserPort::showNewSmsToEdit()
{
    auto& mode = gui.setSmsComposeMode();
    gui.setAcceptCallback([this, &mode] {
        auto phoneNum = mode.getPhoneNumber();
        auto smsText = mode.getSmsText();
        mode.clearSmsText();
        handler->handleSendSms(Sms{phoneNum, smsText, SmsState::Sent});
    });

    gui.setRejectCallback([this, &mode] {
        mode.clearSmsText();
        handler->handleSmsDrop();
    });
}

void UserPort::showCallRequest(common::PhoneNumber from)
{
    auto& mode = gui.setAlertMode();
    mode.setText("Incoming call from: " + std::to_string(from.value));

    gui.setAcceptCallback([this] {
        handler->handleCallAccept();
    });

    gui.setRejectCallback([this] {
        handler->handleCallDrop();
    });
}

void UserPort::showTalking()
{
    auto& callMode = gui.setCallMode();
    // todo
}

std::string makeSmsLabel(const Sms& sms)
{
    std::stringstream ss;

    switch (sms.state)
    {
    case SmsState::NotViewed:
        ss << "[New] [From]: ";
        break;
    case SmsState::Viewed:
        ss << "[From]: ";
        break;
    case SmsState::Sent:
        ss << "[To]: ";
        break;
    }

    ss << static_cast<int>(sms.correspondent.value);
    return ss.str();
}

void UserPort::viewSmsList(gsl::span<const Sms> smsList)
{
    auto& menu = gui.setListViewMode();
    menu.clearSelectionList();

    for (const auto& sms : smsList)
    {
        menu.addSelectionListItem(makeSmsLabel(sms), "");
    }

    gui.setAcceptCallback([this, &menu] {
        const auto [isSelected, index] = menu.getCurrentItemIndex();

        if (isSelected)
        {
            handler->handleShowSms(index);
        }
    });

    gui.setRejectCallback([this] {
        showConnected();
    });
}

void UserPort::viewSms(const Sms& sms)
{
    auto& mode = gui.setViewTextMode();
    mode.setText(sms.text);

    gui.setAcceptCallback([] {});

    gui.setRejectCallback([this, &mode] {
        mode.setText("");
        handler->handleShowSmsList();
    });
}

void UserPort::showPartnerNotAvailable()
{
    auto& alertMode = gui.setAlertMode();
    alertMode.setText("Partner not available");
}

}  // namespace ue