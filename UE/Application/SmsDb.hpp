#pragma once

#include <vector>

#include "ISmsDb.hpp"
#include "Sms.hpp"

namespace ue {
    enum class SmsState {
        NotViewed,
        Viewed
    };

    class SmsDb : public ISmsDb {
    public:
        using SmsMessages = std::vector<std::pair<Sms, SmsState>>;

        void addReceivedSms(const Sms &sms) override;

        [[nodiscard]] const SmsMessages &getSmsMessages() override;

        void updateSmsState(SmsMessages::size_type) override;

        [[nodiscard]] const Sms &getSms(SmsMessages::size_type) override;

    private:
        SmsMessages smsMessages;
    };

}  // namespace ue