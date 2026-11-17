#pragma once

// One 0xa8-byte tile of a breakable tile group (CBreakableTileGroup's unit, 2213 of tools/tu_map.py), as far
// as the hand-written sources know it: the group keeps an array of them at +0x48 (new[] with the count in
// front). Its constructor (fn_7100c9db18) and destructor (fn_7100c9dd20) are out of line. Names and layouts
// are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkStaticStrId.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

// A static string id and file path (unit 2212's statics; destroyed by fn_7100c9cb50).
struct UnkStaticIdPath {
    base::UnkStaticId id;
    base::UnkStaticPathId path;
};
// The three id and path pairs every tile's slots are set from.
extern UnkStaticIdPath g_unk_7172795f50[3] __attribute__((visibility("hidden")));

// A string id and a file path (both the defaults at first) and the resource they resolve to.
struct UnkBreakableTileSlot {
    base::global::CStrId id;
    base::global::CFilePathStrId path;
    UnkCountedResource* resource;

    UnkBreakableTileSlot() : id(g_unk_7101d470e0), path(g_unk_7101d470e8), resource(nullptr) {}
    // Releases the ids and the resource (fn_7100027044), then drops the slot's own reference.
    ~UnkBreakableTileSlot() {
        fn_7100027044(reinterpret_cast<UnkStrIdPathResource*>(this));
        if (UnkCountedResource* r = resource)
            if (--r->ref_count == 0)
                fn_710001737c(r);
    }
};

// The tile's name: built empty (the string-id constructor with no string: only the pool check and the pool's
// empty string remain), released as a CStrId is.
struct UnkBreakableTileName : UnkStrIdTwoStep {
    UnkBreakableTileName() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkBreakableTile_b49 {
    s32 index;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    UnkBreakableTileName name;  // 0x20
    f32 field_0x28;
    f32 field_0x2c;
    void* field_0x30;
    s32 field_0x38;
    bool field_0x3c;
    f32 field_0x40;
    u32 field_0x44;
    u64 render_object;  // 0x48: the render manager's object, or 0xffffffff
    u64 field_0x50;
    u64 field_0x58;
    UnkBreakableTileSlot slots[3];  // 0x60

    UnkBreakableTile_b49() asm("fn_7100c9db18");
    ~UnkBreakableTile_b49() asm("fn_7100c9dd20");
};
static_assert(sizeof(UnkBreakableTile_b49) == 0xa8, "UnkBreakableTile_b49 placeholder layout");
