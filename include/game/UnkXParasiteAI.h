#pragma once

// The X parasite AI component (reflected as CXParasiteAIComponent, 0x147c0 bytes; unit 1102 of
// tools/tu_map.py), as far as the hand-written sources know it: one layout, grown by each source that reads
// a member it does not have yet. Its base, CBehaviorTreeAIComponent, ends at 0x146c8 (tools/gen_fields.py);
// what follows are the component's own members, which reflection does not name. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/math/CVector4D.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkSlots.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The component's array of entries at +0x14740 (a base::UnkVector): counted weak handles, each to a holder
// that has the entry's actor.
struct UnkXParasiteEntryHolder {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
};

static inline UnkXParasiteEntryHolder* Holder(UnkWeakHandle* entry) {
    return reinterpret_cast<UnkXParasiteEntryHolder*>(entry->target);
}

// What the parasite has grabbed (the target of the weak handle at +0x14790), as the parts of it these sources
// touch: its listener component (told through vtable slot 0x410) and its partner.
struct UnkXParasitePartner {
    u8 pad_0x0[0x2bc];
    u32 field_0x2bc;
    u32 field_0x2c0;
};

struct UnkXParasitePartnerLink {
    u8 pad_0x0[0x10];
    UnkXParasitePartner* partner;  // 0x10
};

struct UnkXParasiteListener {
    void** vtable;
    void Notify(void* self) { reinterpret_cast<void (*)(UnkXParasiteListener*, void*)>(vtable[0x410 / 8])(this, self); }
};

struct UnkXParasiteGrabbed {
    u8 pad_0x0[0x240];
    UnkXParasiteListener* listener;  // 0x240
    u8 pad_0x248[0x150];
    UnkXParasitePartnerLink* partner_link;  // 0x398
};

static inline UnkXParasiteGrabbed* Grabbed(UnkWeakHandle* link) {
    return reinterpret_cast<UnkXParasiteGrabbed*>(link->target);
}

// The owner's model holder (the actor's +0x228) and its shape, as far as these sources reach.
struct UnkXParasiteShape {
    u8 pad_0x0[0x114];
    bool field_0x114;
    u8 pad_0x115[7];
    u8 field_0x11c;
    u8 pad_0x11d[0x138 - 0x11d];
    UnkSlots* slots;  // 0x138
};

struct UnkXParasiteHolder {
    u8 pad_0x0[0x50];
    UnkXParasiteShape* shape;  // 0x50
};

static inline UnkXParasiteHolder* ModelHolder(UnkActor* owner) {
    return static_cast<UnkXParasiteHolder*>(owner->field_0x228);
}

// What each state shows: an effect (the file path id at +0x10 that fn_71003b7db0 loads) and two colours for
// the shape's slots, in the table at +0x14768, keyed by the value at +0x14724.
struct UnkXParasiteBinding {
    u8 pad_0x0[0x10];
    base::UnkStrIdMemberOf<g_unk_7101d47200> effect;  // 0x10
    base::math::CVector4D first;   // 0x18
    base::math::CVector4D second;  // 0x28
};
static_assert(sizeof(UnkXParasiteBinding) == 0x38, "placeholder layout");

// The values and their keys are two arrays from one allocator.
struct UnkXParasiteBindings {
    UnkXParasiteBinding* values;
    u32* keys;
    u32 count;
    u32 field_0x14;
    u64 field_0x18;
    base::UnkAllocator* allocator;  // 0x20
};
static_assert(sizeof(UnkXParasiteBindings) == 0x28, "placeholder layout");

// The weights at +0x14728 of the four choices fn_71006e73f4 picks from at random, and the mask of those in use.
// A behaviour can override each weight (fn_71006ea210).
struct UnkXParasiteWeights {
    f32 weight_0;  // 0x0
    f32 weight_2;  // 0x4
    f32 weight_1;  // 0x8
    f32 weight_3;  // 0xc
    u32 mask;      // 0x10
};

struct UnkXParasiteAI {
    // CBehaviorTreeAIComponent and its bases: the members these sources touch
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x146d0 - 0x1350];
    // the component's own members
    UnkRefObject* field_0x146d0;
    u8 pad_0x146d8[8];
    s32 state;  // 0x146e0
    s32 field_0x146e4;
    bool field_0x146e8;  // mirrored into the blackboard and the owner's timeline (fn_71006ea3fc)
    bool field_0x146e9;
    bool field_0x146ea;
    bool field_0x146eb;
    bool field_0x146ec;
    u8 pad_0x146ed[3];
    u64 field_0x146f0;
    u32 field_0x146f8;
    u32 field_0x146fc;
    u64 field_0x14700;
    u32 field_0x14708;
    f32 field_0x1470c;
    u32 field_0x14710;
    f32 field_0x14714;
    f32 field_0x14718;
    f32 field_0x1471c;
    bool field_0x14720;
    u8 pad_0x14721[3];
    u32 field_0x14724;
    UnkXParasiteWeights weights;  // 0x14728
    u8 pad_0x1473c[4];
    base::UnkVector<UnkWeakHandle*> entries;  // 0x14740
    UnkXParasiteBindings bindings;            // 0x14768
    UnkWeakHandle* link;  // 0x14790: the grabbed object's weak handle
    UnkWeakHandle* field_0x14798;  // the prey's weak handle (fn_71006ea9a4)
    UnkRefObject* field_0x147a0;
    f32 field_0x147a8;
    u8 pad_0x147ac[4];
    UnkWeakHandle* field_0x147b0;  // what fn_71006e808c locks on to: a found entry's handle
    bool field_0x147b8;
    bool released;  // 0x147b9
    u8 pad_0x147ba[6];
};
static_assert(__builtin_offsetof(UnkXParasiteAI, blackboard) == 0x1260, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, state) == 0x146e0, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, field_0x146f0) == 0x146f0, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, field_0x14724) == 0x14724, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, entries) == 0x14740, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, bindings) == 0x14768, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, link) == 0x14790, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, released) == 0x147b9, "placeholder layout");
static_assert(sizeof(UnkXParasiteAI) == 0x147c0, "placeholder layout");

// The binding of the current state (one past the end when none).
static inline UnkXParasiteBinding* CurrentBinding(UnkXParasiteAI* self) {
    u64 i = 0;
    for (; i < self->bindings.count; ++i) {
        if (self->bindings.keys[i] == self->field_0x14724)
            break;
    }
    return &self->bindings.values[i];
}
