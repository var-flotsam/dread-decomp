// Unit 271 (base::reflection): copy- and move-assignment (reflection slots 0x48, 0x50) of a cached
// iterator value (base::UnkIterValue, base/UnkCollectionIter.h) with a 64-bit index and a 64-bit
// element (fn_71001f1d38, fn_71001f1db4). Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

// The index: its store may alias the holder's state (the original reloads the state after it).
struct __attribute__((may_alias)) UnkAliasedIndex_b58 {
    u64 value;
};

using UnkU64IterValue_b58 = base::UnkIterValue<u64, UnkAliasedIndex_b58>;

void* fn_71001f1d38(void* dst, const void* src) {
    return base::refl_IterValueCopy<u64, UnkAliasedIndex_b58>(dst, src);
}

UnkU64IterValue_b58* fn_71001f1db4(UnkU64IterValue_b58* self, UnkU64IterValue_b58* other) {
    return &(*self = static_cast<UnkU64IterValue_b58&&>(*other));
}
