/**
 *
 * @file
 *
 * @brief  DigitalStudio chiptune factory
 *
 * @author vitamin.caig@gmail.com
 *
 **/

#pragma once

// local includes
#include "module/players/dac/dac_factory.h"

namespace Module::DigitalStudio
{
  DAC::Factory::Ptr CreateFactory();
}  // namespace Module::DigitalStudio
