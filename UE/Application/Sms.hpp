#pragma once

#include <string>
#include <tuple>

#include "Messages/PhoneNumber.hpp"

namespace ue
{
struct Sms
{
    common::PhoneNumber from;
    std::string text;

    Sms(common::PhoneNumber from, std::string text)
        : from{from},
          text{std::move(text)}
    {}

    [[nodiscard]] auto tie() const
    {
        return std::tie(from, text);
    }
};

inline bool operator==(const Sms& lhs, const Sms& rhs)
{
    return lhs.tie() == rhs.tie();
}

}  // namespace ue