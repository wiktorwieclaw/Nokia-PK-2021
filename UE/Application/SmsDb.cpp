#include "SmsDb.hpp"

#include "Sms.hpp"

namespace ue
{
void SmsDb::addReceivedSms(const Sms& sms)
{
    smsMessages.emplace_back(sms, SmsState::NotViewed);
}

const SmsDb::SmsMessages& SmsDb::getSmsMessages()
{
    return smsMessages;
}

void SmsDb::updateSmsState(SmsMessages::size_type indexOfSms)
{
    auto& [_, state] = smsMessages[indexOfSms];
    state = SmsState::Viewed;
}

const Sms& SmsDb::getSms(SmsMessages::size_type indexOfSms)
{
    auto& [sms, _] = smsMessages[indexOfSms];
    return sms;
}

}  // namespace ue
