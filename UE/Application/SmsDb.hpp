#pragma once

#include <vector>

#include "ISmsDb.hpp"
#include "Sms.hpp"

namespace ue
{
class SmsDb : public ISmsDb
{
public:
    using SmsMessages = std::vector<std::pair<Sms, SmsState>>;

    void addReceivedSms(const Sms& sms) override;
    [[nodiscard]] const SmsMessages& getSmsMessages() override;
    void updateSmsState(const int) override;
    [[nodiscard]] const Sms& getSms(int) override;

private:
    SmsMessages smsMessages;
};

}  // namespace ue