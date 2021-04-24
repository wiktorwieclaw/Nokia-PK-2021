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
    void updateSmsState(const unsigned int) override;
    [[nodiscard]] const Sms &getSms(const unsigned) override;

private:
    SmsMessages smsMessages;
};

}  // namespace ue