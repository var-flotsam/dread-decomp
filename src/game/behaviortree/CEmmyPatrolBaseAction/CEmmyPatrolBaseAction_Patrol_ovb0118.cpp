// behaviortree::CEmmyPatrolBaseAction's unit (tools/tu_map.py). fn_710051564c: hands the owner's
// CEmmyAIComponent (+0x240) the action's patrol callbacks (fn_7100516468, fn_7100516474) through
// fn_7100556c00, first aiming its patrol (+0x146d0, fn_71005554a0) at the owner's position when the
// patrol's object at +0x48 is not already set (its slot 0x18). fn_7100515db8 resets the action and the
// AI's patrol list (as fn_7100515cc8 in CEmmyPatrolBaseAction_Start_b55.cpp) and calls it with 0;
// fn_71005154dc does the same after the base start (fn_710051dae0), also registering the action
// (+0xf8) in the AI's deferred set at +0x29f90 with the callback fn_710051645c at +0x14760. Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU811.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "game/behaviortree/CEmmyPatrolBaseAction.h"
#include "types.h"

struct UnkNodeAllocator_ovb0118 {
    void** vtable;
    void Free(void* node) { reinterpret_cast<void (*)(UnkNodeAllocator_ovb0118*, void*)>(vtable[9])(this, node); }
};

template <class Node>
struct UnkNodeList_ovb0118 {
    u32 count;
    u8 pad_0x4[4];
    Node* head;
    Node* tail;
    UnkNodeAllocator_ovb0118* allocator;

    // `ends` is &head, passed in so a caller can pin its address.
    __attribute__((always_inline)) void Clear(Node** ends) {
        for (Node* node = ends[0]; node;) {
            Node* next = node->next;
            allocator->Free(node);
            node = next;
        }
        count = 0;
        ends[0] = nullptr;
        ends[1] = nullptr;
    }
    __attribute__((always_inline)) void Clear() { Clear(&head); }
};

struct UnkPatrolNode_ovb0118 {
    u8 pad_0x0[8];
    UnkPatrolNode_ovb0118* next;
};

struct UnkEmmyNode_ovb0118 {
    u8 pad_0x0[0x20];
    UnkEmmyNode_ovb0118* next;
};

// An object of the AI component's patrol (+0x14718).
struct UnkEmmyGoal_ovb0118 {
    void** vtable;
    u32 vf_0x18() { return reinterpret_cast<u32 (*)(UnkEmmyGoal_ovb0118*)>(vtable[3])(this); }
};

// The AI component's patrol (+0x146d0).
struct UnkEmmyPatrol_ovb0118 {
    u8 pad_0x0[0x48];
    UnkEmmyGoal_ovb0118 goal;  // 0x48
    u8 pad_0x50[0x8];
    UnkNodeList_ovb0118<UnkEmmyNode_ovb0118> list;  // 0x58
};

struct UnkPatrolBase_ovb0118;

// A callback bound to the action: the object and the function called with it.
struct UnkPatrolCallback_ovb0118 {
    UnkPatrolBase_ovb0118* self;
    void (*fn)(UnkPatrolBase_ovb0118*);
};

struct UnkEmmyAI_ovb0118 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x146c8];
    UnkEmmyPatrol_ovb0118 patrol;  // 0x146d0
    u8 pad_0x14748[0x18];
    UnkPatrolCallback_ovb0118 on_patrol;  // 0x14760
    u8 pad_0x14770[0x15820];
    u8 registrations[0x10];  // 0x29f90: a deferred set
};
static_assert(__builtin_offsetof(UnkEmmyAI_ovb0118, registrations) == 0x29f90, "UnkEmmyAI_ovb0118 layout");

// The action's entry in the AI component's deferred set.
struct UnkPatrolRegistration_ovb0118 {
    void* set;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkPatrolOwner_ovb0118 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1ac];
    UnkEmmyAI_ovb0118* ai;  // 0x240
};

struct UnkPatrolBase_ovb0118 {
    u8 pad_0x0[0x18];
    UnkPatrolOwner_ovb0118* owner;  // 0x18
    u8 pad_0x20[0xc];
    s32 field_0x2c;
    u8 pad_0x30[8];
    UnkNodeList_ovb0118<UnkPatrolNode_ovb0118> list;  // 0x38
    u8 pad_0x58[0x50];
    u32 field_0xa8;
    u8 pad_0xac[0x24];
    u32 field_0xd0;
    u8 pad_0xd4[0x1c];
    u32 steps;  // 0xf0
    u8 pad_0xf4[0x4];
    UnkPatrolRegistration_ovb0118 registration;  // 0xf8
    u8 pad_0x110[0x8];
    u32 field_0x118;
    bool field_0x11c;
};

