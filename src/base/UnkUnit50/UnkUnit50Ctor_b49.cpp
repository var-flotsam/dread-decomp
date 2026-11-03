// From unit 50 of tools/tu_map.py (the object src/base/UnkUnit50/UnkUnit50Init.cpp sets up): its
// constructor, fn_7100071dac. Everything starts cleared, with the default file-path id
// (g_unk_7101d470e8), the zero vector at +0x48, no count (0x7fffffff), the scales 1 and -1 at
// +0x90, and the rumble manager (created on first use) with the given word. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::input::UnkRumbleManager;

extern UnkRumbleManager* g_unk_7101d47348;

struct UnkUnit50Object_b49a1 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    u32 field_0x38;
    CFilePathStrId path;            // 0x40
    UnkVec3f offset;                // 0x48
    s32 count;                      // 0x54
    UnkRumbleManager* manager;      // 0x58
    u64 field_0x60;
    u64 field_0x68;
    u32 field_0x70;
    u32 field_0x74[6];
    f32 field_0x8c[1];
    f32 scale_0x90;
    f32 scale_0x94;
    u64 field_0x98;
    u32 field_0xa0;

    UnkUnit50Object_b49a1(u32 id);
};
static_assert(__builtin_offsetof(UnkUnit50Object_b49a1, field_0xa0) == 0xa0, "layout");

inline UnkUnit50Object_b49a1::UnkUnit50Object_b49a1(u32 id)
    : field_0x0(0), field_0x8(0), field_0x10(0), field_0x18(0), field_0x20(0), field_0x28(0),
      field_0x30(0), field_0x38(0), path(g_unk_7101d470e8),
      offset(reinterpret_cast<const UnkVec3f&>(g_unk_710164779c)), count(0x7fffffff),
      manager(nullptr), field_0x60(0), field_0x68(0), field_0x70(0xffffffff), field_0x74(),
      field_0x8c(), scale_0x90(1.0f), scale_0x94(-1.0f), field_0x98(0), field_0xa0(0) {
    if (!g_unk_7101d47348) {
        auto* m = static_cast<UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    UnkRumbleManager* m = g_unk_7101d47348;
    field_0x70 = id;
    manager = m;
    field_0x60 = 0;
}

void fn_7100071dac(UnkUnit50Object_b49a1* self, u32 id) {
    new (self) UnkUnit50Object_b49a1(id);
}
