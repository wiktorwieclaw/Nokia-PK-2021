#pragma once

#include <vector>
namespace ue
{
    enum class SmsState
    {
        NotViewed,
        Viewed
    };

class Sms;

class ISmsDb
{
public:
    using SmsMessages = std::vector<std::pair<Sms, SmsState>>;

    virtual ~ISmsDb() = default;

    virtual void addReceivedSms(const Sms& sms) = 0;
    [[nodiscard]] virtual const SmsMessages& getSmsMessages() = 0;
    virtual void updateSmsState(const unsigned int) = 0;
    [[nodiscard]] virtual const Sms &getSms(const unsigned) = 0;
};

}  // namespace ue