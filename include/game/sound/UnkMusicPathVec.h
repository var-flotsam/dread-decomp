#pragma once

// The music manager's file-path list (each music entry holds one at +0x28): 0x18-byte records of a file path id
// and a point, in a base/UnkCopyVec_b56.h array whose out-of-line reserve is fn_7100456980 (in
// CChozoCommanderAIComponent's unit). A music entry holds it as a UnkMusicPathList, which adds the out-of-line
// copy constructor fn_7100fddf18 and frees the records on destruction. Names and layouts are placeholders.
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

// The list as a music entry's member (fn_7100fddcb4 copies the entries).
struct UnkMusicPathList : UnkPathVec_b0019 {
    UnkMusicPathList(const UnkMusicPathList& other) asm("fn_7100fddf18");  // fn_7100fddf18
    ~UnkMusicPathList() { Free(); }
};

// Record `i`, growing a growable list to cover it first (the new records constructed by fn_7100456ac0).
inline UnkPathRecord_b0019& PathAt(UnkPathVec_b0019& v, u32 i) {
    if (v.field_0x14 && v.count <= i) {
        u32 n = v.capacity;
        if (v.capacity <= i && v.step != 0)
            n = (i / v.step + 1) * v.step;
        fn_7100456980(&v, n);
        fn_7100456980(&v, i + 1);
        for (u32 j = v.count; j < i + 1; ++j)
            CChozoCommanderAIComponent_ConstructFilePathRecord(reinterpret_cast<UnkPathRecord*>(&v.data[j]));
        v.count = i + 1;
    }
    return v.data[i];
}
