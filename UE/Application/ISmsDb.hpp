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
    [[nodiscard]] const virtual SmsMessages& getSmsMessages() = 0;
    virtual void updateSmsState(const int) = 0;
    [[nodiscard]] const virtual Sms& getSms(int) = 0;
};

}  // namespace ue