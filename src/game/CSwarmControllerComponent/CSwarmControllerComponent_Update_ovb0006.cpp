// CSwarmControllerComponent's unit (tools/tu_map.py unit 1163): the swarm controller's update
// (fn_710074a0d4: the base update, the melee-track flag, the per-group steps of fn_710074a200, the
// targets handed to vf_0x230, the release of the slots whose +0x10 is set, the delayed releases of
// fn_710074a360), the merge of one group into another once they are within 50 units
// (fn_710074c180), the removal of a group (fn_7100748234), two per-group visitors (fn_710074e190,
// fn_710074c684) and the growth of four arrays (fn_710074e938, fn_710074e82c, fn_710074ea40,
// fn_7100748064). The groups are the 0x148-byte entries at +0x50 of the object at +0x10 of the
// definition (+0xe8), counted at its +0x110. Names and layouts are placeholders.
#include <string.h>

#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::math::CVector3D;

extern "C" int rand();

struct UnkSwarmMember_ovb6;

struct UnkSwarmGroup_ovb6 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x23];
    u32 index;  // 0x48, the list (of fn_71007410a4) the group is in
    u8 pad_0x4c[0x1c];
    bool field_0x68;
    u8 pad_0x69[0x47];
    UnkEntry* entry;  // 0xb0
    UnkSwarmMember_ovb6** members;  // 0xb8
    u32 count;  // 0xc0
    u8 pad_0xc4[0x1c];
    u32 field_0xe0;
    u8 pad_0xe4[0x64];
};
static_assert(__builtin_offsetof(UnkSwarmGroup_ovb6, members) == 0xb8, "UnkSwarmGroup_ovb6 layout");
static_assert(sizeof(UnkSwarmGroup_ovb6) == 0x148, "UnkSwarmGroup_ovb6 layout");

struct UnkSwarmGroups_ovb6 {
    u8 pad_0x0[0x50];
    UnkSwarmGroup_ovb6* groups;  // 0x50
    u8 pad_0x58[0xb8];
    s32 count;  // 0x110

    // Group i, or null past the end (compared as 64 bits, or as 32).
    UnkSwarmGroup_ovb6* Get(s64 i) {
        if (i < count && groups)
            return &groups[i];
        return nullptr;
    }
    UnkSwarmGroup_ovb6* Get32(s32 i) {
        if (i < count && groups)
            return &groups[i];
        return nullptr;
    }
};
static_assert(__builtin_offsetof(UnkSwarmGroups_ovb6, count) == 0x110, "UnkSwarmGroups_ovb6 layout");

struct UnkSwarmDef_ovb6 {
    u8 pad_0x0[0x10];
    UnkSwarmGroups_ovb6* groups;  // 0x10
};

struct UnkSwarmTunables_ovb6 {
    u8 pad_0x0[0xd4];
    bool field_0xd4;
    u8 pad_0xd5[0x17];
    bool field_0xec;
};

struct UnkSwarmOwnerPart_ovb6 {
    virtual void vf_0x0();
};

struct UnkSwarmOwner_ovb6 {
    u8 pad_0x0[0x1b8];
    UnkSwarmOwnerPart_ovb6* field_0x1b8;
};

// A slot of the array at +0x278 (see fn_710074ea40).
struct UnkSwarmSlot_ovb6;

struct UnkSwarmSlots_ovb6 {
    UnkSwarmSlot_ovb6* slots;
    void** items;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// The layout of a base::UnkVector, freed when it goes out of scope.
struct UnkSwarmTargets_ovb6 {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkSwarmTargets_ovb6() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkSwarmController_ovb6 {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkSwarmOwner_ovb6* owner;  // 0x30
    u8 pad_0x38[0xb0];
    UnkSwarmDef_ovb6* def;  // 0xe8
    u8 pad_0xf0[0xa0];
    CVector3D field_0x190;
    u8 pad_0x19c[0xc];
    u8 group_lists[0x28];  // 0x1a8, the groups by list (fn_71007410a4)
    bool merging;  // 0x1d0
    s32 merge_from;  // 0x1d4
    s32 merge_into;  // 0x1d8
    f32 merge_time;  // 0x1dc
    UnkSwarmTunables_ovb6* tunables;  // 0x1e0
    u8 active_groups;  // 0x1e8
    u8 pad_0x1e9[0x13];
    bool field_0x1fc;
    u8 pad_0x1fd[0x5b];
    u32 field_0x258;
    u8 pad_0x25c[0x1c];
    UnkSwarmSlots_ovb6 slots;  // 0x278

    UnkSwarmGroups_ovb6* Groups() { return def ? def->groups : nullptr; }

