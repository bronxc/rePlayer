#pragma once

#include <assert.h>
#include <stdint.h>

#ifndef NOMINMAX
#   define NOMINMAX
#endif

namespace core
{
    // basic enum to force definition of some functions (see SmartPtr(AutoAllocate...))
    enum AutoAllocate
    {
        kAllocate
    };

    // return codes (< kOk as error, > kOk as warning)
    enum class Status
    {
        kFail = -1,
        kOk = 0
    };
}
// namespace core