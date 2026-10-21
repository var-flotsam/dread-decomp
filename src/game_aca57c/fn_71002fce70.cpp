// Destructors of static string-id arrays and records outside this slice (base/UnkStaticStrId.h),
// taken here because they share its template.
#include "base/UnkStaticStrId.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::UnkStaticId;
using base::UnkStaticPathId;

// Nineteen string ids and one more, after 0x30 bytes without a destructor.
struct UnkStaticIds19And1 {
    u8 unk_0x0[0x30];
    UnkStaticId ids[19];
    UnkStaticId last;

    __attribute__((always_inline)) ~UnkStaticIds19And1() {}
};
ARRAY_DTOR(71007fd8b0, UnkStaticIds19And1, 1, g_unk_7172721508)
ARRAY_DTOR(7100536450, UnkStaticId, 3, g_unk_71726e2308)
ARRAY_DTOR(7100a8cc60, UnkStaticId, 8, g_unk_7172751a20)

// A key and a string id.
struct UnkKeyedStaticId {
    u64 key;
    UnkStaticId id;
};
ARRAY_DTOR(7100a8cee0, UnkKeyedStaticId, 8, g_unk_7172751a60)

// A string id and a file path.
struct UnkStaticIdPath {
    UnkStaticId id;
    UnkStaticPathId path;
};
extern UnkStaticIdPath g_unk_7172795f50[3] __attribute__((visibility("hidden")));
// NON_MATCHING(minor): the two pools' registers differ and the original is 40 bytes longer.
void fn_7100c9cb50() { base::DestroyArray<UnkStaticIdPath, 3, g_unk_7172795f50>(); }

// Two string ids 0x20 apart in a 0x30-byte record.
struct UnkStaticIdPair30 {
    UnkStaticId a;
    u8 unk_0x8[0x18];
    UnkStaticId b;
    u8 unk_0x28[0x8];

    __attribute__((always_inline)) ~UnkStaticIdPair30() {}
};
// Seven of them, as separate members (the original's release is not a loop).
struct UnkStaticIdPair30x7 {
    UnkStaticIdPair30 r0, r1, r2, r3, r4, r5, r6;

    __attribute__((always_inline)) ~UnkStaticIdPair30x7() {}
};
ARRAY_DTOR(71002fce70, UnkStaticIdPair30x7, 1, g_unk_71726aef08)
