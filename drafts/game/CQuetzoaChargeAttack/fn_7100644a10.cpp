// CQuetzoaChargeAttack's unit (tools/tu_map.py): whether the charge may start. The owner's AI component
// must be a CQuetzoaAIComponent, the owner must have a target with its object at +0x1b8, and the base check
// (fn_7100360b84) must pass; unless the request names no point (-1, -1), that object's value at +0x68 must
// be at most 0.5. The target must lie between 70 degrees below and 20 degrees above the side the owner
// faces, the path that way must be clear (fn_71006444ac), the AI's handle must hold nothing and the
// navigation must be in mode 3; a CQuetzoaXAIComponent also needs its flag clear and its value above 2.5.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The request the check is asked about.
struct UnkChargeRequest_l644a10 {
    u8 pad_0x0[0x8];
    f32 x;  // 0x8
    f32 y;  // 0xc
};

struct UnkChargeTargetObject_l644a10 {
    u8 pad_0x0[0x68];
    f32 field_0x68;
};

// The part of the Quetzoa AI component the charge reads (at +0x14a20 of it).
struct UnkQuetzoaChargeState_l644a10 {
    UnkWeakHandle* handle;  // 0x0
    u8 pad_0x8[0x1a0];
    bool field_0x1a8;
    u8 pad_0x1a9[3];
    f32 field_0x1ac;
};

struct UnkQuetzoaAI_l644a10 {
    void** vtable;
    u8 pad_0x8[0x14a18];
    UnkQuetzoaChargeState_l644a10 charge;  // 0x14a20

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkQuetzoaAI_l644a10*)>(vtable[0])(this);
    }
};
static_assert(__builtin_offsetof(UnkQuetzoaAI_l644a10, charge) == 0x14a20, "layout");

struct UnkNavKind_l644a10 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkNavEntry_l644a10 {
    u8 pad_0x0[0x10];
    UnkNavKind_l644a10* field_0x10;
};

struct UnkNavRecord_l644a10 {
    UnkNavEntry_l644a10* field_0x0;
    u8 pad_0x8[0x10];
    void* field_0x18;
};

struct UnkNav_l644a10 {
    u8 pad_0x0[0xb0];
    UnkNavRecord_l644a10* record;  // 0xb0
};

bool fn_71006444ac(CAttack* self, const UnkVec3f* from, const UnkVec3f* dir, const UnkVec3f* offset);

static inline UnkVec3f Position(const UnkActor* actor) {
    return UnkVec3f(actor->field_0x44, actor->field_0x54, actor->field_0x64);
}

static inline f32 Sign(f32 x) { return (f32)((x > 0.0f) - (x < 0.0f)); }

bool fn_7100644a10(CAttack* self, void* arg, u32 mode) {
    UnkQuetzoaAI_l644a10* ai = static_cast<UnkQuetzoaAI_l644a10*>(self->field_0x8->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_7100649630()))
        ai = nullptr;
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (!target)
        return false;
    if (!ai)
        return false;
    UnkChargeTargetObject_l644a10* object = static_cast<UnkChargeTargetObject_l644a10*>(target->field_0x1b8);
    if (!object)
        return false;
    if (!fn_7100360b84(self, arg, mode))
        return false;
    UnkChargeRequest_l644a10* request = static_cast<UnkChargeRequest_l644a10*>(arg);
    if (!(request->x == -1.0f && request->y == -1.0f) && object->field_0x68 > 0.5f)
        return false;

    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38));
    UnkVec3f forward(m.m[2], m.m[6], m.m[10]);
    UnkVec3f dir = forward;
    UnkVec3f from = Position(self->field_0x8);
    UnkActor* other = fn_7100d7c990(self->field_0x8);
    if (!other)
        return false;
    f32 side = Sign(dir.x);
    f32 dx = other->field_0x44 - from.x;
    f32 dy = other->field_0x54 - from.y;
    f32 angle = atan2f(dy, dx * side);
    if (!(angle >= -1.2217305f && angle <= 0.34906584f))
        return false;
    dir = UnkVec3f(cosf(angle) * side, sinf(angle), 0.0f);
    UnkVec3f offset(0.0f, 0.0f, 0.0f);
    if (!fn_71006444ac(self, &from, &dir, &offset))
        return false;

    UnkQuetzoaChargeState_l644a10* charge = &ai->charge;
    if (UnkWeakHandle* handle = charge->handle) {
        ++handle->ref_count;
        UnkWeakTarget* held = handle->target;
        if (--handle->ref_count == 0)
            handle->vf_0x10();
        if (held)
            return false;
    }
    if (UnkNav_l644a10* nav = static_cast<UnkNav_l644a10*>(self->field_0x8->field_0x298)) {
        UnkNavRecord_l644a10* record = nav->record;
        if (!record->field_0x0 || !record->field_0x0->field_0x10 || record->field_0x0->field_0x10->kind != 3 ||
            !record->field_0x18)
            return false;
    }
    if (!fn_710009d50c(ai->vf_0x0(), fn_710063efe4()))
        return true;
    if (charge->field_0x1a8)
        return false;
    return charge->field_0x1ac > 2.5f;
}
