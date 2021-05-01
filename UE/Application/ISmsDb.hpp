#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue
{
class Sms;

class ISmsDb
{
public:
    virtual ~ISmsDb() = default;

    virtual void addReceivedSms(const Sms& sms) = 0;
    virtual void addSentSms(const Sms& sms) = 0;
};

}  // namespace ue