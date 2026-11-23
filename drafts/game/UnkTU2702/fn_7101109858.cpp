// CAIComponent unit (tools/tu_map.py unit 2702): picks one of two ids of the reflection id table for the
// side a direction points to: by the sign of its x, or, in mode 2 when the owner's check (fn_71010cc3a4)
// says 1, by its cross product with the owner's transform. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CEnemyMovement.h"
#include "types.h"

using base::global::CStrId;

struct UnkSideOwner_l109858 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x218 - 0x78];
    void* field_0x218;
};
static_assert(__builtin_offsetof(UnkSideOwner_l109858, field_0x218) == 0x218, "layout");

struct UnkSideData_l109858 {
    u8 pad_0x0[0xec];
    u32 mode;  // 0xec
};

struct UnkSideAI_l109858 {
    u8 pad_0x0[0x30];
    UnkSideOwner_l109858* owner;  // 0x30
    u8 pad_0x38[0x1610 - 0x38];
    UnkSideData_l109858* data;  // 0x1610
};
static_assert(__builtin_offsetof(UnkSideAI_l109858, data) == 0x1610, "layout");

static inline const CStrId& Id_l109858(u32 index) {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[index];
}

CStrId fn_7101109858(UnkSideAI_l109858* self, const f32* dir) {
    CStrId side(base::global::UnkStrIdInline(), nullptr, false);
    if (self->data->mode == 2 && fn_71010cc3a4(self->owner->field_0x218) == 1) {
        const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
        f32 ax = m.m[1];
        f32 ay = m.m[5];
        f32 across = ay * dir[0];
        f32 along = ax * dir[1];
        f32 cross = along - across;
        if (!(cross >= 0.0f))
            side = Id_l109858(0x2be);
        else
            side = Id_l109858(0x3be);
    } else {
        side = !(dir[0] <= 0.0f) ? Id_l109858(0x2be) : Id_l109858(0x3be);
    }
    return side;
}
