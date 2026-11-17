// Unit 2604: clearing a string-id map (fn_7100f8db84). Names and layouts are placeholders.
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A map entry's value: a second id, two words and a count.
struct UnkMapValue_b0233 {
    CStrId id;     // 0x8
    u64 words[2];  // 0x10
    u32 count;     // 0x20
};

using UnkIdMap_b0233 = base::global::CRntDictionary<CStrId, UnkMapValue_b0233>;

// Frees every entry and empties the buckets.
void fn_7100f8db84(UnkIdMap_b0233* self) { self->Clear(); }
