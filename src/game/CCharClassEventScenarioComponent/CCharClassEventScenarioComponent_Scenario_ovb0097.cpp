// CCharClassEventScenarioComponent unit (tools/tu_map.py unit 1251): the event scenario component's
// constructor (fn_71007e0d60: four callback registrations, the "relax"/"react" state ids, five arrays,
// the on/off delays at -1 and the flags), its reset (fn_71007e3b60: off timer cleared, the
// registrations dropped from their sets, the random values redrawn), the restart after a delay
// (fn_71007e3a8c, through fn_71007e2ecc with mode 3), the timer array rebuilt with one timer per
// entry (fn_71007e2cf4) and its element accessor (fn_71007e45dc), the size setter of a small id list
// (fn_71007e572c) and the collider rebuilt from a shape (fn_71007e4328, as fn_7100a4c444 in
// src/game/CColliderTriggerComponent/CColliderTriggerComponent_Shape_ovb0058.cpp). Names and
// layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/CComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/UnkTU67.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::global::CStrId;

extern "C" int rand();

struct UnkStateObject;
void fn_71007e5304(char* self);  // the string id list's constructor (+0xa0)
extern void* g_unk_7101a73408[];  // the component's vtable

struct UnkEventScenario_b49a2;

// The collision manager (as src/game/CColliderTriggerComponent/CColliderTriggerComponent_Shape_ovb0058.cpp
// declares it).
struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);
struct UnkCollisionObject;
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* entry);
struct UnkStruct_71007e44b8;
struct UnkObj_71007e44b8;
UnkObj_71007e44b8* fn_71007e44b8(UnkStruct_71007e44b8* self, void* owner, const base::global::CStrId* name, u32 index);
struct UnkTriggerCollider_b0058;

// A scenario entry (0x28 bytes): the bound of its random value at +8 (none when zero).
struct UnkEventScenarioEntry_ovb0097 {
    u8 pad_0x0[0x8];
    s32 max_random;  // 0x8
    u8 pad_0xc[0x28 - 0xc];
};

// A timer of the scenario (0x18 bytes): -1 in the first word, the time at +8.
struct UnkScenarioTimer_ovb0097 {
    u64 field_0x0;
    union {
        u64 field_0x8;
        f32 time;  // 0x8
    };
    u64 field_0x10;

    UnkScenarioTimer_ovb0097() {
        field_0x10 = 0;
        field_0x8 = 0;
        field_0x0 = ~0ul;
    }
    // Copied bitwise (the original reloads the count after each element).
    UnkScenarioTimer_ovb0097(const UnkScenarioTimer_ovb0097& o) { __builtin_memcpy(this, &o, sizeof(o)); }
};

// The timer array as fn_71007e2cf4 grows it: one element at a time, to exactly the new count.
struct UnkScenarioTimers_ovb0097 {
    UnkScenarioTimer_ovb0097* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkScenarioTimer_ovb0097& Resize(u32 n) {
        if (capacity < n) {
            auto* p = static_cast<UnkScenarioTimer_ovb0097*>(
                allocator->vf_0x40((u64)n * sizeof(UnkScenarioTimer_ovb0097), 8, flags));
            if (data) {
                UnkScenarioTimer_ovb0097* src = data;
                UnkScenarioTimer_ovb0097* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++src)
                    new (dst) UnkScenarioTimer_ovb0097(*src);
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        for (u32 i = count; i < n; ++i)
            new (&data[i]) UnkScenarioTimer_ovb0097();
        count = n;
        UnkScenarioTimer_ovb0097* end = &data[n];
        return n ? end[-1] : *end;
    }
};

// The scenario's collider: created by the collision manager, then configured through its vtable.
struct UnkScenarioCollider_ovb0097 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(bool on);
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60(bool on);
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void SetTransform(const UnkMatrix44& m);  // 0x78
};

struct UnkScenarioActor_ovb0097 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkEventScenario_ovb0097;

// A callback registration: the set it is in (none yet), the component and the function.
struct UnkScenarioReg_ovb0097 {
    base::UnkDeferredSet<void>* set;
    UnkEventScenario_ovb0097* object;
    void (*callback)(UnkEventScenario_ovb0097*);
};

void fn_71007e566c(UnkEventScenario_ovb0097* self);
void fn_71007e5694(UnkEventScenario_ovb0097* self);
void fn_71007e56bc(UnkEventScenario_ovb0097* self);
void fn_71007e56f4(UnkEventScenario_ovb0097* self);

