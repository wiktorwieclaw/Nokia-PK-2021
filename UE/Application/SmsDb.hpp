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
    Send
};

class SmsDb : public ISmsDb
{
public:
    using SmsMessages = std::vector<std::pair<Sms, SmsState>>;

    void addReceivedSms(const Sms& sms) override;
    void addSms(const common::PhoneNumber& receiverPhoneNumber,const std::string& text) override;
    [[nodiscard]] const SmsMessages& getSmsMessages() { return smsMessages; }

private:
    SmsMessages smsMessages;
};

}  // namespace ue