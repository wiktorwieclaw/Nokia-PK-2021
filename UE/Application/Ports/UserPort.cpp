#include "UserPort.hpp"

#include "Sms.hpp"
#include "UeGui/ICallMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/IDialMode.hpp"

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
    menu.addSelectionListItem("Dial", "");

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
            case 2:
                handler->handleStartDial();
                break;
            default:
                break;
            }
        }
    });

    gui.setRejectCallback([] {
        //this should be empty
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
        handler->handleSendCallAccept();
    });

    gui.setRejectCallback([this] {
        handler->handleSendCallDrop();
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

void UserPort::showEnterPhoneNumber()
{
    auto& dialView = gui.setDialMode();
    gui.setAcceptCallback([this,&dialView] {
      PhoneNumber enteredNumber{dialView.getPhoneNumber()};
      handler->handleSendCallRequest(this->phoneNumber,enteredNumber);
    });

    gui.setRejectCallback([this] {
      showConnected();
    });
}

void UserPort::showDialing()
{
    gui.setViewTextMode().setText("Dialling...");
}

void UserPort::showPartnerNotAvailable()
{
    auto& alertMode = gui.setAlertMode();
    alertMode.setText("Partner not available");
}

void UserPort::showPartnerDroppedCall()
{
    auto& alertMode = gui.setAlertMode();
    alertMode.setText("Call Ended");
    gui.setRejectCallback([this] {
      showConnected();
    });
}

}  // namespace ue