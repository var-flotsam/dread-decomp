// CMissileMovement's unit (tools/tu_map.py unit 1618): a constructor over the projectile movement
// (fn_7100ac2c5c): three empty string ids, an empty file path, cleared fields, the flag at +0x571
// copied to +0x5f9, and the colour at +0x4d8 set. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CMissileMovement.h"
#include "game/CProjectileMovement.h"
#include "types.h"

using base::color::CColor4F;
using base::global::UnkStringData;

extern void* g_unk_7101a95fc0[];

struct UnkMissileMovement_laba0c0 {
    void** vtable;
    u8 pad_0x8[0x448 - 0x8];
    u32 field_0x448;
    u8 pad_0x44c[0x4d8 - 0x44c];
    CColor4F color;  // 0x4d8
    bool field_0x4e8;
    u8 pad_0x4e9[0x571 - 0x4e9];
    u8 field_0x571;
    u8 pad_0x572[0x5d0 - 0x572];
    bool field_0x5d0;
    u8 pad_0x5d1[3];
    u32 field_0x5d4;
    UnkStringData* ids[3];  // 0x5d8
    u64 field_0x5f0;
    bool field_0x5f8;
    u8 field_0x5f9;
    u8 pad_0x5fa[6];
    UnkStringData* path;  // 0x600
    bool field_0x608;
    u8 pad_0x609[7];
    u64 field_0x610;
    u64 field_0x618;
};
static_assert(__builtin_offsetof(UnkMissileMovement_laba0c0, field_0x618) == 0x618, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

static inline void MakeEmptyPath(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    *id = &g_unk_7101d47200->empty;
}

// The header declares the movement opaquely; its layout as this constructor fills it is above.
void fn_7100aba0c0(UnkMissileMovement* movement) {
    auto* self = reinterpret_cast<UnkMissileMovement_laba0c0*>(movement);
    fn_7100ac2c5c(self);
    self->vtable = g_unk_7101a95fc0 + 2;
    self->field_0x5d0 = false;
    self->field_0x5d4 = 0;
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    self->field_0x5f0 = 0;
    self->field_0x5f8 = false;
    MakeEmptyPath(&self->path);
    self->field_0x5f9 = self->field_0x571;
    self->field_0x608 = false;
    self->field_0x610 = 0;
    self->field_0x618 = 0;
    self->field_0x448 = 8;
    CColor4F color(1.0f, 0.1f, 0.0f, 1.0f);
    self->color = color;
    self->field_0x4e8 = true;
}
