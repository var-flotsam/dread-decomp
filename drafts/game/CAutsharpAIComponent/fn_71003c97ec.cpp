// CAutsharpAIComponent unit (tools/tu_map.py unit 572): fn_71003c97ec, while moving along its path, takes
// hold when its target is near, level with it and not playing the release animation, and the side search
// toward the target ends within 5 of the path's end: it sets the hold flag and switches the collider.
// Names and layouts are placeholders.
#include <functional>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkSegment2D.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU1501.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

void fn_7100863730(void* collision, const CStrId* name, const u32* flags);

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726bee48;  // the collider to switch to

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

struct UnkAutsharpPathPoint_l3c97ec {
    CVector2D pos;
    f32 field_0x8;
};

// The path the component follows (+0x68).
struct UnkAutsharpPath_l3c97ec {
    UnkAutsharpPathPoint_l3c97ec* points;
    u32 count;  // 0x8
    u8 pad_0xc[0x20];
    bool closed;  // 0x2c
};

struct UnkAutsharpMovement_l3c97ec {
    u8 pad_0x0[0x58];
    CVector3D heading;  // 0x58
    u8 pad_0x64[0x90d];
    bool field_0x971;
    u8 pad_0x972[0x9a];
    bool field_0xa0c;
};

// The owner as this unit reads it (UnkActor, with the collision container at +0x2a0).
struct UnkAutsharpOwner_l3c97ec {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38, the transform
    u8 pad_0x78[0x1a0];
    UnkAutsharpMovement_l3c97ec* movement;  // 0x218
    u8 pad_0x220[0x80];
    void* collision;  // 0x2a0
    u8 pad_0x2a8[0xd4];
    f32 radius;  // 0x37c
};
static_assert(__builtin_offsetof(UnkAutsharpOwner_l3c97ec, radius) == 0x37c, "owner");

struct UnkAutsharpAI_l3c97ec {
    u8 pad_0x0[0x30];
    UnkAutsharpOwner_l3c97ec* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkAutsharpPath_l3c97ec path;  // 0x68
    u8 pad_0x98[0x14670];
    bool reversed;  // 0x14708
    u8 pad_0x14709[0xa0];
    bool holding;  // 0x147a9
};
static_assert(__builtin_offsetof(UnkAutsharpAI_l3c97ec, reversed) == 0x14708, "layout");
static_assert(__builtin_offsetof(UnkAutsharpAI_l3c97ec, holding) == 0x147a9, "layout");

static inline CVector3D Pos(const UnkAutsharpOwner_l3c97ec* a) {
    return {a->xform.m[3], a->xform.m[7], a->xform.m[11]};
}

static inline CVector3D Pos(const UnkActor* a) {
    return {a->field_0x44, a->field_0x54, a->field_0x64};
}

static inline f32 X(const UnkAutsharpOwner_l3c97ec* a) { return a->xform.m[3]; }
static inline f32 Y(const UnkAutsharpOwner_l3c97ec* a) { return a->xform.m[7]; }
static inline f32 Z(const UnkAutsharpOwner_l3c97ec* a) { return a->xform.m[11]; }

// Whether a is within eps of b.
inline bool IsNear(f32 a, f32 b, f32 eps) {
    return a - eps <= b && a + eps >= b;
}

void fn_71003c97ec(UnkAutsharpAI_l3c97ec* self) {
    UnkAutsharpMovement_l3c97ec* movement = self->owner->movement;
    if (fn_71010cc3a4(movement) != 1)
        return;
    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    if (DistanceSq(Pos(self->owner), Pos(target)) > 22500.0f)
        return;
    if (!target->field_0x208 || !fn_71009e12d0(target->field_0x208))
        return;
    auto* anim = static_cast<UnkAnimationComponent*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x220);
    if (anim && fn_7101177ae8(anim, 0xf4061ff25a2163d6)->data->Key() == 0x34fb0ed603175553)
        return;
    UnkAutsharpOwner_l3c97ec* owner = self->owner;
    f32 y = Y(owner);
    if (target->field_0x54 > y)
        return;
    f32 reach = owner->radius + target->field_0x37c + 20.0f;
    if (!(target->field_0x54 - reach <= y && target->field_0x54 + reach >= y))
        return;

    UnkAutsharpPath_l3c97ec& path = self->path;
    bool reverse = (movement->field_0xa0c != 0) ^ (movement->field_0x971 != 0);
    CVector2D point = {0.0f, 0.0f};
    u32 out7;
    bool blocked;
    u32 out_a = 0xffffffff;
    u32 out_b = 0xffffffff;
    f32 out_c = -1.0f;
    UnkAIPathSearchOut out;
    CVector2D from;
    {
        CVector3D pos = {X(owner), y, Z(owner)};
        fn_710007b430(&from, &pos);
    }
    CVector2D to;
    {
        CVector3D at = Pos(target);
        fn_710007b430(&to, &at);
    }
    CVector2D heading;
    fn_710007b430(&heading, &movement->heading);
    const UnkMatrix44& m = fn_710002614c(&self->owner->xform);
    CVector2D facing;
    {
        CVector3D dir = {m.m[2], m.m[6], m.m[10]};
        fn_710007b430(&facing, &dir);
    }
    fn_710035e8d8(reinterpret_cast<UnkAIPathFollower*>(&path), &from, &to, &heading, &facing, reverse, &point,
                  &out7, 0.0f, &blocked, reinterpret_cast<f32*>(&out_a), reinterpret_cast<f32*>(&out_b),
                  reinterpret_cast<u32*>(&out_c), &out, std::function<void()>());

    // The path's end in the direction it is walked.
    CVector3D last = {0.0f, 0.0f, 0.0f};
    if (self->reversed) {
        bool open = path.count != 0 && !path.closed;
        u32 n = path.count - open;
        UnkSegment2D seg(path.points[n - 1].pos, open ? path.points[n].pos : path.points[0].pos);
        last.x = seg.end.x;
        last.y = seg.end.y;
    } else {
        UnkSegment2D seg(path.points[0].pos, path.count == 1 ? path.points[0].pos : path.points[1].pos);
        last.x = seg.start.x;
        last.y = seg.start.y;
    }
    last.z = 0.0f;
    CVector2D end;
    fn_710007b430(&end, &last);
    if (IsNear(point.x, end.x, 5.0f) && IsNear(point.y, end.y, 5.0f)) {
        if (void* collision = self->owner->collision) {
            self->holding = true;
            u32 flags = 0x100;
            fn_7100863730(collision, &g_unk_71726bee48, &flags);
        }
    }
}
