// CYamplotXBiteAttack's unit (tools/tu_map.py, unit 1105): runs the AI's path follower's side search
// (fn_710035e8d8) toward a point 450 to the side the movement faces, then names the bite by how far
// along the found point lies ("_h1" to "_h4" appended to a base name). Names and layouts are placeholders.
#include <math.h>

#include <functional>

#include "base/UnkMatrix44.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAIPathSearch.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkAIPathFollower;

// What the side search fills here, as 4-byte words (0x14 bytes).
struct UnkBiteSearchOut_l6ecce4 {
    u32 field_0x0[4];
    f32 distance;

    UnkBiteSearchOut_l6ecce4() : field_0x0(), distance(-1.0f) {}
};

// The bite names' common start.
extern const CRntString g_unk_717270bd60 __attribute__((visibility("hidden")));

// The movement's path state (at +0x8e8 of the movement, kind 1).
struct UnkBitePathState_l6ecce4 {
    u8 pad_0x0[0x89];
    u8 field_0x89;
    u8 pad_0x8a[0x124 - 0x8a];
    u8 field_0x124;
};

struct UnkBiteMovement_l6ecce4 {
    u8 pad_0x0[0x58];
    CVector3D dir;  // 0x58
    u8 pad_0x64[0x8e8 - 0x64];
    UnkBitePathState_l6ecce4 state;  // 0x8e8
};

struct UnkBiteAI_l6ecce4 {
    u8 pad_0x0[0x68];
    u8 follower[0x30];  // 0x68, the path follower
};

struct UnkBiteOwner_l6ecce4 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f0 - 0x78];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x240 - 0x1f8];
    UnkBiteAI_l6ecce4* ai;  // 0x240
};

struct UnkYamplotXBite_b57a4;

struct UnkYamplotXBiteAttack_l6ecce4 {
    u8 pad_0x0[0x8];
    UnkBiteOwner_l6ecce4* owner;  // 0x8
};

// A string built empty out of line (fn_710007e6c4).
struct UnkBiteString_l6ecce4 {
    alignas(8) u8 storage[sizeof(CRntString)];

    UnkBiteString_l6ecce4() { fn_710007e6c4(storage); }
    ~UnkBiteString_l6ecce4() { CRntString_Destroy(Str()); }
    CRntString* Str() { return reinterpret_cast<CRntString*>(storage); }
};

static inline CVector3D Translation(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[3];
    v.y = m.m[7];
    v.z = m.m[11];
    return v;
}

// The third column of a transform: the direction it faces.
static inline CVector3D AxisZ(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[2];
    v.y = m.m[6];
    v.z = m.m[10];
    return v;
}

static inline void Flatten(CVector2D* out, const CVector3D& v) {
    fn_710007b430(out, &v);
}

CStrId CYamplotXBiteAttack_GetDistanceSuffixedName(UnkYamplotXBite_b57a4* attack) {
    UnkYamplotXBiteAttack_l6ecce4* self = reinterpret_cast<UnkYamplotXBiteAttack_l6ecce4*>(attack);
    UnkReflObject* mv = self->owner->movement;
    UnkReflObject* movement_obj = mv && fn_710009d50c(mv->vf_0x0(), fn_71010c74e0()) ? mv : nullptr;
    UnkBiteMovement_l6ecce4* movement = reinterpret_cast<UnkBiteMovement_l6ecce4*>(movement_obj);

    bool blocked;  // unset in the original (the search sets it)
    u32 forward;   // unset in the original (the search sets it)
    UnkAIPathFollower* follower = reinterpret_cast<UnkAIPathFollower*>(self->owner->ai->follower);
    CVector3D target = Translation(self->owner->transform);
    s32 edge_from = -1;
    s32 edge_to = -1;
    f32 distance = -1.0f;
    UnkBiteSearchOut_l6ecce4 out;
    CVector2D point = {0.0f, 0.0f};

    u32 kind = fn_71010cc3a4(movement);
    UnkBitePathState_l6ecce4* state = kind == 1 ? &movement->state : nullptr;
    bool flag = (state->field_0x124 != 0) != (state->field_0x89 != 0);
    target.x = (flag ? 450.0f : -450.0f) + target.x;

    CVector2D from;
    CVector2D to;
    CVector2D dir;
    CVector2D facing;
    {
        CVector3D pos = Translation(self->owner->transform);
        Flatten(&from, pos);
        Flatten(&to, target);
        Flatten(&dir, movement->dir);
        CVector3D axis = AxisZ(fn_710002614c(&self->owner->transform));
        Flatten(&facing, axis);
    }

    fn_710035e8d8(follower, &from, &to, &dir, &facing, flag, &point, &forward, 0.0f, &blocked,
                  reinterpret_cast<f32*>(&edge_from), reinterpret_cast<f32*>(&edge_to),
                  reinterpret_cast<u32*>(&distance),
                  reinterpret_cast<UnkAIPathSearchOut*>(&out), std::function<void()>());

    UnkBiteString_l6ecce4 suffix;
    f32 along = fabsf(point.x - self->owner->transform.m[3]);
    const char* tag;
    if (along < 100.0f)
        tag = "_h1";
    else if (along < 200.0f && along >= 100.0f)
        tag = "_h2";
    else if (along < 300.0f && along >= 200.0f)
        tag = "_h3";
    else
        tag = "_h4";
    CRntString_AssignCString(suffix.Str(), tag);
    CRntString name = CRntString_Concat(&g_unk_717270bd60, suffix.Str());
    return CStrId(name, true);
}