struct UnkEventScenario_ovb0097 {
    void** vtable;
    u8 base_0x8[0x28];
    UnkScenarioActor_ovb0097* owner;  // 0x30
    u8 base_0x38[0x8];
    UnkScenarioReg_ovb0097 reg_0x40;
    UnkScenarioReg_ovb0097 reg_0x58;
    UnkScenarioReg_ovb0097 reg_0x70;
    UnkScenarioReg_ovb0097 reg_0x88;
    u8 ids_0xa0[0x38];
    CStrId relax;        // 0xd8
    CStrId relax2react;  // 0xe0
    CStrId react;        // 0xe8
    CStrId react2relax;  // 0xf0
    UnkScenarioCollider_ovb0097* collider;  // 0xf8
    void* shape;                            // 0x100
    base::UnkVector<void*> field_0x108;
    base::UnkVector<UnkEventScenarioEntry_ovb0097> entries;  // 0x130
    base::UnkVector<u32> randoms;                            // 0x158
    base::UnkVector<UnkScenarioTimer_ovb0097> timers;        // 0x180
    base::UnkVector<void*> field_0x1a8;
    f32 on_delay;   // 0x1d0
    f32 off_delay;  // 0x1d4
    f32 on_timer;   // 0x1d8
    f32 off_timer;  // 0x1dc
    u32 field_0x1e0;
    bool field_0x1e4;
    bool field_0x1e5;
    bool field_0x1e6;
    bool field_0x1e7;
    bool field_0x1e8;
    bool field_0x1e9;
    bool field_0x1ea;
    bool field_0x1eb;
    bool field_0x1ec;
    bool finished;  // 0x1ed
    bool active;    // 0x1ee
};
static_assert(__builtin_offsetof(UnkEventScenario_ovb0097, field_0x1a8) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEventScenario_ovb0097, field_0x1e0) == 0x1e0, "layout");

void fn_71007e0d60(UnkEventScenario_ovb0097* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a73408 + 2;
    self->reg_0x40 = {nullptr, self, fn_71007e566c};
    self->reg_0x58 = {nullptr, self, fn_71007e5694};
    self->reg_0x70 = {nullptr, self, fn_71007e56bc};
    self->reg_0x88 = {nullptr, self, fn_71007e56f4};
    fn_71007e5304(reinterpret_cast<char*>(self->ids_0xa0));
    new (&self->relax) CStrId("relax", true);
    new (&self->relax2react) CStrId("relax2react", true);
    new (&self->react) CStrId("react", true);
    new (&self->react2relax) CStrId("react2relax", true);
    self->collider = nullptr;
    self->shape = nullptr;
    new (&self->field_0x108) base::UnkVector<void*>(0xf);
    new (&self->entries) base::UnkVector<UnkEventScenarioEntry_ovb0097>(0xf);
    new (&self->randoms) base::UnkVector<u32>(0xf);
    new (&self->timers) base::UnkVector<UnkScenarioTimer_ovb0097>(0xf);
    new (&self->field_0x1a8) base::UnkVector<void*>(0xf);
    self->on_delay = -1.0f;
    self->off_delay = -1.0f;
    self->on_timer = 0.0f;
    self->off_timer = 3.40282347e+38f;
    self->field_0x1e0 = 0;
    self->field_0x1e4 = false;
    self->field_0x1e5 = true;
    self->field_0x1e6 = true;
    self->field_0x1e7 = true;
    self->field_0x1e8 = false;
    self->field_0x1e9 = false;
    self->field_0x1ea = false;
    self->field_0x1eb = true;
    self->field_0x1ec = true;
    self->finished = false;
    self->active = false;
}

// Rebuilds the timer array: one timer per entry of the array at +0x108, each starting at -1.
void fn_71007e2cf4(UnkEventScenario_ovb0097* self) {
    auto* timers = reinterpret_cast<UnkScenarioTimers_ovb0097*>(&self->timers);
    timers->count = 0;
    for (u32 i = 0; i < self->field_0x108.count; ++i)
        timers->Resize(timers->count + 1).time = -1.0f;
}

// The off timer cleared, the event not active; not finished either when the off delay is negative.
static inline void Deactivate(UnkEventScenario_ovb0097* self) {
    self->off_timer = 0.0f;
    self->active = false;
    if (!self->field_0x1e4 && self->off_delay < 0.0f)
        self->finished = false;
}

