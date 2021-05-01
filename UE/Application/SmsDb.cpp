#include "SmsDb.hpp"

#include "Sms.hpp"

namespace ue
{
void SmsDb::addReceivedSms(const Sms& sms)
{
    smsMessages.emplace_back(sms, SmsState::NotViewed);
}

void SmsDb::addSentSms(const Sms& sms)
{
    smsMessages.emplace_back(sms, SmsState::Sent);
}

}  // namespace ue
