#include "UserPort.hpp"

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

std::string makeSmsLabel(common::PhoneNumber number, SmsState smsState)
{
    std::stringstream ss;

    if (smsState == SmsState::NotViewed)
    {
        ss << "[New] ";
    }

    ss << static_cast<int>(number.value);
    return ss.str();
}

void UserPort::viewSmsList(const ISmsDb::SmsMessages& smsList)
{
    auto& menu = gui.setListViewMode();
    menu.clearSelectionList();
    for (const auto& [sms, state] : smsList)
    {
        const auto label = makeSmsLabel(sms.from, state);
        menu.addSelectionListItem(label, "");
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
    gui.setViewTextMode().setText(sms.text);

    gui.setAcceptCallback([] {});

    gui.setRejectCallback([this] {
        handler->handleShowSmsList();
    });
}

}  // namespace ue