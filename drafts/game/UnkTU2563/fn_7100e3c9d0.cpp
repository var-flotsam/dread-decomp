// UnkTU2563 (CMinimapData's unit): fills a minimap icon for an actor component: its position (from the
// owner's transform), a box round it, and the icon's name picked by the component's class (recharge
// stations, save stations, elevators, teleporters...). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;

struct UnkVec3 {
    u32 a;
    u32 b;
    u32 c;
};

const CStrId* fn_7100e68f20(u32 index);

extern CStrId g_unk_71727c8238;
extern CStrId g_unk_71727c8240;
extern CStrId g_unk_71727c8248;
extern CStrId g_unk_71727c8250;
extern CStrId g_unk_71727c8258;
extern CStrId g_unk_71727c8260;
extern CStrId g_unk_71727c8270;
extern CStrId g_unk_71727c8280;
extern CStrId g_unk_71727c8290;
extern CStrId g_unk_71727c82a0;
extern CStrId g_unk_71727c82b0;
extern CStrId g_unk_71727c82c0;

struct UnkIconXform_le3c9d0 {
    u8 pad_0x0[0x44];
    u32 x;  // 0x44
    u8 pad_0x48[0x54 - 0x48];
    u32 y;  // 0x54
    u8 pad_0x58[0x64 - 0x58];
    u32 z;  // 0x64
};

struct UnkIconSource_le3c9d0 {
    u8 pad_0x0[0x10];
    UnkIconXform_le3c9d0* xform;  // 0x10
};

struct UnkIconComponent_le3c9d0 {
    u8 pad_0x0[0x18];
    UnkIconXform_le3c9d0* xform;  // 0x18
    u8 pad_0x20[0xe0 - 0x20];
    UnkIconSource_le3c9d0* source;  // 0xe0
    u8 pad_0xe8[0x588 - 0xe8];
    u32 teleporter;  // 0x588
};

struct UnkMinimapIcon_le3c9d0 {
    CVector2D pos;
    CVector2D min;  // 0x8
    CVector2D max;  // 0x10
    CStrId name;    // 0x18
};

template <const base::reflection::CClass* (*GetClass)()>
static inline bool IsA(UnkIconComponent_le3c9d0* self) {
    auto* obj = reinterpret_cast<base::reflection::UnkReflObject*>(self);
    return fn_710009d50c(obj->vf_0x0(), GetClass());
}

void fn_7100e3c9d0(UnkIconComponent_le3c9d0* self, UnkMinimapIcon_le3c9d0* icon) {
    UnkIconXform_le3c9d0* xform;
    if (self->source && self->source->xform)
        xform = self->source->xform;
    else
        xform = self->xform;
    UnkVec3 v = {xform->x, xform->y, xform->z};
    fn_710007b444(icon, &v);
    icon->min.x = icon->pos.x - 150.0f;
    icon->min.y = icon->pos.y;
    icon->max.x = icon->pos.x + 150.0f;
    icon->max.y = icon->pos.y + 300.0f;

    const CStrId* name;
    if (IsA<fn_7100a76ec4>(self)) {
        name = &g_unk_71727c8280;
    } else if (IsA<fn_7100a82b10>(self)) {
        name = &g_unk_71727c8270;
    } else if (IsA<fn_7100a969f4>(self)) {
        name = &g_unk_71727c8290;
    } else if (IsA<fn_7100a896c8>(self)) {
        name = &g_unk_71727c82a0;
    } else if (IsA<fn_7100a849c8>(self)) {
        name = &g_unk_71727c82b0;
    } else if (IsA<fn_7100a6b630>(self)) {
        name = &g_unk_71727c82c0;
    } else if (IsA<fn_7100a77c64>(self)) {
        name = &g_unk_71727c8258;
    } else if (IsA<fn_7100a811f0>(self)) {
        name = &g_unk_71727c8260;
    } else if (IsA<fn_7100aa4b78>(self)) {
        name = &g_unk_71727c8238;
    } else if (IsA<fn_7100a8fbdc>(self)) {
        name = &g_unk_71727c8240;
    } else if (IsA<fn_7100a7bfc4>(self)) {
        name = &g_unk_71727c8248;
    } else if (IsA<fn_7100a98cd0>(self)) {
        name = &g_unk_71727c8250;
    } else if (IsA<fn_7100a802e4>(self)) {
        name = &g_unk_71727c8260;
    } else {
        if (IsA<fn_7100a8d6d4>(self)) {
            // Teleporters are named after their destination and get a taller box.
            icon->name = *fn_7100e68f20(self->teleporter);
            icon->min.x = icon->pos.x - 150.0f;
            icon->min.y = icon->pos.y;
            icon->max.x = icon->pos.x + 150.0f;
            icon->max.y = icon->pos.y + 500.0f;
        }
        return;
    }
    icon->name = *name;
}
