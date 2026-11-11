// Unit 437 (no reliable name; hint 'CAimComponent'): twelve aim helpers, each with its own comment below.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkAnimationComponent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU341.h"
#include "game/UnkTU365.h"
#include "game/UnkTU437.h"
#include "game/UnkTU54.h"
#include "types.h"

#include <cmath>
#include <new>

// Unit 437: whether an aim angle (atan2f of the second value over the absolute first) lies inside
// the tunable limit band of the given region (0..6). Limits are read at +0xa4..+0xb8 of the aim
// tunables (g_unk_71726b1438). Names are placeholders.
struct UnkAimTunable;
static inline f32 AimLimit(u64 offset) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71726b1438) + offset);
}

bool fn_7100342ee4(u32 region, f32 x, f32 y) {
    f32 angle = atan2f(y, fabsf(x));
    f32 lo, hi;
    switch (region) {
        case 0:
            lo = AimLimit(0xb4);
            hi = AimLimit(0xac);
            if (angle > lo && angle < hi)
                return true;
            break;
        case 1:
            lo = AimLimit(0xa4);
            hi = AimLimit(0xa8);
            if (angle > lo && angle < hi)
                return true;
            break;
        case 2:
            if (angle > AimLimit(0xa8))
                return true;
            break;
        case 3:
            lo = AimLimit(0xac);
            hi = AimLimit(0xa4);
            if (angle > lo && angle < hi)
                return true;
            break;
        case 4:
            lo = AimLimit(0xb8);
            hi = AimLimit(0xb0);
            if (angle > lo && angle < hi)
                return true;
            break;
        case 5:
            lo = AimLimit(0xb0);
            hi = AimLimit(0xb4);
            if (angle > lo && angle < hi)
                return true;
            break;
        case 6:
            if (angle < AimLimit(0xb8))
                return true;
            break;
        default:
            break;
    }
    return false;
}


// Unit 437: turns the angle at out[0] toward the angle at out[1] at the speed of the object at
// +0x2b0 (its +0x64) times `step`, taking the short way round and stopping on the target; wraps the
// result. Without the flag at +0x570 only clears out[0]. Names and layouts are placeholders.

struct UnkAimSpeed_ovb0198g {
    u8 pad_0x0[0x64];
    f32 speed;  // 0x64
};

struct UnkAimTurn_ovb0198g {
    u8 pad_0x0[0x2b0];
    UnkAimSpeed_ovb0198g* mover;  // 0x2b0
    u8 pad_0x2b8[0x2b8];
    bool active;  // 0x570
};
static_assert(__builtin_offsetof(UnkAimTurn_ovb0198g, active) == 0x570, "layout");

void fn_7100347bd8(UnkAimTurn_ovb0198g* self, f32* out, f32 b);
void fn_7100347fe0(UnkAimTurn_ovb0198g* self, f32* out, bool flag, f32 b);
void fn_71003493d8(UnkAimTurn_ovb0198g* self, f32* out, f32 b);

void fn_7100347a70(UnkAimTurn_ovb0198g* self, f32* out, f32 step, f32 b, bool flag) {
    if (!self->active) {
        __builtin_memset(out, 0, sizeof(f32));
        return;
    }
    fn_7100347bd8(self, out, b);
    fn_7100347fe0(self, out, flag, b);
    fn_71003493d8(self, out, b);
    if (step < 0.0f) {
        __builtin_memcpy(out, out + 1, sizeof(f32));
        out[0] = fn_710007a1d0(out[1]);
        return;
    }
    f32 speed = self->mover->speed;
    f32 x = out[0];
    f32 y = out[1];
    f32 target = y < 0.0f ? y + 6.2831855f : y;
    if (x < 0.0f) {
        x += 6.2831855f;
        out[0] = x;
    }
    f32 diff = target - x;
    if (diff > 3.1415927f) {
        target = y + -6.2831855f;
    } else if (diff < -3.1415927f) {
        target = y + 6.2831855f;
    }
    if (x < target) {
        x = speed * step + x;
        out[0] = x;
        if (x > target) {
            x = target;
            out[0] = target;
        }
    } else if (x > target) {
        x = x - speed * step;
        out[0] = x;
        if (x < target) {
            x = target;
            out[0] = target;
        }
    }
    out[0] = fn_710007a1d0(x);
}


