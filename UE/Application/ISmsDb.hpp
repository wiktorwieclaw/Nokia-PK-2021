#pragma once

#include <vector>

namespace ue
{
class Sms;
enum class SmsState;

class ISmsDb
{
public:
    using SmsMessages = std::vector<std::pair<Sms, SmsState>>;

    virtual ~ISmsDb() = default;

    virtual void addReceivedSms(const Sms& sms) = 0;

    [[nodiscard]] virtual const SmsMessages& getSmsMessages() = 0;

    virtual void updateSmsState(SmsMessages::size_type index) = 0;

    [[nodiscard]] virtual const Sms& getSms(SmsMessages::size_type index) = 0;
};

}  // namespace ue