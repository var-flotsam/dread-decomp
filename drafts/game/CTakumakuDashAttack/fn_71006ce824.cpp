// CTakumakuDashAttack's unit (tools/tu_map.py, unit 1075): picks the id the dash plays from the owner's
// Takumaku AI component: by whether its target point lies behind the owner's facing when it has one,
// else by the target's life component, the AI's timer (+0x13d4) and its char class's two flags. The empty
// id when the owner is not a Takumaku. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CLifeComponent.h"
#include "game/UnkActor.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkTakumakuCharClass_ovb0135 {
    u8 pad_0x0[0x640];
    f32 field_0x640;
    bool field_0x644;
    bool field_0x645;
    u8 pad_0x646[6];
    f32 field_0x64c;
    f32 field_0x650;
};

// An optional target point (unaligned inside the AI component).
struct __attribute__((packed)) UnkTakumakuTarget_ovb0135 {
    bool set;
    u8 pad_0x1[2];
    f32 x, y, z;  // 0x3
};

// The Takumaku AI component (CTakumakuAIComponent).
struct UnkTakumakuAI_ovb0135 {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x13d4 - 0x18];
    f32 field_0x13d4;
    u8 pad_0x13d8[0x1610 - 0x13d8];
    UnkReflObject* field_0x1610;
    u8 pad_0x1618[0x14699 - 0x1618];
    UnkTakumakuTarget_ovb0135 target;  // 0x14699
    u8 pad_0x146a8[0x14759 - 0x146a8];
    bool field_0x14759;
};
static_assert(__builtin_offsetof(UnkTakumakuAI_ovb0135, field_0x14759) == 0x14759, "layout");

const CStrId* fn_71006d3e3c(UnkTakumakuAI_ovb0135* ai);
const CStrId* fn_71006d40cc(UnkTakumakuAI_ovb0135* ai);
const CStrId* fn_71006d40d8(UnkTakumakuAI_ovb0135* ai);

// What this unit reads of the target actor: its life component (+0x1c0).
struct UnkTakumakuTargetActor_l6ce824 {
    u8 pad_0x0[0x1c0];
    void* life;  // 0x1c0
};

struct UnkTakumakuDashPick_l6ce824 {
    void** vtable;
    UnkActor* owner;  // 0x8
};

template <const base::reflection::CClass* (*Getter)()>
static inline bool IsA(UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), Getter());
}

const CStrId* fn_71006ce824(UnkTakumakuDashPick_l6ce824* self) {
    if (!self->owner)
        return &g_unk_7101d470e0;
    UnkActor* target = fn_7100d7c990(self->owner);
    void* life = target ? reinterpret_cast<UnkTakumakuTargetActor_l6ce824*>(target)->life : nullptr;
    if (!self->owner)
        return &g_unk_7101d470e0;
    UnkReflObject* ai_obj = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (!ai_obj || !IsA<fn_71006d3ba8>(ai_obj))
        return &g_unk_7101d470e0;
    UnkTakumakuAI_ovb0135* ai = reinterpret_cast<UnkTakumakuAI_ovb0135*>(ai_obj);
    UnkReflObject* cc_obj = ai->char_class;
    if (!cc_obj || !IsA<fn_71006d2174>(cc_obj))
        return &g_unk_7101d470e0;
    UnkTakumakuCharClass_ovb0135* char_class = reinterpret_cast<UnkTakumakuCharClass_ovb0135*>(cc_obj);
    UnkTakumakuTarget_ovb0135* point = &ai->target;
    bool* set = &point->set;
    if (*set) {
        UnkActor* owner = self->owner;
        f32 ox = owner->field_0x44;
        f32 oy = owner->field_0x54;
        f32 oz = owner->field_0x64;
        f32 dx = point->x - ox;
        f32 dy = point->y - oy;
        f32 dz = point->z - oz;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
        if (!(dx * m.m[2] + dy * m.m[6] + dz * m.m[10] >= 0.0f))
            return fn_71006d40d8(ai);
    } else {
        if (life && fn_7101161088(life))
            return fn_71006d3e3c(ai);
        f32 timer = ai->field_0x13d4;
        if (timer < 1.5f && timer > -0.001f && char_class->field_0x644)
            return fn_71006d3e3c(ai);
        if (char_class->field_0x645 && !ai->field_0x14759)
            return fn_71006d3e3c(ai);
    }
    return fn_71006d40cc(ai);
}