// Unit 437: whether an aim target counts: false for a target whose slot 0xb flags have bit 12, true
// for bit 7, else the player's inventory must hold ITEM_CENTRAL_UNIT_ENERGY and the target's
// collision component's object must carry one of two ids. Names and layouts are placeholders.
struct UnkInventory;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);

static inline const UnkQualifiedId& GetQualifiedId_ovb0198f(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkAimPlayer_ovb0198f {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

struct UnkAimIdSlot_ovb0198f {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkAimIdHolder_ovb0198f {
    UnkAimIdSlot_ovb0198f* slot;
};

struct UnkAimCollisionObject_ovb0198f : base::reflection::UnkReflObject {
    u8 pad_0x8[0x10];
    void* named;  // 0x18
};

struct UnkAimTarget_ovb0198f {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 GetFlags();  // 0x58
    u8 pad_0x8[0x8];
    UnkAimCollisionObject_ovb0198f* collision;  // 0x10
};

struct UnkNameHolder_b0161;

bool fn_710033fe10(UnkAimTarget_ovb0198f* target) {
    if (!target)
        return false;
    u32 flags = target->GetFlags();
    if (flags & 0x1000)
        return false;
    if (flags & 0x80)
        return true;
    UnkAimPlayer_ovb0198f* player = reinterpret_cast<UnkAimPlayer_ovb0198f*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    UnkInventory* inventory = player->inventory;
    if (!inventory)
        return false;
    if (!fn_71011428bc(inventory, GetQualifiedId_ovb0198f(g_unk_717275ac18)))
        return false;
    UnkAimCollisionObject_ovb0198f* collision = target->collision;
    if (!collision)
        return false;
    if (!fn_710009d50c(collision->vf_0x0(), fn_710085fc44()))
        return false;
    if (!collision->named)
        return false;
    u64 id = reinterpret_cast<UnkAimIdHolder_ovb0198f*>(fn_7100232fcc(collision->named))->slot->id;
    return id == 0x9a81146558dcd8feull || id == 0x2155f9f478783a9bull;
}


// Unit 437: turns the aim angle at +0x2b8 toward `target` (unwrapped near it by fn_710007a170) by the
// tunable's speed (+0x2b0, +0x7c) times `dt`, or straight to it for a negative `dt`. Inside the
// snap band around +0x2c0..+0x2c4 (an eighth of a turn wider) the speed at +0x2cc is used and the
// flag at +0x2d4 and the step at +0x2d0 are set. The result is wrapped to 0..2pi, and the owner's
// object at +0x1f0 is told the side (sign of the value at +0x330, else of the transform's third
// component). Names and layouts are placeholders.
struct UnkAim;

struct UnkAimTunable_ovb0198p {
    u8 pad_0x0[0x7c];
    f32 speed;  // 0x7c
};

struct UnkAimSide_ovb0198p {
    f32 x, y, z;
};

typedef void (*UnkAimSetSide_ovb0198p)(void* object, const UnkAimSide_ovb0198p* side);

struct UnkAimActor_ovb0198p {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkAimTurn_ovb0198p {
    u8 pad_0x0[0x30];
    UnkAimActor_ovb0198p* actor;  // 0x30
    u8 pad_0x38[0x278];
    UnkAimTunable_ovb0198p* tunable;  // 0x2b0
    f32 angle;                        // 0x2b8
    u8 pad_0x2bc[4];
    f32 band_lo;  // 0x2c0
    f32 band_hi;  // 0x2c4
    u8 pad_0x2c8[4];
    f32 band_speed;  // 0x2cc
    f32 band_step;   // 0x2d0
    u8 snapped;      // 0x2d4
    u8 pad_0x2d5[0x5b];
    f32 side;  // 0x330
};
static_assert(__builtin_offsetof(UnkAimTurn_ovb0198p, side) == 0x330, "layout");

void fn_7100349f28(UnkAimTurn_ovb0198p* self, f32 dt, f32 target) {
    fn_710007a170(reinterpret_cast<UnkAim*>(&target), self->angle);
    bool up = target > self->angle;
    f32 cur = self->angle;
    f32 v;
    if (up) {
        f32 lo = self->band_lo + -0.3926991f;
        self->snapped = 0;
        f32 hi = self->band_hi + 0.3926991f;
        f32 raw_lo = self->band_lo;
        f32 raw_hi = self->band_hi;
        if (lo >= 0.0f && hi >= 0.0f && target > lo && target < hi) {
            v = target;
            if (dt >= 0.0f)
                v = cur + self->band_speed * dt;
            if (v > target)
                v = target;
            if (raw_lo >= 0.0f && v > raw_lo && raw_hi >= 0.0f && v < raw_hi) {
                self->band_step = self->band_speed;
                self->angle = v;
                self->snapped = 1;
                goto wrap;
            }
        }
        if (dt >= 0.0f) {
            v = cur + self->tunable->speed * dt;
            self->angle = v;
            if (!(v > target))
                goto wrap;
        }
        v = target;
        self->angle = target;
    } else if (target < cur) {
        f32 lo = self->band_lo + -0.3926991f;
        self->snapped = 0;
        f32 hi = self->band_hi + 0.3926991f;
        f32 raw_lo = self->band_lo;
        f32 raw_hi = self->band_hi;
        if (lo >= 0.0f && hi >= 0.0f && target > lo && target < hi) {
            v = target;
            if (dt >= 0.0f)
                v = cur - self->band_speed * dt;
            if (v < target)
                v = target;
            if (raw_lo >= 0.0f && v > raw_lo && raw_hi >= 0.0f && v < raw_hi) {
                self->band_step = self->band_speed;
                self->angle = v;
                self->snapped = 1;
                goto wrap;
            }
        }
        if (dt >= 0.0f) {
            v = cur - self->tunable->speed * dt;
            self->angle = v;
            if (!(v < target))
                goto wrap;
        }
        v = target;
        self->angle = target;
    } else {
        v = cur;
    }
wrap:
    v = fn_710007a1d0(v);
    if (v < 0.0f)
        v += 6.2831855f;
    self->angle = v;
    void* object = *reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x1f0);
    if (object) {
        f32 s = self->side;
        if (s == 0.0f)
            s = fn_710002614c(&self->actor->transform).m[2];
        UnkAimSide_ovb0198p side;
        side.x = (f32)((s > 0.0f) - (s < 0.0f));
        __builtin_memset(&side.y, 0, 8);
        (*reinterpret_cast<UnkAimSetSide_ovb0198p**>(object))[0x208 / 8](object, &side);
    }
}


// Unit 437: speed needed to cover the distance between two points beyond 50 units within `time`.
// Names and layouts are placeholders.
struct UnkPoint3_ovb0198 {
    f32 x, y, z;
};

f32 fn_7100351fe0(void* self, f32 time, const UnkPoint3_ovb0198* a, const UnkPoint3_ovb0198* b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    f32 dist = sqrtf(dx * dx + dy * dy + dz * dz);
    f32 nx = dx / dist;
    f32 ny = dy / dist;
    f32 nz = dz / dist;
    f32 excess = dist + -50.0f;
    f32 t = excess > 0.0f ? excess : 0.0f;
    f32 wx = nx * t;
    f32 wy = ny * t;
    f32 wz = nz * t;
    f32 inv = 1.0f / time;
    f32 vx = inv * wx;
    f32 vy = inv * wy;
    f32 vz = inv * wz;
    return sqrtf(vx * vx + vy * vy + vz * vz);
}


// Unit 437: the nearest-target visitor of the aim in 3D (as fn_71003549a4's; fn_710033265c in
// src/game/CAbilityShinespark/CAbilityShinespark_Glow_ovb0045.cpp keeps the nearest by distance):
// a candidate that passes the class filter at +0x78 replaces the best one (record at +0x8, flag at
// +0x70) when there is none or it lies nearer to the point at +0x78 (x, y, z squared on both sides).
// Result 1 when taken, else the guarded "skip" 0. Names and layouts are placeholders.
extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_ovb0198l() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

struct UnkAssignedSlot {
    void* ptr;

    UnkAssignedSlot& operator=(const UnkAssignedSlot& other);
};
__attribute__((always_inline)) inline UnkAssignedSlot& UnkAssignedSlot::operator=(const UnkAssignedSlot& other) {
    fn_71002a5d1c(this, &other);
    return *this;
}

struct UnkAimTarget_ovb0198l {
    UnkAssignedSlot slot;
    UnkAssignedSlot slot_0x8;
    u32 words[13];  // 0x10

    f32 X() const { return *reinterpret_cast<const f32*>(&words[1]); }
    f32 Y() const { return *reinterpret_cast<const f32*>(&words[2]); }
    f32 Z() const { return *reinterpret_cast<const f32*>(&words[3]); }

    UnkAimTarget_ovb0198l& operator=(const UnkAimTarget_ovb0198l& other) {
        if (this != &other) {
            slot = other.slot;
            slot_0x8 = other.slot_0x8;
            for (u32 i = 0; i < 13; ++i)
                words[i] = other.words[i];
        }
        return *this;
    }
};

struct UnkAimFilter_ovb0198l;
bool fn_7100358d68(UnkAimFilter_ovb0198l* self, UnkAimTarget_ovb0198l* target);

struct UnkAimPoint_ovb0198l {
    f32 x, y, z;
};

struct UnkAimNearest_ovb0198l {
    u8 pad_0x0[8];
    UnkAimTarget_ovb0198l best;  // 0x8
    u8 pad_0x50[0x28];
    bool has_best;  // 0x78
    u8 pad_0x79[7];
    UnkAimPoint_ovb0198l point;  // 0x80
};
static_assert(__builtin_offsetof(UnkAimNearest_ovb0198l, has_best) == 0x78, "layout");

static inline f32 Sign_ovb0198l(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

UnkVisitResult_b0040 fn_7100358bc4(UnkAimNearest_ovb0198l* self, UnkAimTarget_ovb0198l* target) {
    if (fn_7100358d68(reinterpret_cast<UnkAimFilter_ovb0198l*>(&self->point), target)) {
        bool take = !self->has_best;
        if (!take) {
            f32 px = self->point.x;
            f32 py = self->point.y;
            f32 oldDx = px - self->best.X();
            f32 oldDxx = oldDx * oldDx;
            f32 oldDy = py - self->best.Y();
            f32 oldDyy = oldDy * oldDy;
            f32 oldDist = oldDxx;
            oldDist += oldDyy;
            f32 pz = self->point.z;
            f32 oldDz = pz - self->best.Z();
            oldDist = oldDist + oldDz * oldDz;
            f32 newDx = px - target->X();
            f32 newDxx = newDx * newDx;
            f32 newDy = py - target->Y();
            f32 newDyy = newDy * newDy;
            f32 newDist = newDxx;
            newDist += newDyy;
            f32 newDz = pz - target->Z();
            newDist = newDist + newDz * newDz;
            take = (s32)Sign_ovb0198l(oldDist - newDist) >= 1;
        }
        if (take) {
            self->has_best = true;
            self->best = *target;
            return UnkVisitResult_b0040(1);
        }
    }
    return Skip_ovb0198l();
}


// Unit 437: the nearest-target visitor of the aim in 3D (as fn_71003549a4's; fn_710033265c in
// src/game/CAbilityShinespark/CAbilityShinespark_Glow_ovb0045.cpp keeps the nearest by distance):
// a candidate that passes the class filter at +0x78 replaces the best one (record at +0x8, flag at
// +0x70) when there is none or it lies nearer to the point at +0x78 (x, y, z squared on both sides).
// Result 1 when taken, else the guarded "skip" 0. Names and layouts are placeholders.
static inline UnkVisitResult_b0040 Skip_ovb0198m() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

struct UnkAimRefOwner_ovb0198m {
    u8 pad_0x0[0x10];
    void* owner;  // 0x10
};

struct UnkAimTarget_ovb0198m {
    UnkAssignedSlot slot;
    UnkAssignedSlot slot_0x8;
    u32 words[13];  // 0x10

    UnkAimRefOwner_ovb0198m* Ref() const { return static_cast<UnkAimRefOwner_ovb0198m*>(slot.ptr); }
    f32 X() const { return *reinterpret_cast<const f32*>(&words[1]); }
    f32 Y() const { return *reinterpret_cast<const f32*>(&words[2]); }
    f32 Z() const { return *reinterpret_cast<const f32*>(&words[3]); }

    UnkAimTarget_ovb0198m& operator=(const UnkAimTarget_ovb0198m& other) {
        if (this != &other) {
            slot = other.slot;
            slot_0x8 = other.slot_0x8;
            for (u32 i = 0; i < 13; ++i)
                words[i] = other.words[i];
        }
        return *this;
    }
};

struct UnkAimPoint_ovb0198m {
    f32 x, y, z;
};

struct UnkAimNearest_ovb0198m {
    u8 pad_0x0[8];
    UnkAimTarget_ovb0198m best;  // 0x8
    u8 pad_0x50[0x20];
    bool has_best;  // 0x70
    u8 pad_0x71[7];
    UnkAimPoint_ovb0198m point;  // 0x78
    u8 pad_0x84[4];
    void* ignore;  // 0x88
};
static_assert(__builtin_offsetof(UnkAimNearest_ovb0198m, has_best) == 0x70, "layout");

static inline f32 Sign_ovb0198m(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

UnkVisitResult_b0040 fn_7100358f64(UnkAimNearest_ovb0198m* self, UnkAimTarget_ovb0198m* target) {
    void* ignore = self->ignore;
    if (!(ignore && target->Ref() && target->Ref()->owner == ignore)) {
        bool take = !self->has_best;
        if (!take) {
            f32 px = self->point.x;
            f32 py = self->point.y;
            f32 oldDx = px - self->best.X();
            f32 oldDxx = oldDx * oldDx;
            f32 oldDy = py - self->best.Y();
            f32 oldDyy = oldDy * oldDy;
            f32 oldDist = oldDxx;
            oldDist += oldDyy;
            f32 pz = self->point.z;
            f32 oldDz = pz - self->best.Z();
            oldDist = oldDist + oldDz * oldDz;
            f32 newDx = px - target->X();
            f32 newDxx = newDx * newDx;
            f32 newDy = py - target->Y();
            f32 newDyy = newDy * newDy;
            f32 newDist = newDxx;
            newDist += newDyy;
            f32 newDz = pz - target->Z();
            newDist = newDist + newDz * newDz;
            take = (s32)Sign_ovb0198m(oldDist - newDist) >= 1;
        }
        if (take) {
            self->has_best = true;
            self->best = *target;
            return UnkVisitResult_b0040(1);
        }
    }
    return Skip_ovb0198m();
}

// Unit 437: caches the shot orientation of the aim: clears the flag at +0x990, asks the owner's
// animation (+0x220 of the actor at +0x30) for a CCharClassUseDC_ShotOrientationTrack and, with its
// record, takes the x axis of the node transform the id table's id at +0xd58 names on the entity
// (+0x228); when the axis' z is below one and the record's flag (+0x18) is set the axis is flattened
// and renormalised. Stores it at +0x984 and sets the flag. Names and layouts are placeholders.
using base::global::CStrId;

struct UnkEntity;
bool fn_71011735e8(UnkEntity* entity, const void* key, UnkMatrix44* out, u32 flags);

struct UnkGameManagerIdTable {
    u8 pad_0x0[0x2578];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkAimActor_ovb0198v {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x0];
    UnkEntity* entity;  // 0x228
};

struct UnkAimRecord_ovb0198v {
    u8 pad_0x0[0x18];
    bool flat;  // 0x18
};

struct UnkAimShot_ovb0198v {
    u8 pad_0x0[0x30];
    UnkAimActor_ovb0198v* actor;  // 0x30
    u8 pad_0x38[0x94c];
    f32 axis[3];  // 0x984
    u8 valid;     // 0x990
};
static_assert(__builtin_offsetof(UnkAimShot_ovb0198v, valid) == 0x990, "layout");

void fn_710034f374(UnkAimShot_ovb0198v* self) {
    self->valid = 0;
    UnkAnimationComponent* animation = self->actor->animation;
    UnkTrackQuery query = {fn_7100d34f18(), nullptr, -1.0f};
    if (!animation)
        return;
    fn_710117b08c(animation, &query);
    if (!query.result)
        return;
    UnkMatrix44 m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    UnkEntity* entity = self->actor->entity;
    const CStrId& id = *reinterpret_cast<const CStrId*>(reinterpret_cast<u8*>(&GameManagerIdTable()) + 0xd58);
    u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id.data) + 0x30);
    if (!fn_71011735e8(entity, &key, &m, 0))
        return;
    // The axis is copied as words and only converted when it is renormalised.
    const u32* words = reinterpret_cast<const u32*>(m.m);
    u32 xw = words[0];
    u32 yw = words[4];
    u32 zw = words[8];
    f32 z;
    __builtin_memcpy(&z, &zw, 4);
    if (z < 1.0f && reinterpret_cast<UnkAimRecord_ovb0198v*>(query.result)->flat) {
        f32 x;
        f32 y;
        __builtin_memcpy(&x, &xw, 4);
        __builtin_memcpy(&y, &yw, 4);
        f32 xx = x * x;
        f32 yy = y * y;
        f32 sum = xx;
        sum += yy;
        f32 inv = 1.0f / (sqrtf(sum + 0.0f) + 0.0001f);
        f32 nx = inv * x;
        f32 ny = inv * y;
        f32 nz = inv * 0.0f;
        __builtin_memcpy(&xw, &nx, 4);
        __builtin_memcpy(&yw, &ny, 4);
        __builtin_memcpy(&zw, &nz, 4);
    }
    __builtin_memcpy(&self->axis[0], &xw, 4);
    __builtin_memcpy(&self->axis[1], &yw, 4);
    __builtin_memcpy(&self->axis[2], &zw, 4);
    self->valid = 1;
}

// Unit 437: sets the on/off flag at +0x640; on a change releases the handle at +0x630 and every
// handle of the array at +0x648 (count +0x650), then restores the saved block at +0x4f8..0x514 into
// +0x510..0x528. Names and layouts are placeholders.
struct UnkRefCounted_ovb0198d {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct __attribute__((packed)) UnkWord_ovb0198d {
    u64 v;
};

struct UnkAimState_ovb0198d {
    u8 pad_0x0[0x4f8];
    u64 field_0x4f8;
    u32 field_0x500;
    UnkWord_ovb0198d field_0x504;
    u32 field_0x50c;
    u64 field_0x510;
    u32 field_0x518;
    UnkWord_ovb0198d field_0x51c;
    u32 field_0x524;
    u8 pad_0x528[0x108];
    UnkRefCounted_ovb0198d* handle;  // 0x630
    u8 pad_0x638[8];
    bool flag;                       // 0x640
    u8 field_0x641;
    u8 pad_0x642[6];
    UnkRefCounted_ovb0198d** handles;  // 0x648
    u32 handleCount;                   // 0x650
};
static_assert(__builtin_offsetof(UnkAimState_ovb0198d, handleCount) == 0x650, "layout");

void fn_7100352a2c(UnkAimState_ovb0198d* self, bool on) {
    if (self->flag == on)
        return;
    self->flag = on;
    if (UnkRefCounted_ovb0198d* h = self->handle) {
        if (--h->ref_count == 0)
            h->Delete();
        self->handle = nullptr;
    }
    for (u64 i = 0; i < self->handleCount; ++i) {
        UnkRefCounted_ovb0198d* h = self->handles[i];
        if (h && --h->ref_count == 0)
            h->Delete();
    }
    // Keeps the saved-block address calculations after the release loop.
    asm("" : "+r"(self));
    self->field_0x510 = self->field_0x4f8;
    u32 word = self->field_0x500;
    UnkWord_ovb0198d* dst = reinterpret_cast<UnkWord_ovb0198d*>(reinterpret_cast<u8*>(self) + 0x51c);
    self->handleCount = 0;
    // Keeps the destination as its own address instead of an offset from the source.
    asm("" : "+r"(dst));
    self->field_0x518 = word;
    UnkWord_ovb0198d* src = reinterpret_cast<UnkWord_ovb0198d*>(reinterpret_cast<u8*>(self) + 0x504);
    *dst = *src;
    self->field_0x524 = self->field_0x50c;
    self->field_0x641 = 0;
}

// Unit 437: adds an aim target to the fixed list of 64 (u32 count at +0x8, 0x48-byte records from
// +0xc) when the class filter at +0x1210 (fn_7100354b34) accepts it, then sorts it in
// (fn_7100354e10, with a copy of the filter's first three words). Result: 1 added, 2 full, 3 added
// and now full, else the guarded 0. Names and layouts are placeholders.
static inline UnkVisitResult_b0040 Skip_ovb0198s() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

#pragma pack(push, 4)
struct UnkAimRecord_ovb0198s {
    u64 unk_0x0[9];
    UnkAimRecord_ovb0198s() {}
    UnkAimRecord_ovb0198s(const UnkAimRecord_ovb0198s& other) { fn_7100331c7c(this, &other); }
};

struct UnkAimRecordArray_ovb0198s {
    u32 count;
    UnkAimRecord_ovb0198s items[0x40];
};
#pragma pack(pop)

struct UnkAimFilterWords_ovb0198s {
    u64 a;
    u32 b;
    u64 c;
};

struct UnkAimTargets_ovb0198s {
    void* vtable;
    UnkAimRecordArray_ovb0198s array;  // 0x8
    UnkAimFilterWords_ovb0198s filter;  // 0x1210
};
static_assert(__builtin_offsetof(UnkAimTargets_ovb0198s, filter) == 0x1210, "layout");

struct UnkAimFilter_ovb0198s;
bool fn_7100354b34(UnkAimFilter_ovb0198s* self, const UnkAimRecord_ovb0198s* entry);
void fn_7100354e10(UnkAimRecord_ovb0198s* begin, UnkAimRecord_ovb0198s* position, const UnkAimFilterWords_ovb0198s* key);

struct UnkAimAddResult_ovb0198s {
    s32 code;
    UnkAimAddResult_ovb0198s(s32 c) : code(c) {}
    UnkAimAddResult_ovb0198s(const UnkAimAddResult_ovb0198s& other) : code(other.code) {}
};

// Sorts the list after the result has been built.
struct UnkAimSortOnExit_ovb0198s {
    UnkAimTargets_ovb0198s* targets;
    u32 count;
    ~UnkAimSortOnExit_ovb0198s() {
        UnkAimTargets_ovb0198s* object = targets;
        // Keeps the key loads relative to the object, not to the filter pointer passed earlier.
        asm("" : "+r"(object));
        u64 a = object->filter.a;
        u32 b = object->filter.b;
        UnkAimRecord_ovb0198s* end = object->array.items + count;
        UnkAimFilterWords_ovb0198s key;
        key.a = a;
        key.b = b;
        key.c = object->filter.c;
        fn_7100354e10(object->array.items, end, &key);
    }
};

UnkVisitResult_b0040 fn_7100354d04(UnkAimTargets_ovb0198s* self, const UnkAimRecord_ovb0198s* entry) {
    if (!fn_7100354b34(reinterpret_cast<UnkAimFilter_ovb0198s*>(reinterpret_cast<u8*>(self) + 0x1210), entry))
        return Skip_ovb0198s();
    UnkAimSortOnExit_ovb0198s sorter = {self, 0};
    u32 n = self->array.count;
    sorter.count = n;
    s32 free = 0x40 - (s32)n;
    // Keeps the free-slot count as a separate subtraction.
    asm("" : "+r"(free));
    if (free > 0) {
        u8* base = reinterpret_cast<u8*>(self) + 8;
        fn_7100331c7c(base + (u64)n * 0x48 + 4, entry);
        u32 grown = self->array.count + 1;
        self->array.count = grown;
        sorter.count = grown;
        return UnkVisitResult_b0040(free == 1 ? 3 : 1);
    }
    return UnkVisitResult_b0040(2);
}

// Unit 437: appends a default-constructed 0x48-byte record to a fixed list (u32 count at +0x8,
// records from +0xc). Result code: 1 added, 2 full, 3 added and now full. Names are placeholders.
// The record initialiser, called without a source (the original leaves its second argument alone).
void InitRecord_ovb0198(void* self) asm("_Z12fn_7100331c7cPvPKv");

#pragma pack(push, 4)
struct UnkRecord_ovb0198 {
    u64 unk_0x0[9];
};

template <int N>
struct UnkRecordArray_ovb0198 {
    u32 count;
    UnkRecord_ovb0198 items[N];
};

template <int N>
struct UnkRecordList_ovb0198 {
    void* vtable;
    UnkRecordArray_ovb0198<N> array;  // 0x8
};
#pragma pack(pop)

struct UnkAddResult_ovb0198 {
    s32 code;
    UnkAddResult_ovb0198(s32 c) : code(c) {}
    UnkAddResult_ovb0198(const UnkAddResult_ovb0198& other) : code(other.code) {}
};

template <int N>
static inline UnkAddResult_ovb0198 AddDefault(UnkRecordList_ovb0198<N>* self) {
    u32 n = self->array.count;
    s32 free = N - (s32)n;
    // Keeps the free-slot count as a separate subtraction.
    asm("" : "+r"(free));
    if (free > 0) {
        u8* base = reinterpret_cast<u8*>(self) + 8;
        InitRecord_ovb0198(base + (u64)n * 0x48 + 4);
        u32 grown = self->array.count + 1;
        // Keeps the last-slot test after the call.
        asm("" : "+r"(free));
        bool last = free == 1;
        self->array.count = grown;
        return UnkAddResult_ovb0198(last ? 3 : 1);
    }
    return UnkAddResult_ovb0198(2);
}

UnkAddResult_ovb0198 fn_7100353a40(UnkRecordList_ovb0198<0x40>* self) { return AddDefault(self); }
UnkAddResult_ovb0198 fn_7100353b74(UnkRecordList_ovb0198<0x80>* self) { return AddDefault(self); }
