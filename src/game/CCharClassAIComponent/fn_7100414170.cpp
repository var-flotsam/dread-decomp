// CCharClassAIComponent's unit: picks one of the component's two ids by where the actor faces. When the
// target exists, the second id is set and the actor faces away from the target, answers the second id and
// clears the flag; else answers the first and sets it. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkYamplotPick_b67a4;

struct UnkPickFacing_l414170 {
    void** vtable;

    UnkVec3f vf_0x370() { return reinterpret_cast<UnkVec3f (*)(UnkPickFacing_l414170*)>(vtable[0x370 / 8])(this); }
};

struct UnkPickVec_l414170 {
    f32 x, y, z;
};

static inline UnkPickVec_l414170 operator-(const UnkPickVec_l414170& a, const UnkPickVec_l414170& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

struct UnkPickActor_l414170 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x218 - 0x78];
    UnkPickFacing_l414170* facing;  // 0x218

    UnkPickVec_l414170 Position() const { return {transform.m[3], transform.m[7], transform.m[11]}; }
};

struct UnkPickAI_l414170 {
    u8 pad_0x0[0x478];
    CStrId id_a;  // 0x478
    CStrId id_b;  // 0x480
};

// The direction the actor faces: its facing object's answer, else its transform's third column.
static inline UnkVec3f Forward(UnkPickActor_l414170* actor) {
    if (actor->facing)
        return actor->facing->vf_0x370();
    const UnkMatrix44& m = fn_710002614c(&actor->transform);
    UnkVec3f forward(m.m[2], m.m[6], m.m[10]);
    return forward;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
const UnkYamplotPick_b67a4* fn_7100414170(void* self_, void* actor_, void* target_, void* out_) {
    UnkPickAI_l414170* self = static_cast<UnkPickAI_l414170*>(self_);
    UnkPickActor_l414170* actor = static_cast<UnkPickActor_l414170*>(actor_);
    UnkPickActor_l414170* target = static_cast<UnkPickActor_l414170*>(target_);
    bool* first = static_cast<bool*>(out_);
    if (!target || !actor || self->id_b.data == &g_unk_7101d47108->empty) {
        *first = true;
        return reinterpret_cast<const UnkYamplotPick_b67a4*>(&self->id_a);
    }
    UnkPickVec_l414170 away = actor->Position() - target->Position();
    UnkVec3f forward = Forward(actor);
    if (away.x * forward.x + away.y * forward.y + away.z * forward.z > 0.0f) {
        *first = false;
        return reinterpret_cast<const UnkYamplotPick_b67a4*>(&self->id_b);
    }
    *first = true;
    return reinterpret_cast<const UnkYamplotPick_b67a4*>(&self->id_a);
}
