// CAutoolAIComponent's unit (tools/tu_map.py): the AI's reset, its collider and effect switches, and the reserve of its two parallel point arrays.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAutoolAIComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU134.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkSelf;
void fn_7100662110(void* self);
struct UnkObject;
struct UnkComponent;
struct UnkPart;
struct UnkWorld;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
struct UnkSwitchObject;
void fn_7100863730(void* collision, const CStrId* name, const u32* flags);
void fn_71008635a4(void* collision, const CStrId* name, const u32* flags);

// The colliders switched off with the effects (names not recovered).
extern CStrId g_unk_71726be9b8 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726be9c0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726be9c8 __attribute__((visibility("hidden")));

struct UnkAutoolOwner_ovb0137 {
    u8 pad_0x0[0x280];
    UnkObject* effects;  // 0x280
    u8 pad_0x288[0x18];
    void* collision;  // 0x2a0
};

struct UnkAutoolAI_ovb0137 {
    u8 pad_0x0[0x30];
    UnkAutoolOwner_ovb0137* owner;  // 0x30
};

static inline void SwitchOff(UnkAutoolAI_ovb0137* self, u64 a, u64 b) {
    UnkComponent* first = reinterpret_cast<UnkComponent*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(self->owner->effects), a));
    UnkComponent* second = reinterpret_cast<UnkComponent*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(self->owner->effects), b));
    fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(first), false);
    fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(second), false);
}

// The base reset, then timeline kind 2 and three colliders set to 0x100, each with its two effects off.
void fn_71003c60c0(UnkAutoolAI_ovb0137* self) {
    u32 flags_c, flags_b, flags_a;
    fn_7100662110(self);
    fn_71003c4e64(reinterpret_cast<UnkSelf*>(self), 2);
    if (!self->owner->collision)
        return;
    flags_a = 0x100;
    fn_7100863730(self->owner->collision, &g_unk_71726be9b8, &flags_a);
    SwitchOff(self, 0xa08f6cbc1d98f726, 0xa8b007d46b53867d);
    if (!self->owner->collision)
        return;
    flags_b = 0x100;
    fn_7100863730(self->owner->collision, &g_unk_71726be9c0, &flags_b);
    SwitchOff(self, 0x9fd0648f1eae4a1c, 0x97ef0fe768653b47);
    if (!self->owner->collision)
        return;
    flags_c = 0x100;
    fn_7100863730(self->owner->collision, &g_unk_71726be9c8, &flags_c);
    SwitchOff(self, 0xc924614080498486, 0xd45cceb79172305e);
}

#define AUTOOL_SET_COLLIDER(name)                                       if (mode) {                                                             u32 flags = 0x100;                                                  fn_71008635a4(self->owner->collision, &(name), &flags);         } else {                                                                u32 flags = 0x100;                                                  fn_7100863730(self->owner->collision, &(name), &flags);         }

#define AUTOOL_SET_COLLIDER_B(name)                                     if (!mode) {                                                            u32 flags = 0x100;                                                  fn_7100863730(self->owner->collision, &(name), &flags);         } else {                                                                u32 flags = 0x100;                                                  fn_71008635a4(self->owner->collision, &(name), &flags);         }

// Sets collider `which` (0-2) for `mode`, then switches its two effects: both off in mode 0, the
// second on in mode 1, the first on in mode 2.
void fn_71003c4fb0(UnkAutoolAI_ovb0137* self, u32 which, u32 mode) {
    if (!self->owner->collision)
        return;
    u64 b = ~0ull;
    u64 a = b;
    switch (which) {
    case 0:
        a = 0xa08f6cbc1d98f726;
        b = 0xa8b007d46b53867d;
        AUTOOL_SET_COLLIDER(g_unk_71726be9b8)
        break;
    case 1:
        a = 0x9fd0648f1eae4a1c;
        b = 0x97ef0fe768653b47;
        AUTOOL_SET_COLLIDER(g_unk_71726be9c0)
        break;
    case 2:
        a = 0xc924614080498486;
        b = 0xd45cceb79172305e;
        AUTOOL_SET_COLLIDER_B(g_unk_71726be9c8)
        break;
    default:
        break;
    }
    UnkComponent* first = reinterpret_cast<UnkComponent*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(self->owner->effects), a));
    UnkComponent* second = reinterpret_cast<UnkComponent*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(self->owner->effects), b));
    switch (mode) {
    case 0:
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(first), false);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(second), false);
        break;
    case 1:
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(first), false);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(second), true);
        break;
    case 2:
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(first), true);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(second), false);
        break;
    }
}

// A refcounted pointer slot: assigning raises the new object's count and releases the previous one.
struct UnkAutoolRefSlot_ovb0137 {
    union {
        base::UnkRefCounted* ptr;
    };

