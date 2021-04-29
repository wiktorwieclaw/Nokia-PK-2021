#pragma once

#include <vector>
#include <gsl/span>
#include <gsl/util>

namespace ue
{
class Sms;
enum class SmsState;

class ISmsDb
{
public:
    virtual ~ISmsDb() = default;
    virtual void addMessage(const Sms& sms) = 0;
    virtual void setMessageState(gsl::index i, SmsState state) = 0;
    [[nodiscard]] virtual gsl::span<const Sms> getAllMessages() = 0;
    [[nodiscard]] virtual const Sms& getMessage(gsl::index i) = 0;
};

}  // namespace ue