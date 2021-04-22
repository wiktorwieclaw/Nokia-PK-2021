#include "UserPort.hpp"

#include "UeGui/IListViewMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"

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
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    gui.setAcceptCallback([this,&menu]{
        switch (menu.getCurrentItemIndex().second)
        {
        case 0:
        {
            handler->handleComposeSms();
            break;
        }
        };
    });
}

void UserPort::showNewSmsNotification()
{
    gui.showNewSms();
}

void UserPort::showNewSmsToEdit()
{
    mode = &gui.setSmsComposeMode();
    gui.setAcceptCallback([this]{
        handler->handleSendSms();
    });
}

std::pair<common::PhoneNumber, IUserPort::SmsText> UserPort::getSmsData()
{
    auto phoneNum = mode->getPhoneNumber();
    auto smsText = mode->getSmsText();
    mode->clearSmsText();
    return std::make_pair(
        phoneNum,
        smsText
    );
}

}  // namespace ue