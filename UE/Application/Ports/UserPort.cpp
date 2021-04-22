#include "UserPort.hpp"

#include "UeGui/ICallMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"

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
}

void UserPort::showNewSmsNotification()
{
    gui.showNewSms();
}

void UserPort::showCallRequest(common::PhoneNumber from)
{
    auto& mode = gui.setAlertMode();
    mode.setText("Incoming call from: " + std::to_string(from.value));

    gui.setAcceptCallback([this, from] {
        handler->handleCallAccept(from);
    });

    gui.setRejectCallback([this] {
        // todo
    });
}

void UserPort::showTalking()
{
    auto& callMode = gui.setCallMode();
    // todo
}

}  // namespace ue