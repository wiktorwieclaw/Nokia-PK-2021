#include "SmsDb.hpp"

#include "Sms.hpp"

namespace ue
{
void SmsDb::addMessage(const Sms& sms)
{
    messages.push_back(sms);
}

gsl::span<const Sms> SmsDb::getAllMessages()
{
    return messages;
}

void SmsDb::setMessageState(gsl::index i, SmsState state)
{
    messages.at(i).state = state;
}

const Sms& SmsDb::getMessage(gsl::index i)
{
    return messages.at(i);
}

std::size_t SmsDb::getNumberOfMessages()
{
    return messages.size();
}

}  // namespace ue
