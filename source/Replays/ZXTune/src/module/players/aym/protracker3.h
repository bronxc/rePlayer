/**
 *
 * @file
 *
 * @brief  ProTracker v3.x chiptune factory
 *
 * @author vitamin.caig@gmail.com
 *
 **/

#pragma once

// library includes
#include <formats/chiptune/aym/protracker3.h>
#include <module/players/factory.h>

namespace Module
{
  namespace ProTracker3
  {
    Factory::Ptr CreateFactory(Formats::Chiptune::ProTracker3::Decoder::Ptr decoder);
  }
}  // namespace Module
