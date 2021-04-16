#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue
{
struct Sms
{
    common::PhoneNumber from;
    std::string text;

    Sms(common::PhoneNumber from, std::string text)
        : from{from},
          text{std::move(text)} {}
};

inline bool operator==(const Sms& lhs, const Sms& rhs)
{
    return lhs.from == rhs.from && lhs.text == rhs.text;
}

class ISmsDb
{
public:
    virtual ~ISmsDb() = default;

    virtual void addReceivedSms(const Sms& sms) = 0;
};

}  // namespace ue