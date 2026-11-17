// CYamplotXAIComponent's unit: fn_71006f23c0 is the base check (fn_71011074b8) narrowed by a probe: when a
// segment from the target down 1500 units hits something, the owner's enemy movement must be in state 1 and
// a side search from it toward the hit must hold. Names and layouts are placeholders.
#include <functional>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkChozoRobotSoldierAI_b66a2;
bool fn_71011074b8(UnkChozoRobotSoldierAI_b66a2* self);

struct UnkAIPathFollower;

// What a side search fills (0x14 bytes).
struct UnkAIPathSearchOut {
    u64 a;
    u64 b;
    f32 distance;

    UnkAIPathSearchOut() : a(0), b(0), distance(-1.0f) {}
};

// The side search from `from` toward `to` (fn_710035e8d8): `blocked` tells whether it stopped on
// a side, `point` where.
bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);

// A counted reference released through the object's slot 0x10.
struct UnkProbeRef_l6f23c0 {
    UnkRefObject* p;

    UnkProbeRef_l6f23c0() : p(nullptr) {}
    ~UnkProbeRef_l6f23c0() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// A collision hit (0x48 bytes): two references, an id, the point, two vectors and flags.
struct UnkProbeHit_l6f23c0 {
    UnkProbeRef_l6f23c0 object;
    UnkProbeRef_l6f23c0 field_0x8;
    s32 id;
    CVector3D point;
    CVector3D field_0x20;
    CVector3D normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkProbeHit_l6f23c0()
        : id(-1), point{0.0f, 0.0f, 0.0f}, field_0x20{0.0f, 0.0f, 0.0f}, normal{0.0f, 0.0f, 0.0f}, field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkProbeHit_l6f23c0) == 0x48, "layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The vtables
// (reached through the GOT) are the callback's and its base's.
struct UnkProbeQuery_l6f23c0 {
    void** vtable;
    UnkProbeHit_l6f23c0 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkProbeQuery_l6f23c0(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkProbeQuery_l6f23c0() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkProbeSegment_l6f23c0 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkProbeSegment_l6f23c0(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkProbeMovement_l6f23c0 {
    u8 pad_0x0[0x58];
    CVector3D field_0x58;
    u8 pad_0x64[0x90d];
    u8 field_0x971;
    u8 pad_0x972[0x2];
    CVector2D field_0x974;
    u8 pad_0x97c[0x90];
    u8 field_0xa0c;
};
static_assert(__builtin_offsetof(UnkProbeMovement_l6f23c0, field_0x974) == 0x974, "layout");
static_assert(__builtin_offsetof(UnkProbeMovement_l6f23c0, field_0xa0c) == 0xa0c, "layout");

struct UnkProbeAI_l6f23c0 {
    u8 pad_0x0[0x68];
    u8 follower[0x8];  // 0x68, the path follower
};

struct UnkProbeActor_l6f23c0 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkProbeAI_l6f23c0* ai;  // 0x240
};

struct UnkProbeState_l6f23c0 {
    u8 pad_0x0[0x6fc];
    u32 field_0x6fc;
};

struct UnkYamplotXAI_l6f23c0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x146f0];
    UnkProbeState_l6f23c0* field_0x14728;
};
static_assert(__builtin_offsetof(UnkYamplotXAI_l6f23c0, field_0x14728) == 0x14728, "layout");

// With the base check passed: whether the ground below the target can be reached (true when the probe
// hits nothing).
static inline bool CanReachBelow(UnkYamplotXAI_l6f23c0* self, bool ok) {
    UnkActor* target = fn_7100d7c990(self->actor);
    CVector2D below_start = {target->field_0x44, target->field_0x54};
    CVector2D below_end = {below_start.x, below_start.y - 1500.0f};
    UnkProbeSegment_l6f23c0 segment(below_start, below_end);
    UnkProbeQuery_l6f23c0 query(CVector3D{self->actor->field_0x44, self->actor->field_0x54, self->actor->field_0x64});
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (fn_7100d5c518(tree, 0x80401, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        UnkProbeActor_l6f23c0* actor = reinterpret_cast<UnkProbeActor_l6f23c0*>(self->actor);
        UnkReflObject* object = actor->movement;
        if (object)
            object = fn_710009d50c(object->vf_0x0(), fn_71010c74e0()) ? object : nullptr;
        if (fn_71010cc3a4(object) != 1)
            return false;
        UnkProbeMovement_l6f23c0* movement = reinterpret_cast<UnkProbeMovement_l6f23c0*>(object);
        UnkAIPathFollower* follower = reinterpret_cast<UnkAIPathFollower*>(
            reinterpret_cast<UnkProbeActor_l6f23c0*>(self->actor)->ai->follower);
        bool flag = (movement->field_0xa0c != 0) ^ (movement->field_0x971 != 0);
        CVector2D point;
        point.x = point.y = 0.0f;
        u32 out7;
        bool blocked;
        s32 index_a = -1;
        s32 index_b = -1;
        f32 out_c = -1.0f;
        UnkAIPathSearchOut out;
        CVector2D to;
        fn_710007b430(&to, &query.hit.point);
        CVector2D dir_a;
        fn_710007b430(&dir_a, &movement->field_0x58);
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
        CVector2D dir_b;
        fn_710007b430(&dir_b, &static_cast<const CVector3D&>(CVector3D{m.m[2], m.m[6], m.m[10]}));
        bool found = fn_710035e8d8(follower, &movement->field_0x974, &to, &dir_a, &dir_b, flag, &point, &out7, 0.0f,
                                   &blocked, reinterpret_cast<f32*>(&index_a), reinterpret_cast<f32*>(&index_b),
                                   reinterpret_cast<u32*>(&out_c), &out, std::function<void()>());
        ok &= found;
    }
    return ok;
}

bool fn_71006f23c0(UnkYamplotXAI_l6f23c0* self) {
    bool ok = fn_71011074b8(reinterpret_cast<UnkChozoRobotSoldierAI_b66a2*>(self));
    if (self->field_0x14728->field_0x6fc == 0 && ok)
        ok = CanReachBelow(self, ok);
    return ok;
}
