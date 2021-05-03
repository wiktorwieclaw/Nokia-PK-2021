#pragma once

#include <vector>

#include "ISmsDb.hpp"
#include "Sms.hpp"

namespace ue
{
class SmsDb : public ISmsDb
{
public:
    void addMessage(const Sms& sms) override;
    void setMessageState(gsl::index i, SmsState state) override;
    [[nodiscard]] gsl::span<const Sms> getAllMessages() override;
    [[nodiscard]] const Sms& getMessage(gsl::index i) override;
    void markedLastSmsAsFailed() override;

private:
    std::vector<Sms> messages;
};

}  // namespace ue