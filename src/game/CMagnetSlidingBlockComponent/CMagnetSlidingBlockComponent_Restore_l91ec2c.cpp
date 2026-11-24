// CMagnetSlidingBlockComponent's unit (tools/tu_map.py 1412): restores the block from the blackboard. Fills the
// property map under the owner's name; when the "used" property is stored in the current scenario's section,
// the rail's glow goes to full, the machine moves to state 4 and a key joins the key list. Names and layouts
// are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/CMagnetSlidingBlockComponent.h"
#include "game/CMagnetSlidingBlockRailComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflRegistryMap;

struct UnkRefl_fn_710091ef38;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_710091ef38>(
    base::reflection::UnkReflRegistryMap*, const base::global::CStrId&);

struct UnkMagnetSlidingBlockRail_ovb0097;
// The rail component of an object.

struct UnkMagnetSlidingBlock_b0086;

struct UnkMagnetState_l91ec2c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkMagnetState_l91ec2c* __attribute__((aligned(4))) UnkMagnetStatePtr_l91ec2c;

struct UnkMagnetMachine_l91ec2c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMagnetState_l91ec2c* previous, UnkMagnetState_l91ec2c* next);
    virtual void vf_0x28(UnkMagnetState_l91ec2c* current, UnkMagnetState_l91ec2c* next);

    u64 field_0x8;
    UnkMagnetState_l91ec2c* current;
    UnkMagnetState_l91ec2c* previous;
    u32 count;
    UnkMagnetStatePtr_l91ec2c states[16];
    u32 pending;
    bool busy;

    // Switches to state `id` unless the machine is busy; `none` is the id reported when there was no state.
    void Request(u32 id, u32 none) {
        pending = id;
        if (busy)
            return;
        UnkMagnetState_l91ec2c* from = current;
        if (!from || from->id != id) {
            if (UnkMagnetState_l91ec2c* next = FindStateById(states, count, id)) {
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
struct UnkMagnetRailRef_l91ec2c : base::UnkRefCounted {
    void* object;  // 0x10
};

struct UnkMagnetOwnerInfo_l91ec2c {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkMagnetSlidingBlock_l91ec2c {
    u8 pad_0x0[0x18];
    UnkMagnetOwnerInfo_l91ec2c* info;  // 0x18
    u8 pad_0x20[0x280 - 0x20];
    UnkMagnetMachine_l91ec2c machine;  // 0x280
    u8 pad_0x330[0x348 - 0x330];
    u32 field_0x348;
    u8 pad_0x34c[0x350 - 0x34c];
    f32 glow;  // 0x350
    u8 pad_0x354[0x358 - 0x354];
    UnkGuiKeyList keys;  // 0x358
    u8 pad_0x388[0x430 - 0x388];
    u8 rail_source[0x28];  // 0x430
    s32 rail_state;        // 0x458
    u8 pad_0x45c[0x4];
    base::UnkRefHandle rail;          // 0x460
    UnkReflRegistryMap properties;  // 0x468
};
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91ec2c, machine) == 0x280, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91ec2c, machine.pending) == 0x324, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91ec2c, keys) == 0x358, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91ec2c, rail) == 0x460, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91ec2c, properties) == 0x468, "layout");

// The rail's object, the handle to it fetched on first use.
static inline void* RailObject(UnkMagnetSlidingBlock_l91ec2c* self) {
    if (self->rail_state == 1) {
        {
            base::UnkRefPtrMember fetched = fn_7100353be8(self->rail_source);
            fn_710031d208(&self->rail, reinterpret_cast<const base::UnkRefHandle*>(&fetched));
        }
        self->rail_state = 2;
    }
    if (base::UnkRefCounted* ref = self->rail.ptr)
        return static_cast<UnkMagnetRailRef_l91ec2c*>(ref)->object;
    return nullptr;
}

// Adds `key` to the key list; when the list's state changes, the block is told the negated new state.
static inline void AddKey(UnkMagnetSlidingBlock_l91ec2c* self, CStrId* key) {
    UnkGuiKeyList* list = &self->keys;
    bool before = fn_710002fb70(list);
    fn_710002fd10(list, key);
    if (before != fn_710002fb70(list))
        CMagnetSlidingBlockComponent_SetCollidersAndSpeed(reinterpret_cast<UnkMagnetSlidingBlock_b0086*>(self), !fn_710002fb70(list));
}

// The name of `property` in the map's values (the value past the last when the map lacks it).
static inline const CStrId* FindName(UnkReflRegistryMap* map, const UnkProperty& property) {
    u64 index = 0;
    u32 count = map->count;
    if (count) {
        CStrId* keys = map->keys;
        for (; index != count; ++index) {
            if (keys[index].data == property.name.data)
                break;
        }
    }
    return reinterpret_cast<const CStrId*>(&map->values[index]);
}

// Reads the bool stored under `name` in the blackboard section `section` names; false when there is none.
static inline bool GetSectionBool(CBlackboard* blackboard, const CStrId& section, const CStrId* name, bool* value) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        return fn_710006d11c(s, name, value);
    return false;
}

// Fills the property map from the registry under `name`.
static inline void InitProperties(UnkReflRegistryMap* properties, CStrId name) {
    base::reflection::refl_RegistryMapInit<UnkRefl_fn_710091ef38>(properties, name);
}

void CMagnetSlidingBlockComponent_RestoreFinishedMovementFromBlackboard(UnkMagnetSlidingBlock_l91ec2c* self, CBlackboard* blackboard) {
    InitProperties(&self->properties, self->info->name);

    bool used = false;
    if (!GetSectionBool(blackboard, fn_7100de9c30(g_unk_7101d47310), FindName(&self->properties, g_unk_7172737530),
                        &used))
        return;

    self->glow = 1.0f;
    self->field_0x348 = 0;
    if (void* rail = RailObject(self))
        CMagnetSlidingBlockRailComponent_SetMaterialColorGreen(fn_710091f6bc(rail), self->glow);
    self->machine.Request(4, 6);
    AddKey(self, &g_unk_71727375d8);
}
