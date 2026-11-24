// CMagnetSlidingBlockComponent's unit (tools/tu_map.py 1412): the step after a slide state ends. From state 2 the
// machine goes to 1; from 4, or from 3 once the glow has reached its target (or +0x340 is set), a used-up block
// adds its key and (from 4) darkens its rail, otherwise the machine goes to 5. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CMagnetSlidingBlockComponent.h"
#include "game/CMagnetSlidingBlockRailComponent.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU39.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::global::CStrId;

struct UnkMagnetSlidingBlockRail_ovb0097;
// The rail component of an object.

struct UnkMagnetSlidingBlock_b0086;

struct UnkMagnetState_l91f3a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkMagnetState_l91f3a8* __attribute__((aligned(4))) UnkMagnetStatePtr_l91f3a8;

struct UnkMagnetMachine_l91f3a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMagnetState_l91f3a8* previous, UnkMagnetState_l91f3a8* next);
    virtual void vf_0x28(UnkMagnetState_l91f3a8* current, UnkMagnetState_l91f3a8* next);

    u64 field_0x8;
    UnkMagnetState_l91f3a8* current;
    UnkMagnetState_l91f3a8* previous;
    u32 count;
    UnkMagnetStatePtr_l91f3a8 states[16];
    u32 pending;
    bool busy;

    // Switches to state `id` unless the machine is busy; `none` is the id reported when there was no state.
    void Request(u32 id, u32 none) {
        pending = id;
        if (busy)
            return;
        UnkMagnetState_l91f3a8* from = current;
        if (!from || from->id != id) {
            if (UnkMagnetState_l91f3a8* next = FindStateById(states, count, id)) {
                if (from) {
                    from->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : none);
                vf_0x20(previous, next);
            }
        }
        pending = none;
    }
};

// What the rail handle refers to: a counted reference to the rail's object.
struct UnkMagnetRailRef_l91f3a8 : base::UnkRefCounted {
    void* object;  // 0x10
};

struct UnkMagnetBlockFlags_l91f3a8 {
    u8 pad_0x0[0x58];
    f32 glow_target;  // 0x58
    bool used_up;     // 0x5c
};

struct UnkMagnetSlidingBlock_l91f3a8 {
    u8 pad_0x0[0x40];
    UnkMagnetBlockFlags_l91f3a8* flags;  // 0x40
    bool field_0x48;
    u8 pad_0x49[0x280 - 0x49];
    UnkMagnetMachine_l91f3a8 machine;  // 0x280
    u8 pad_0x330[0x340 - 0x330];
    bool field_0x340;
    u8 pad_0x341[0x350 - 0x341];
    f32 glow;  // 0x350
    u8 pad_0x354[0x358 - 0x354];
    UnkGuiKeyList keys;  // 0x358
    u8 pad_0x388[0x430 - 0x388];
    u8 rail_source[0x28];  // 0x430
    s32 rail_state;        // 0x458
    u8 pad_0x45c[0x4];
    base::UnkRefHandle rail;  // 0x460
};
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91f3a8, machine) == 0x280, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91f3a8, machine.pending) == 0x324, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91f3a8, field_0x340) == 0x340, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91f3a8, keys) == 0x358, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91f3a8, rail) == 0x460, "layout");

// The rail's object, the handle to it fetched on first use.
static inline void* RailObject(UnkMagnetSlidingBlock_l91f3a8* self) {
    if (self->rail_state == 1) {
        {
            base::UnkRefPtrMember fetched = fn_7100353be8(self->rail_source);
            fn_710031d208(&self->rail, reinterpret_cast<const base::UnkRefHandle*>(&fetched));
        }
        self->rail_state = 2;
    }
    if (base::UnkRefCounted* ref = self->rail.ptr)
        return static_cast<UnkMagnetRailRef_l91f3a8*>(ref)->object;
    return nullptr;
}

// Adds `key` to the key list; when the list's state changes, the block is told the negated new state.
static inline void AddKey(UnkMagnetSlidingBlock_l91f3a8* self, CStrId* key) {
    UnkGuiKeyList* list = &self->keys;
    bool before = fn_710002fb70(list);
    fn_710002fd10(list, key);
    if (before != fn_710002fb70(list))
        CMagnetSlidingBlockComponent_SetCollidersAndSpeed(reinterpret_cast<UnkMagnetSlidingBlock_b0086*>(self), !fn_710002fb70(list));
}

void fn_710091f3a8(UnkMagnetSlidingBlock_l91f3a8* self) {
    if (UnkMagnetState_l91f3a8* state = self->machine.current) {
        switch (state->id) {
        case 2:
            self->machine.Request(1, 6);
            break;
        case 3:
            if (!self->field_0x340 && self->glow < self->flags->glow_target)
                self->machine.Request(5, 6);
            else if (self->flags->used_up)
                AddKey(self, &g_unk_71727375d8);
            break;
        case 4:
            if (self->flags->used_up) {
                AddKey(self, &g_unk_71727375d8);
                if (RailObject(self))
                    CMagnetSlidingBlockRailComponent_SetMaterialOnState(fn_710091f6bc(RailObject(self)), false);
            } else {
                self->machine.Request(5, 6);
            }
            break;
        }
    }
    self->field_0x48 = false;
}
