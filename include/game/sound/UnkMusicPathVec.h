#pragma once

// The music manager's file-path list (each music entry holds one at +0x28): 0x18-byte records of a file path id
// and a point, in a base/UnkCopyVec_b56.h array whose out-of-line reserve is fn_7100456980 (in
// CChozoCommanderAIComponent's unit) and whose copy constructor is fn_7100fddf18. Names and layouts are
// placeholders.
#include "base/UnkCopyVec_b56.h"
#include "base/global/UnkPooledId.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPathRecord_b0019 {
    UnkPooledId<&g_unk_7101d47200> id;  // the file path
    UnkVec3f field_0x8;
};
static_assert(sizeof(UnkPathRecord_b0019) == 0x18, "UnkPathRecord_b0019 layout");

using UnkPathVec_b0019 = UnkCopyVec_b56<UnkPathRecord_b0019>;

// Reserves room for `n` records.
void fn_7100456980(UnkPathVec_b0019* self, u32 n);
template <>
inline void UnkPathVec_b0019::Reserve(u32 n) {
    fn_7100456980(this, n);
}

// Record `i`, growing a growable list to cover it first (the new records constructed by fn_7100456ac0).
inline UnkPathRecord_b0019& PathAt(UnkPathVec_b0019& v, u32 i) {
    if (v.field_0x14 && v.count <= i) {
        u32 n = v.capacity;
        if (v.capacity <= i && v.step != 0)
            n = (i / v.step + 1) * v.step;
        fn_7100456980(&v, n);
        fn_7100456980(&v, i + 1);
        for (u32 j = v.count; j < i + 1; ++j)
            fn_7100456ac0(reinterpret_cast<UnkPathRecord*>(&v.data[j]));
        v.count = i + 1;
    }
    return v.data[i];
}
