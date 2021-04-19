#include "SmsDb.hpp"

#include "Sms.hpp"

namespace ue
{
void SmsDb::addReceivedSms(const Sms& sms)
{
    smsMessages.emplace_back(sms, SmsState::NotViewed);
}

void SmsDb::addSms(const common::PhoneNumber& receiverPhoneNumber, const std::string& text)
{
    smsMessages.emplace_back(Sms{receiverPhoneNumber, text}, SmsState::Send);
}

}  // namespace ue
