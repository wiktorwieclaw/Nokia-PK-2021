#include "SmsDb.hpp"

#include "Sms.hpp"

namespace ue {
    void SmsDb::addReceivedSms(const Sms &sms) {
        smsMessages.emplace_back(sms, SmsState::NotViewed);
    }

    const SmsDb::SmsMessages &SmsDb::getSmsMessages() {
        return smsMessages;
    }

    void SmsDb::updateSmsState(SmsMessages::size_type indexOfSms) {
        smsMessages.at(indexOfSms).second = SmsState::Viewed;
    }

    const Sms &SmsDb::getSms(SmsMessages::size_type indexOfSms) {
        return smsMessages.at(indexOfSms).first;
    }

}  // namespace ue
