#include "UserPort.hpp"

#include "UeGui/ITextMode.hpp"
#include "UeGui/IListViewMode.hpp"

namespace ue {
    UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
            : logger(logger, "[USER-PORT]"),
              gui(gui),
              phoneNumber(phoneNumber) {}

    void UserPort::start(IUserEventsHandler &handler) {
        this->handler = &handler;
        gui.setTitle("Nokia " + to_string(phoneNumber));
    }

    void UserPort::stop() {
        handler = nullptr;
    }

    void UserPort::showNotConnected() {
        gui.showNotConnected();
    }

    void UserPort::showConnecting() {
        gui.showConnecting();
    }

    void UserPort::showConnected() {
        IUeGui::IListViewMode &menu = gui.setListViewMode();
        menu.clearSelectionList();
        menu.addSelectionListItem("Compose SMS", "");
        menu.addSelectionListItem("View SMS", "");

        gui.setAcceptCallback([this, &menu] {
            switch (menu.getCurrentItemIndex().second) {
                case 0:
                    break;
                case 1:
                    handler->handleShowSmsList();
                    break;
            }
        });
    }

    void UserPort::showNewSmsNotification() {
        gui.showNewSms();
    }

    void UserPort::viewSmsList(const SmsMessages &smsList) {
        auto &menu = gui.setListViewMode();
        menu.clearSelectionList();
        for (const auto&[sms, state] : smsList) {

            std::string label;
            if (state == SmsState::NotViewed) {
                label.append("[New] ");
            }
            label.append(to_string(sms.from));

            menu.addSelectionListItem(label, "");
        }
        gui.setAcceptCallback([this, &menu] {
            auto[isSelected, indexOfSelectedItem] = menu.getCurrentItemIndex();
            handler->handleShowSms(indexOfSelectedItem);
        });

        gui.setRejectCallback([this] {
            showConnected();
        });
    }

    void UserPort::viewSms(const Sms &sms) {
        gui.setViewTextMode().setText(sms.text);

        gui.setRejectCallback([this] {
            handler->handleShowSmsList();
        });
    }

}  // namespace ue