    __attribute__((always_inline)) void Assign(base::UnkRefCounted* p) {
        if (p) {
            ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
    __attribute__((always_inline)) void Release() {
        if (base::UnkRefCounted* old = ptr) {
            if (--old->ref_count == 0)
                delete old;
        }
    }
};

// Two parallel arrays (refcounted objects and a point for each) sharing a count and a capacity.
struct UnkAutoolPointArray_ovb0137 {
    UnkAutoolRefSlot_ovb0137* refs;
    UnkVec3f* points;  // 0x8
    u32 count;                       // 0x10
    u32 capacity;                    // 0x14
    u32 step;
    u32 flags;                    // 0x1c
    base::UnkAllocator* allocator;  // 0x20
};

// Reserves room for `n` entries in both arrays.
void fn_71003c6fa4(UnkAutoolPointArray_ovb0137* self, u32 n) {
    if (self->capacity >= n)
        return;
    UnkAutoolRefSlot_ovb0137* refs =
        static_cast<UnkAutoolRefSlot_ovb0137*>(self->allocator->vf_0x40((u64)n * 8, 8, self->flags));
    UnkVec3f* points =
        static_cast<UnkVec3f*>(self->allocator->vf_0x40((u64)n * 12, 8, self->flags));
    if (UnkAutoolRefSlot_ovb0137* old = self->refs) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j) {
            refs[j].ptr = nullptr;
            refs[j].Assign(old[j].ptr);
            old[j].Release();
        }
        self->allocator->vf_0x48(self->refs);
    }
    if (UnkVec3f* src = self->points) {
        UnkVec3f* dst = points;
        for (u32 i = 0; i < self->count; ++i) {
            new (dst) UnkVec3f(*src);
            ++src;
            ++dst;
        }
        self->allocator->vf_0x48(self->points);
    }
    self->refs = refs;
    self->points = points;
    self->capacity = n;
}

extern "C" int rand();
extern base::reflection::UnkTunable g_unk_71726beb18;

// CTunableCharClassAutoolAIComponent's two wait times (+0x28, +0x2c).
struct UnkAutoolTunable_ovb0137 {
    u8 pad_0x0[0x28];
    f32 wait_min;
    f32 wait_max;
};

// An entry of the action list: polymorphic, 0x40 bytes, destroyed in place.
struct UnkAutoolAction_ovb0137 {
    virtual ~UnkAutoolAction_ovb0137();
    u8 pad_0x8[0x38];
};

struct UnkAutoolState_ovb0137 {
    UnkAutoolAction_ovb0137* actions;  // 0x0
    u32 action_count;                  // 0x8
    u8 pad_0xc[0x1c];
    UnkRefObject* field_0x28;
    UnkRefObject** refs;  // 0x30
    u8 pad_0x38[8];
    u32 ref_count;  // 0x40
    u8 pad_0x44[0x1c];
    u32 field_0x60;
    u8 pad_0x64[0x1c];
    UnkRefObject* field_0x80;
    UnkRefObject* field_0x88;
    UnkRefObject* field_0x90;
    bool field_0x98;
    bool field_0x99;
    bool field_0x9a;
    u8 pad_0x9b;
    f32 wait;  // 0x9c
    u64 field_0xa0;
    u64 field_0xa8;
    base::math::CVector3D field_0xb0;
    u8 pad_0xbc[4];
    u8 link[0x18];          // 0xc0
    UnkWeakHandle* target;  // 0xd8
};

struct UnkAutoolAIReset_ovb0137 {
    u8 pad_0x0[0x146d8];
    UnkAutoolState_ovb0137 state;
};

static inline f32 Random01() { return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)); }

// Resets the AI: the base reset, then the state block at +0x146d8 (references dropped, flags
// cleared, a new random wait between the tunable's limits, the action list and reference list
// emptied, and the target's listener removed).
void fn_71003c3424(UnkAutoolAIReset_ovb0137* self) {
    UnkAutoolState_ovb0137* s = &self->state;
    fn_71003695b8(reinterpret_cast<UnkBehaviorTreeAI*>(self));
    ResetRef(&s->field_0x28);
    ResetRef(&s->field_0x80);
    ResetRef(&s->field_0x88);
    ResetRef(&s->field_0x90);
    s->field_0x98 = false;
    s->field_0x99 = false;
    s->field_0x9a = false;
    UnkAutoolTunable_ovb0137* tunable = reinterpret_cast<UnkAutoolTunable_ovb0137*>(&g_unk_71726beb18);
    f32 min = tunable->wait_min;
    f32 max = tunable->wait_max;
    f32 r = Random01();
    s->wait = min + (max - min) * r;
    u64* cleared = reinterpret_cast<u64*>(reinterpret_cast<u8*>(self) + 0x14778);  // state +0xa0
    cleared[0] = 0;
    cleared[1] = 0;
    s->field_0xb0.x = g_unk_710164779c.x;
    s->field_0xb0.y = g_unk_710164779c.y;
    s->field_0xb0.z = g_unk_710164779c.z;
    for (u64 i = 0; i < s->action_count; ++i)
        s->actions[i].~UnkAutoolAction_ovb0137();
    s->action_count = 0;
    if (s->refs) {
        for (u64 i = 0; i < s->ref_count; ++i) {
            if (UnkRefObject* p = s->refs[i]) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
            }
        }
    }
    s->ref_count = 0;
    s->field_0x60 = 0;
    if (UnkWeakHandle* handle = s->target) {
        if (handle->target)
            fn_710117b3f0(handle->target, s->link);
        ResetRef(&s->target);
    }
}