    // Virtual slots of the controller, called through its vtable.
    typedef void (*StepFn)(UnkSwarmController_ovb6*, f32);
    typedef void (*GroupStepFn)(UnkSwarmController_ovb6*, f32, s32);
    typedef bool (*AcceptsFn)(UnkSwarmController_ovb6*, UnkSwarmMember_ovb6*, void*);
    typedef void (*TargetsFn)(UnkSwarmController_ovb6*, f32, UnkSwarmTargets_ovb6*);
    template <class Fn>
    Fn Slot(u32 offset) {
        return (*reinterpret_cast<Fn**>(this))[offset / 8];
    }
    void Step(u32 offset, f32 dt) { Slot<StepFn>(offset)(this, dt); }
    void GroupStep(u32 offset, s32 index, f32 dt) { Slot<GroupStepFn>(offset)(this, dt, index); }
};
static_assert(__builtin_offsetof(UnkSwarmController_ovb6, def) == 0xe8, "UnkSwarmController_ovb6 layout");
static_assert(__builtin_offsetof(UnkSwarmController_ovb6, merging) == 0x1d0, "UnkSwarmController_ovb6 layout");
static_assert(__builtin_offsetof(UnkSwarmController_ovb6, field_0x1fc) == 0x1fc, "UnkSwarmController_ovb6 layout");
static_assert(__builtin_offsetof(UnkSwarmController_ovb6, field_0x258) == 0x258, "UnkSwarmController_ovb6 layout");
static_assert(__builtin_offsetof(UnkSwarmController_ovb6, slots) == 0x278, "UnkSwarmController_ovb6 layout");

struct UnkSwarmMember_ovb6 {
    u8 pad_0x0[0x64];
    f32 field_0x64;
};

struct UnkGroup_fs;
struct UnkArray_fs;
UnkGroup_fs* fn_71007410a4(UnkArray_fs* array, u32 index);
struct UnkSwarmController_A5_b68;
void fn_710074bdd4(UnkSwarmController_A5_b68* self, u32 index, f32 delay);
void fn_7100232c68(void*);

// The list a group is in (the shape of fn_71007410a4's result).
struct UnkSwarmGroupList_ovb6 {
    UnkSwarmGroup_ovb6** data;
    u32 count;
};

// Removes a group from its list (handing an emptied list to fn_710074bdd4 with 1.5), clears it and,
// when it was the last active one, stops the owner.
void fn_7100748234(UnkSwarmController_ovb6* self, UnkSwarmGroup_ovb6* group) {
    if (!group || !group->active)
        return;
    u32 index = group->index;
    UnkSwarmGroupList_ovb6* list = reinterpret_cast<UnkSwarmGroupList_ovb6*>(
        fn_71007410a4(reinterpret_cast<UnkArray_fs*>(self->group_lists), index));
    u32 count = list->count;
    UnkSwarmGroup_ovb6** data = list->data;
    u64 i = 0;
    for (; i != count; ++i) {
        if (data[i] == group)
            break;
    }
    if ((u32)i < count) {
        __builtin_memcpy(&data[i], &data[i] + 1, (u64)(count - (u32)i - 1) * sizeof(*data));
        count = list->count - 1;
        list->count = count;
    }
    if (count == 0)
        fn_710074bdd4(reinterpret_cast<UnkSwarmController_A5_b68*>(self), index, 1.5f);
    group->active = false;
    if (group->entry)
        fn_7100d35a98(group->entry, false);
    if (--self->active_groups == 0) {
        if (UnkSwarmOwnerPart_ovb6* part = self->owner->field_0x1b8) {
            typedef void (*Fn)(UnkSwarmOwnerPart_ovb6*, s32, s32);
            (*reinterpret_cast<Fn**>(part))[0x180 / 8](part, 0, 1);
        }
        fn_7100232c68(self->owner);
    }
    self->field_0x258 = 0;
    self->field_0x190.x = g_unk_710164779c.x;
    self->field_0x190.y = g_unk_710164779c.y;
    self->field_0x190.z = g_unk_710164779c.z;
}

struct UnkObj_710072a110;

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3_710072a110 {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3_710072a110(const UnkVec3_710072a110& o) : x(o.x), y(o.y), z(o.z) {}
};

UnkVec3_710072a110 fn_7100745e24(UnkObj_710072a110* obj);
s32 fn_7100729654(UnkSwarmGroups_ovb6* groups, UnkSwarmGroup_ovb6* from, UnkSwarmGroup_ovb6* into);

// The group's position.
static inline UnkVec3_710072a110 Center_ovb6(UnkSwarmGroup_ovb6* group) {
    return fn_7100745e24(reinterpret_cast<UnkObj_710072a110*>(group));
}

// A pending merge (from +0x1d4 into +0x1d8, for at most 3 time units): once both groups are active
// and closer than 50, fn_7100729654 merges them and an emptied source group is removed.
void fn_710074c180(UnkSwarmController_ovb6* self, f32 dt) {
    if (!self->merging)
        return;
    self->merge_time += dt;
    if (self->merge_time > 3.0f)
        self->merging = false;
    UnkSwarmGroup_ovb6* from = self->Groups()->Get32(self->merge_from);
    UnkSwarmGroup_ovb6* into = self->def->groups->Get32(self->merge_into);
    if (into && from && from->active && into->active) {
        UnkVec3_710072a110 a = Center_ovb6(from);
        UnkVec3_710072a110 b = Center_ovb6(into);
        f32 dx = b.x - a.x;
        f32 dy = b.y - a.y;
        f32 dz = b.z - a.z;
        if (!(dx * dx + dy * dy + dz * dz < 2500.0f))
            return;
        if (fn_7100729654(self->Groups(), from, into) && from->field_0xe0 == 0)
            fn_7100748234(self, from);
    }
    self->merging = false;
}

void fn_710074bf18(UnkSwarmController_ovb6* self, s32 index, f32 dt);

// The per-frame steps: the merge, vf_0x1f0 and vf_0x180, then for every active group vf_0x168,
// vf_0x170, fn_710074bf18 (when the tunables' +0xd4 is set) and vf_0x178 unless its +0x68 is set,
// and vf_0x1e8.
void fn_710074a200(UnkSwarmController_ovb6* self, f32 dt) {
    if (!self->def || !self->def->groups)
        return;
    fn_710074c180(self, dt);
    self->Step(0x1f0, dt);
    self->Step(0x180, dt);
    for (s32 i = 0; i < self->def->groups->count; ++i) {
        UnkSwarmGroup_ovb6* group = self->Groups()->Get(i);
        if (!group->active)
            continue;
        if (!group->field_0x68) {
            self->GroupStep(0x168, i, dt);
            self->GroupStep(0x170, i, dt);
            if (self->tunables->field_0xd4)
                fn_710074bf18(self, i, dt);
            self->GroupStep(0x178, i, dt);
        }
        self->GroupStep(0x1e8, i, dt);
    }
}

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
char* fn_71010c4234(char*);

struct UnkSwarmPlayer_ovb6 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkSwarmSlot_ovb6 {
    u8 pad_0x0[0x10];
    void* field_0x10;
    u8 pad_0x18[0x28];
};
static_assert(sizeof(UnkSwarmSlot_ovb6) == 0x40, "UnkSwarmSlot_ovb6 layout");

UnkSwarmTargets_ovb6 fn_710074ab2c(UnkSwarmController_ovb6* self);
UnkSwarmSlot_ovb6* fn_710074a568(UnkSwarmSlots_ovb6* self, UnkSwarmSlot_ovb6* slot);
void fn_710074a360(UnkSwarmController_ovb6* self, f32 dt);

void fn_710074a0d4(UnkSwarmController_ovb6* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    UnkSwarmPlayer_ovb6* player = reinterpret_cast<UnkSwarmPlayer_ovb6*>(fn_7100dec788(g_unk_7101d47310));
    if (UnkAnimationComponent* animation = player->animation) {
        if (!fn_710117b348(animation, fn_7100d1ab58()))  // CCharClassMeleeTrack
            self->field_0x1fc = true;
    }
    fn_710074a200(self, dt);
    {
        UnkSwarmTargets_ovb6 targets = fn_710074ab2c(self);
        self->Slot<UnkSwarmController_ovb6::TargetsFn>(0x230)(self, dt, &targets);
    }
    if (self->slots.count != 0) {
        // An unused array (only its default allocator is fetched).
        base::UnkVector<void*> unused;
        UnkSwarmSlot_ovb6* it = self->slots.slots;
        while (self->slots.slots + self->slots.count != it) {
            if (it->field_0x10)
                it = fn_710074a568(&self->slots, it);
            else
                ++it;
        }
    }
    fn_710074a360(self, dt);
}

bool fn_7100744e1c(UnkSwarmMember_ovb6* member);
struct UnkTarget;
void fn_7100745440(UnkTarget* target, void* effect, u32 flag);

// Every live member that vf_0x228 accepts for `arg` gets fn_7100745440 and `value` at +0x64.
void fn_710074e190(void* controller, void* arg, f32 value) {
    UnkSwarmController_ovb6* self = static_cast<UnkSwarmController_ovb6*>(controller);
    for (s32 i = 0; i < self->def->groups->count; ++i) {
        UnkSwarmGroup_ovb6* group = self->def->groups->Get(i);
        for (u32 j = 0; j < group->count; ++j) {
            UnkSwarmMember_ovb6* member = group->members[j];
            if (member && fn_7100744e1c(member) &&
                self->Slot<UnkSwarmController_ovb6::AcceptsFn>(0x228)(self, member, arg)) {
                fn_7100745440(reinterpret_cast<UnkTarget*>(member), arg, 0);
                member->field_0x64 = value;
            }
        }
    }
}

s32 fn_710074d95c(UnkSwarmController_ovb6* self, UnkSwarmGroup_ovb6* group, void* arg, f32 a, f32 b, f32 c, f32 d,
                  s32 first, s32 last, bool flag, void* extra);

struct UnkSwarmUpdate_b65a4;

// When the tunables' +0xec is set, runs fn_710074d95c on every active group with a random value in
// [150, 250], numbering on from the total it has returned so far.
void fn_710074c684(UnkSwarmUpdate_b65a4* controller, void* arg) {
    UnkSwarmController_ovb6* self = reinterpret_cast<UnkSwarmController_ovb6*>(controller);
    if (!self->tunables->field_0xec)
        return;
    s32 total = 0;
    for (s32 i = 0; i < self->def->groups->count; ++i) {
        UnkSwarmGroup_ovb6* group = self->def->groups->Get(i);
        if (!group->active)
            continue;
        f32 r = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
        f32 a = r * 100.0f + -50.0f + 200.0f;
        total += fn_710074d95c(self, group, arg, a, 0.0f, 250.0f, 400.0f, -total, -total - 1, true, nullptr);
    }
}

// Two parallel arrays, the first of words and the second of word pairs (the growth step and the
// allocation flags of base::UnkVector, then its allocator).
struct UnkSwarmPair_ovb6 {
    u32 a;
    u32 b;
};

struct UnkSwarmWordsAndPairs_ovb6 {
    u32* words;
    UnkSwarmPair_ovb6* pairs;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Reserves room for n entries in both arrays.
void fn_710074e938(UnkSwarmWordsAndPairs_ovb6* self, u32 n) {
    if (self->capacity >= n)
        return;
    u32* words = static_cast<u32*>(self->allocator->vf_0x40((u64)n * sizeof(u32), 8, self->flags));
    UnkSwarmPair_ovb6* pairs =
        static_cast<UnkSwarmPair_ovb6*>(self->allocator->vf_0x40((u64)n * sizeof(UnkSwarmPair_ovb6), 8, self->flags));
    if (self->words) {
        memcpy(words, self->words, (u64)self->count * sizeof(u32));
        self->allocator->vf_0x48(self->words);
    }
    if (UnkSwarmPair_ovb6* old = self->pairs) {
        UnkSwarmPair_ovb6* dst = pairs;
        for (u32 i = 0; i < self->count; ++i)
            *dst++ = *old++;
        self->allocator->vf_0x48(self->pairs);
    }
    self->words = words;
    self->pairs = pairs;
    self->capacity = n;
}

// The same with two arrays of pairs.
struct UnkSwarmPairs2_ovb6 {
    UnkSwarmPair_ovb6* first;
    UnkSwarmPair_ovb6* second;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710074e82c(UnkSwarmPairs2_ovb6* self, u32 n) {
    if (self->capacity >= n)
        return;
    UnkSwarmPair_ovb6* first =
        static_cast<UnkSwarmPair_ovb6*>(self->allocator->vf_0x40((u64)n * sizeof(UnkSwarmPair_ovb6), 8, self->flags));
    UnkSwarmPair_ovb6* second =
        static_cast<UnkSwarmPair_ovb6*>(self->allocator->vf_0x40((u64)n * sizeof(UnkSwarmPair_ovb6), 8, self->flags));
    if (UnkSwarmPair_ovb6* old = self->first) {
        UnkSwarmPair_ovb6* dst = first;
        for (u32 i = 0; i < self->count; ++i)
            *dst++ = *old++;
        self->allocator->vf_0x48(self->first);
    }
    if (UnkSwarmPair_ovb6* old = self->second) {
        UnkSwarmPair_ovb6* dst = second;
        for (u32 i = 0; i < self->count; ++i)
            *dst++ = *old++;
        self->allocator->vf_0x48(self->second);
    }
    self->first = first;
    self->second = second;
    self->capacity = n;
}

struct UnkShotWeakTarget;

// The layout of UnkShotWeakRef in src/game/CShotManager.cpp: a weak reference, empty when its
// generation is -1. Its fields are written untyped (the original reloads the array after each new
// element) and it is not trivially copyable (the original copies element by element).
struct UnkShotWeakRef {
    UnkShotWeakTarget* target;
    s64 generation;

    UnkShotWeakRef() {
        UnkShotWeakTarget* none = nullptr;
        s64 invalid = -1;
        __builtin_memcpy(&target, &none, sizeof(none));
        __builtin_memcpy(&generation, &invalid, sizeof(invalid));
    }
    ~UnkShotWeakRef() {}
};

// The weak reference at index i, growing the array (with empty references) to reach it.
UnkShotWeakRef* fn_7100748064(base::UnkVector<UnkShotWeakRef>* self, u32 i) {
    return &self->GrowAt(i);
}
