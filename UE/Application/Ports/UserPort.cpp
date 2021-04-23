#include "UserPort.hpp"

#include "UeGui/ITextMode.hpp"
#include "UeGui/IListViewMode.hpp"

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
        switch (menu.getCurrentItemIndex().second) {
            case 0:
                break;
            case 1:
                handler->handleShowSmsList();
                break;
        }
    });
}

void UserPort::showNewSmsNotification()
{
    gui.showNewSms();
}

void UserPort::viewSmsList(const SmsMessages& smsList) {
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    for(auto e : smsList){
        std::string label = e.second == SmsState::Viewed ? to_string(e.first.from) : "[New] " +to_string(e.first.from);
        menu.addSelectionListItem(label, "");
    }
    gui.setAcceptCallback([this,&menu]{
        unsigned indexOfSelectedSms = menu.getCurrentItemIndex().second;
        handler->handleShowSms(indexOfSelectedSms);
    });

    gui.setRejectCallback([this]{
        showConnected();
    });
}

void UserPort::viewSms(const Sms & sms) {
    IUeGui::ITextMode& display = gui.setViewTextMode();

    display.setText(sms.text);

    gui.setRejectCallback([this]{
        handler->handleShowSmsList();
    });
}

}  // namespace ue