// A new random value below each entry's bound (rand() & 0x7fff), stored at the entry's index.
static inline void RandomizeEntries(UnkEventScenario_ovb0097* self) {
    for (u64 i = 0; i < self->entries.count; ++i) {
        UnkEventScenarioEntry_ovb0097* entry = &self->entries.data[i];
        if (entry->max_random) {
            fn_710008c504(&self->randoms, (u32)i);
            s32 max = entry->max_random;
            *fn_710008c504(&self->randoms, (u32)i) = max ? (rand() & 0x7fff) % max : 0;
        }
    }
}

// fn_71007e2ecc's first argument is a mode (3 here), not the bool src/game/CCharClassEventScenarioComponent/
// CEventScenarioComponent_b49.cpp declares.
using UnkScenarioStart_ovb0097 = void (*)(UnkEventScenario_ovb0097*, s32, bool, f32, f32, f32, f32);

void fn_71007e3a8c(UnkEventScenario_ovb0097* self) {
    self->finished = false;
    reinterpret_cast<UnkScenarioStart_ovb0097>(fn_71007e2ecc)(self, 3, false, 0.0f, 0.0f, 0.0f,
                                                              self->field_0x1e4 ? 0.0f : 3.0f);
    RandomizeEntries(self);
}

void fn_71007e3b60(UnkEventScenario_ovb0097* self) {
    Deactivate(self);
    if (self->reg_0x40.set) {
        fn_71004f3ea4(self->reg_0x40.set, &self->reg_0x40);
        self->reg_0x40.set = nullptr;
    }
    if (self->reg_0x58.set) {
        fn_71004f3ea4(self->reg_0x58.set, &self->reg_0x58);
        self->reg_0x58.set = nullptr;
    }
    if (self->reg_0x70.set) {
        fn_7100514b7c(self->reg_0x70.set, &self->reg_0x70);
        self->reg_0x70.set = nullptr;
    }
    if (self->reg_0x88.set) {
        fn_7100514b7c(self->reg_0x88.set, &self->reg_0x88);
        self->reg_0x88.set = nullptr;
    }
    RandomizeEntries(self);
}

static inline UnkManager_7100863a8c* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

// Hands the old collider back to the manager, has it create one named after the actor, places it
// (while the game manager allows it), and gives it the shape.
void fn_71007e4328(UnkEventScenario_ovb0097* self, void* shape, const UnkMatrix44* transform) {
    if (self->collider) {
        fn_7100d46c60(Manager(), reinterpret_cast<UnkCollisionObject*>(self->collider));
        self->collider = nullptr;
    }
    UnkManager_7100863a8c* manager = Manager();
    {
        CStrId name(self->owner->name);
        self->collider = reinterpret_cast<UnkScenarioCollider_ovb0097*>(
            fn_71007e44b8(reinterpret_cast<UnkStruct_71007e44b8*>(manager), self, &name, 0));
    }
    if (fn_7100deabe4(g_unk_7101d47310))
        self->collider->SetTransform(fn_710002614c(transform));
    self->collider->vf_0x30(true);
    self->collider->vf_0x60(false);
    fn_7100d36d30(reinterpret_cast<UnkTriggerCollider_b0058*>(self->collider), shape);
    self->shape = shape;
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(self->collider), true);
}

// The timer at `index`, growing the array to cover it first (UnkVector::GrowAt, out of line).
__attribute__((flatten)) UnkScenarioTimer_ovb0097& fn_71007e45dc(base::UnkVector<UnkScenarioTimer_ovb0097>* self,
                                                                 u32 index) {
    return self->GrowAt(index);
}

// A small fixed array of string ids with its count in front (0x24 bytes, 4-byte aligned).
struct __attribute__((packed)) UnkScenarioIdList_ovb0097 {
    u32 count;
    CStrId ids[4];  // 0x4
};
static_assert(sizeof(UnkScenarioIdList_ovb0097) == 0x24, "UnkScenarioIdList_ovb0097 layout");

// Sets the count: new ids are the empty string, dropped ones are released.
void fn_71007e572c(UnkScenarioIdList_ovb0097* self, u32 n) {
    if (self->count < n) {
        for (u32 i = self->count; i < n; ++i) {
            CStrId* id = &self->ids[i];
            id->data = nullptr;
            if (!g_unk_7101d47108)
                base::global::UnkEnsureStringPool();
            id->data = &g_unk_7101d47108->empty;
        }
    } else if (self->count > n) {
        for (u64 i = n; i < self->count; ++i)
            self->ids[i].Reset();
    }
    self->count = n;
}
