#pragma once

namespace ue
{
class Sms;

class ISmsDb
{
public:
    virtual ~ISmsDb() = default;

    virtual void addReceivedSms(const Sms& sms) = 0;
};

}  // namespace ue