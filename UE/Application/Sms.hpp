#pragma once

#include "Messages/PhoneNumber.hpp"

struct Sms
{
  common::PhoneNumber from;
  std::string text;

  Sms(common::PhoneNumber from, std::string text)
      : from{from}, text{std::move(text)} {}
};