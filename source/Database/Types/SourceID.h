#pragma once

#include <Core.h>

namespace rePlayer
{
    struct SourceID
    {
        enum eSourceID : uint32_t
        {
            AmigaMusicPreservationSourceID = 0,
            TheModArchiveSourceID = 1,
            ModlandSourceID = 2,
            FileImportID = 3,
            HighVoltageSIDCollectionID = 4,

            NumSourceIDs
        };

        SourceID() = default;
        constexpr SourceID(eSourceID newSourceId, uint32_t newInternalId) : sourceId(newSourceId), internalId(newInternalId) {}

        constexpr bool operator==(SourceID other) const { return sourceId == other.sourceId && internalId == other.internalId; }

        union
        {
            uint32_t value = 0;
            struct
            {
                eSourceID sourceId : 8;
                uint32_t internalId : 24;
            };
        };

        static const char* const sourceNames[NumSourceIDs];
    };

    static constexpr SourceID kInvalidSourceID = { SourceID::AmigaMusicPreservationSourceID, 0 };
}
// namespace rePlayer