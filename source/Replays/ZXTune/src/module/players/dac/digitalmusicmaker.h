/**
 *
 * @file
 *
 * @brief  DigitalMusicMaker chiptune factory
 *
 * @author vitamin.caig@gmail.com
 *
 **/

#pragma once

// local includes
#include "module/players/dac/dac_factory.h"

namespace Module::DigitalMusicMaker
{
  DAC::Factory::Ptr CreateFactory();
}  // namespace Module::DigitalMusicMaker
