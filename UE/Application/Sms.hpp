#pragma once

#include <string>
#include <tuple>

#include "Messages/PhoneNumber.hpp"

namespace ue
{
enum class SmsState
{
    NotViewed,
    Viewed,
    Sent
};

struct Sms
{
    common::PhoneNumber correspondent{};
    std::string text;
    SmsState state{};
};

[[nodiscard]] inline auto tie(const Sms& sms)
{
    return std::tie(sms.correspondent, sms.text, sms.state);
}

inline bool operator==(const Sms& lhs, const Sms& rhs)
{
    return tie(lhs) == tie(rhs);
}

}  // namespace ue