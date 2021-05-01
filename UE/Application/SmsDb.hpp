#pragma once

#include <vector>

#include "ISmsDb.hpp"
#include "Sms.hpp"

namespace ue
{
enum class SmsState
{
    NotViewed,
    Viewed,
    Sent
};

class SmsDb : public ISmsDb
{
public:
    using SmsMessages = std::vector<std::pair<Sms, SmsState>>;

    void addReceivedSms(const Sms& sms) override;
    void addSentSms(const Sms& sms) override;
    [[nodiscard]] const SmsMessages& getSmsMessages() { return smsMessages; }

private:
    SmsMessages smsMessages;
};

}  // namespace ue