// fn_710051564c takes the action as src/game/behaviortree/CEmmyPatrolBaseAction/CEmmyPatrolBaseAction_Start_b55.cpp
// declares it; it is defined with its result (the declarations elsewhere drop it).
struct UnkPatrolBase_b55;

struct UnkEmmyAI_b0068;
struct UnkEmmySearchCallback_b0068;
void fn_71005554a0(UnkEmmyPatrol_ovb0118* patrol, const base::math::CVector2D* target, bool flag);
void fn_7100516468(UnkPatrolBase_ovb0118* self);
void fn_7100516474(UnkPatrolBase_ovb0118* self);

static inline UnkEmmyAI_ovb0118* EmmyAI(UnkPatrolBase_ovb0118* self) {
    UnkEmmyAI_ovb0118* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        return ai;
    return nullptr;
}

bool fn_710051564c(UnkPatrolBase_b55* base, bool flag) {
    UnkPatrolBase_ovb0118* self = reinterpret_cast<UnkPatrolBase_ovb0118*>(base);
    UnkEmmyAI_ovb0118* ai = EmmyAI(self);
    if (!ai)
        return false;
    bool moving;
    if (ai->patrol.goal.vf_0x18()) {
        moving = false;
    } else {
        UnkVec3f position = self->owner->xform.GetTranslation();
        base::math::CVector2D target;
        fn_710007b430(&target, reinterpret_cast<const base::math::CVector3D*>(&position));
        fn_71005554a0(&ai->patrol, &target, flag);
        moving = true;
    }
    UnkPatrolCallback_ovb0118 a = {self, fn_7100516468};
    UnkPatrolCallback_ovb0118 b = {self, fn_7100516474};
    UnkPatrolCallback_ovb0118 none = {nullptr, nullptr};
    // fn_7100556c00 returns a flag here (declared elsewhere without it).
    using Start = bool (*)(UnkEmmyAI_ovb0118*, bool, const UnkPatrolCallback_ovb0118*, const UnkPatrolCallback_ovb0118*,
                           u32, void*, const UnkPatrolCallback_ovb0118&);
    return reinterpret_cast<Start>(fn_7100556c00)(ai, moving, &a, &b, 1, nullptr, none);
}

// Empties the action's node list and the AI component's patrol list.
static inline __attribute__((always_inline)) void ClearPatrol(UnkPatrolBase_ovb0118* self) {
    self->list.Clear();
    self->field_0x2c = -1;
    self->field_0xa8 = 0;
    self->field_0xd0 = 0;
    if (UnkEmmyAI_ovb0118* ai = EmmyAI(self)) {
        UnkEmmyPatrol_ovb0118* patrol = &ai->patrol;
        UnkEmmyNode_ovb0118** ends = &ai->patrol.list.head;
        asm("" : "+r"(patrol), "+r"(ends));
        patrol->list.Clear(ends);
    }
}

// Removes the action's registration from its deferred set (base::UnkDeferredSet<..>::Remove).
static inline void Unregister(UnkPatrolRegistration_ovb0118* reg) {
    if (reg->set) {
        fn_7100514b7c(reg->set, reg);
        reg->set = nullptr;
    }
}

void fn_7100515db8(UnkPatrolBase_ovb0118* self) {
    if (!EmmyAI(self))
        return;
    self->field_0x11c = false;
    ClearPatrol(self);
    fn_710051564c(reinterpret_cast<UnkPatrolBase_b55*>(self), false);
    self->steps = 0;
}

void fn_710051dae0(UnkPatrolBase_ovb0118* self);
void fn_71005162a8(void* set, UnkPatrolRegistration_ovb0118* reg);
void fn_710051645c(UnkPatrolBase_ovb0118* self, u64 a, u64 b);  // passes both on to the object's slot 0xb8

void fn_71005154dc(UnkPatrolBase_ovb0118* self) {
    fn_710051dae0(self);
    self->field_0x118 = 0;
    self->field_0x11c = false;
    ClearPatrol(self);
    if (UnkEmmyAI_ovb0118* ai = EmmyAI(self)) {
        UnkPatrolRegistration_ovb0118* reg = &self->registration;
        UnkPatrolCallback_ovb0118* on_patrol = &ai->on_patrol;
        u8* set = ai->registrations;
        Unregister(reg);
        reg->set = set;
        fn_71005162a8(set, reg);
        *on_patrol = {self, reinterpret_cast<void (*)(UnkPatrolBase_ovb0118*)>(&fn_710051645c)};
    }
    fn_710051564c(reinterpret_cast<UnkPatrolBase_b55*>(self), false);
}
