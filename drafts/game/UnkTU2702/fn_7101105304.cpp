// Unit 2702 (hint CAIComponent): whether an AI component may fire an event: a chance roll, a counter
// window, and optional tests on the side, life, distances and path of the owner and its target.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CLifeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

extern "C" int rand();

struct UnkOwnerParams_l105304 {
    u8 pad_0x0[0x58];
    f32 direction;  // 0x58
};

struct UnkOwner_l105304 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x15c];
    UnkOwnerParams_l105304* params;  // 0x1f0

    f32 X() const { return xform.m[0][3]; }
    f32 Y() const { return xform.m[1][3]; }
};
static_assert(__builtin_offsetof(UnkOwner_l105304, params) == 0x1f0, "UnkOwner_l105304 layout");

struct UnkSelf_l105304 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l105304* owner;  // 0x30
    u8 pad_0x38[0x30];
    CVector3D* points;  // 0x68
    u32 count;          // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94
    u8 pad_0x95[0x10ab];
    s32 counter;  // 0x1140
    u8 pad_0x1144[0x13794];
    u8 boss_ready;  // 0x148d8

    // The path's points other than a closing one.
    u32 Open() const {
        bool open = closed == 0;
        return (count != 0) & open;
    }
};
static_assert(__builtin_offsetof(UnkSelf_l105304, counter) == 0x1140, "UnkSelf_l105304 layout");
static_assert(__builtin_offsetof(UnkSelf_l105304, boss_ready) == 0x148d8, "UnkSelf_l105304 layout");

static inline const base::math::CVector2D& Pt_l105304(const CVector3D* p) {
    return *reinterpret_cast<const base::math::CVector2D*>(p);
}

// x of the path's first point.
static inline f32 FirstX_l105304(const UnkSelf_l105304* self) {
    CVector3D* points = self->points;
    UnkSegment2D seg(Pt_l105304(points), Pt_l105304(self->count == 1 ? points : points + 1));
    return seg.start.x;
}

// x of the end of the path's last segment, 0 when there is none.
static inline f32 LastX_l105304(const UnkSelf_l105304* self) {
    u32 n = self->count - self->Open();
    if (n == 0)
        return 0.0f;
    u32 last = n - 1;
    CVector3D* points = self->points;
    UnkSegment2D seg(Pt_l105304(&points[last]), Pt_l105304(self->Open() ? &points[n] : &points[0]));
    return seg.end.x;
}

static inline f32 RandomRange(f32 lo, f32 hi) {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) * (hi - lo) + lo;
}

// Whether `d` lies inside the window; a bound that is not positive is not checked.
static inline bool InWindow(f32 d, f32 max, f32 min) {
    bool below_max = !(max > 0) | !(d >= max);
    bool above_min = !(min > 0) | !(d <= min);
    return below_max & above_min;
}

struct UnkEventSelf_b0143;

bool fn_7101105304(UnkEventSelf_b0143* event_self, CStrId* id, f32 chance, bool check_side, bool require_alive,
                   bool require_dead, f32 max_dx, f32 min_dx, f32 max_dy, f32 min_dy, f32 max_path_dx,
                   f32 min_path_dx, u32 min_counter, u32 max_counter, bool check_boss) {
    UnkSelf_l105304* self = reinterpret_cast<UnkSelf_l105304*>(event_self);
    if (!(chance > 0))
        return false;
    if (id->data == &g_unk_7101d47108->empty)
        return false;
    s32 lo = min_counter;
    s32 hi = max_counter;
    if ((lo >= 0 && self->counter < lo) || (hi >= 0 && self->counter > hi))
        return false;
    if (!(RandomRange(0.0f, 1.0f) < chance))
        return false;

    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    if (!target)
        return true;
    UnkOwner_l105304* other = reinterpret_cast<UnkOwner_l105304*>(target);

    bool ok = true;
    if (check_side) {
        UnkOwner_l105304* owner = self->owner;
        f32 direction = owner->params->direction;
        UnkVec3f target_pos;
        target_pos = other->xform.GetTranslation();
        UnkVec3f owner_pos;
        owner_pos = owner->xform.GetTranslation();
        f32 facing = owner->xform.GetAngle().y;
        ok = (direction > 0 && facing > 0 && target_pos.x > owner_pos.x) ||
             (direction < 0 && facing < 0 && target_pos.x < owner_pos.x);
    }
    if (ok && require_alive) {
        void* life = target->field_0x1b8;
        ok = life == nullptr || !fn_7101161088(life);
    }
    if (ok && require_dead) {
        void* life = target->field_0x1b8;
        if (life)
            ok = ok && fn_7101161088(life);
    }
    if (ok && (max_dx > 0 || min_dx > 0)) {
        f32 d = __builtin_fabsf(other->X() - self->owner->X());
        ok = ok && InWindow(d, max_dx, min_dx);
    }
    if (ok && (max_dy > 0 || min_dy > 0)) {
        f32 d = other->Y() - self->owner->Y();
        ok = ok && InWindow(d, max_dy, min_dy);
    }
    if (ok && check_boss) {
        base::reflection::UnkReflObject* obj = reinterpret_cast<base::reflection::UnkReflObject*>(self);
        if (fn_710009d50c(obj->vf_0x0(), fn_71003ab524()))
            ok = ok && self->boss_ready != 0;
    }
    if (ok && (max_path_dx > 0 || min_path_dx > 0)) {
        f32 facing = self->owner->xform.GetAngle().y;
        f32 x;
        if (facing > 0) {
            if (self->count == self->Open())
                return ok;
            x = LastX_l105304(self);
        } else {
            if (self->count == self->Open())
                return ok;
            x = FirstX_l105304(self);
        }
        f32 d = __builtin_fabsf(x - self->owner->X());
        ok = ok && InWindow(d, max_path_dx, min_path_dx);
    }
    return ok;
}
