/**
 *
 * @file
 *
 * @brief  Simple parser for <prefix><index> pairs implementation
 *
 * @author vitamin.caig@gmail.com
 *
 **/

// library includes
#include <strings/casing.h>
#include <strings/conversion.h>
#include <strings/prefixed_index.h>

namespace Strings
{
  PrefixedIndex PrefixedIndex::Parse(StringView prefix, StringView value)
  {
    std::size_t index = 0;
    const auto valid = 0 == value.compare(0, prefix.size(), prefix)
                       && Strings::Parse(value.substr(prefix.size()), index);
    return PrefixedIndex(value.to_string(), valid, index);
  }

  PrefixedIndex PrefixedIndex::ParseNoCase(StringView prefix, StringView value)
  {
    std::size_t index = 0;
    const auto valid = EqualNoCaseAscii(value.substr(0, prefix.size()), prefix)
                       && Strings::Parse(value.substr(prefix.size()), index);
    return PrefixedIndex(value.to_string(), valid, index);
  }

  PrefixedIndex PrefixedIndex::Create(StringView prefix, std::size_t index)
  {
    return PrefixedIndex(prefix.to_string() + ConvertFrom(index), true, index);
  }
}  // namespace Strings