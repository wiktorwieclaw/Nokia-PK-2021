#pragma once

#include <vector>

#include "ISmsDb.hpp"
#include "Sms.hpp"

namespace ue
{
enum class SmsState
{
    NotViewed,
    Viewed
};

class SmsDb : public ISmsDb
{
public:
    void addReceivedSms(const Sms& sms) override;

    [[nodiscard]] const SmsMessages& getSmsMessages() override;

    void updateSmsState(SmsMessages::size_type index) override;

    [[nodiscard]] const Sms& getSms(SmsMessages::size_type index) override;

private:
    SmsMessages smsMessages;
};

}  // namespace ue