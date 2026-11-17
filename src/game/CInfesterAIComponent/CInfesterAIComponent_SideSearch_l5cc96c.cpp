// CInfesterAIComponent's unit (tools/tu_map.py, unit 892): fn_71005cc96c runs the AI's path follower's
// side search (fn_710035e8d8) from the movement's position toward the actor's target and returns the
// point of the path it finds. Names and layouts are placeholders.
#include <functional>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/CInfesterAIComponent.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkAIPathFollower;

// What a side search fills (0x14 bytes).
struct UnkAIPathSearchOut {
    u64 a;
    u64 b;
    f32 distance;

    UnkAIPathSearchOut() : a(0), b(0), distance(-1.0f) {}
};

bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);

struct UnkVec2f_A5 {
    f32 x, y;
    UnkVec2f_A5(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2f_A5(const UnkVec2f_A5& o) : x(o.x), y(o.y) {}
};

// The movement's path state (at +0x8e8 of the movement, kind 1).
struct UnkInfesterPathState_l5cc96c {
    u8 pad_0x0[0x89];
    bool flag;      // 0x89
    u8 pad_0x8a[2];
    CVector2D pos;  // 0x8c
};

struct UnkInfesterMovement_l5cc96c {
    u8 pad_0x0[0x58];
    CVector3D dir;  // 0x58
    u8 pad_0x64[0x8e8 - 0x64];
    UnkInfesterPathState_l5cc96c state;  // 0x8e8
};

struct UnkInfesterAI_l5cc96c {
    u8 pad_0x0[0x68];
    u8 follower[0x30];  // 0x68, the path follower
};

struct UnkInfesterOwner_l5cc96c {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f0 - 0x78];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkReflObject* ai;  // 0x240
};

static inline CVector3D UnkTranslation(const UnkActor* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

// The third column of a transform: the direction it faces.
static inline CVector3D UnkAxisZ(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[2];
    v.y = m.m[6];
    v.z = m.m[10];
    return v;
}

struct UnkInfesterSelf_l5cc96c {
    u8 pad_0x0[0x30];
    UnkInfesterOwner_l5cc96c* owner;  // 0x30
};

UnkVec2f_A5 fn_71005cc96c(UnkInfesterSelf_l5cc96c* self, u32* forward) {
    UnkReflObject* mv = self->owner->movement;
    UnkReflObject* movement_obj = mv && fn_710009d50c(mv->vf_0x0(), fn_71010c74e0()) ? mv : nullptr;
    UnkInfesterMovement_l5cc96c* movement = reinterpret_cast<UnkInfesterMovement_l5cc96c*>(movement_obj);
    u32 kind = fn_71010cc3a4(movement);
    UnkInfesterPathState_l5cc96c* state = kind == 1 ? &movement->state : nullptr;
    if (!movement || kind != 1)
        return UnkVec2f_A5(0.0f, 0.0f);

    UnkReflObject* o = self->owner->ai;
    UnkReflObject* ai = nullptr;
    if (o) {
        const CType* type = o->vf_0x0();
        fn_71005ca678();
        ai = fn_710009d50c(type, &g_unk_71726ec448) ? o : nullptr;
    }
    UnkAIPathFollower* follower =
        reinterpret_cast<UnkAIPathFollower*>(reinterpret_cast<UnkInfesterAI_l5cc96c*>(ai)->follower);

    bool flag = state->flag;
    UnkVec2f_A5 point(0.0f, 0.0f);
    bool blocked;
    s32 edge_from = -1;
    s32 edge_to = -1;
    f32 distance = -1.0f;
    const CVector2D* from = &state->pos;
    UnkAIPathSearchOut out;

    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    CVector2D to;
    CVector3D target_pos = UnkTranslation(target);
    fn_710007b430(&to, &target_pos);
    CVector2D dir;
    fn_710007b430(&dir, &movement->dir);
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    CVector2D facing;
    CVector3D axis = UnkAxisZ(m);
    fn_710007b430(&facing, &axis);

    fn_710035e8d8(follower, from, &to, &dir, &facing, flag, reinterpret_cast<CVector2D*>(&point),
                  forward, 0.0f, &blocked, reinterpret_cast<f32*>(&edge_from), reinterpret_cast<f32*>(&edge_to),
                  reinterpret_cast<u32*>(&distance), &out, std::function<void()>());
    return point;
}
