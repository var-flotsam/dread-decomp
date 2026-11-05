// CCharClassEmmyValveComponent's unit (CEmmyValveComponent): requests on the component's state machine at
// +0x40 (last state 3, "none" 4), with the request template of src/game/UnkStateMachineSet_b60.cpp copied
// under this file's names. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkAnimationComponent.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkSmState_A5v_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A5v_b64* __attribute__((aligned(4))) UnkSmStatePtr_A5v_b64;

struct UnkSmMachine_A5v_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A5v_b64* previous, UnkSmState_A5v_b64* next);
    virtual void vf_0x28(UnkSmState_A5v_b64* current, UnkSmState_A5v_b64* next);

    u64 field_0x8;
    UnkSmState_A5v_b64* current;
    UnkSmState_A5v_b64* previous;
    u32 count;
    UnkSmStatePtr_A5v_b64 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A5v_b64 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A5v_b64 machine;
};

__attribute__((always_inline)) static inline bool FindState_A5v_b64(UnkSmMachine_A5v_b64* self, u32 id,
                                                               UnkSmState_A5v_b64*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A5v_b64);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_A5v_b64* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A5v_b64)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None>
__attribute__((always_inline)) static inline void SwitchState_A5v_b64(UnkSmMachine_A5v_b64* self, u32 id,
                                                                 UnkSmState_A5v_b64* current) {
    if (!current || current->id != id) {
        UnkSmState_A5v_b64* found;
        UnkSmState_A5v_b64* next;
        u32 enter;
        if (FindState_A5v_b64(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            asm("" : "+r"(enter));
            next = nullptr;
            asm("" : "+r"(next));
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_A5v_b64*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_A5v_b64(UnkSmMachine_A5v_b64* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A5v_b64<None>(self, id, self->current);
}

struct UnkSmBase_b60;
bool fn_71007b3318(UnkSmBase_b60*);
struct UnkEmmyRegistry_b55;
extern UnkEmmyRegistry_b55* g_unk_71726af0a8;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
extern UnkDesc_SpitSend g_unk_7172769794;  // reached through the GOT
extern UnkProperty g_unk_717271a210;  // "Disabled"
extern const base::global::CStrId g_unk_717271a2c0 __attribute__((visibility("hidden")));

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the blackboard
// is locked), as in src/game/CCharClassActivatableComponent/CCharClassActivatableComponent_b48.cpp.
static inline void SetSectionBool_A5_b64(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name,
                                         bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

struct UnkValveOwner_A5_b64 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkValveValue_A5_b64 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkEmmyValve_A5_b64 {
    u8 pad_0x0[0x30];
    UnkValveOwner_A5_b64* owner;
    u8 pad_0x38[0x40 - 0x38];
    UnkSmMachine_A5v_b64 machine;
    u8 pad_0xf0[0x240 - 0xf0];
    UnkAnimationComponent* field_0x240;
    u8 pad_0x248[0x268 - 0x248];
    UnkValveValue_A5_b64* values;
    u64* keys;
    u32 count;
    u8 pad_0x27c[0x290 - 0x27c];
    bool field_0x290;
};

__attribute__((always_inline)) static inline void Request_A5_b64(UnkEmmyValve_A5_b64* self, u32 id) {
    RequestState_A5v_b64<3, 4>(&self->machine, id);
}

void fn_71007b3740(UnkEmmyValve_A5_b64* self) {
    if (self->field_0x290)
        return;
    if (reinterpret_cast<u32*>(g_unk_71726af0a8)[2] == 0) {
        Request_A5_b64(self, 3);
        return;
    }
    if (!fn_71007b3318(reinterpret_cast<UnkSmBase_b60*>(self)))
        return;
    Request_A5_b64(self, 1);
}

void fn_71007b3db8(UnkEmmyValve_A5_b64* self, bool open, bool set) {
    if (set) {
        if (open)
            Request_A5_b64(self, 1);
        else
            Request_A5_b64(self, 2);
    }
    self->field_0x290 = set;
}

// Opens the valve: with E.M.M.I. entries registered, enables the owner's valve collision object, stores
// "Disabled" = false in the current scenario's blackboard section and requests state 2.
void fn_71007b3b84(UnkEmmyValve_A5_b64* self) {
    if (reinterpret_cast<u32*>(g_unk_71726af0a8)[2] == 0)
        return;
    if (UnkCollisionContainer* collision = self->owner->collision) {
        if (UnkCollisionObject* object = fn_71008640a4(collision, &g_unk_717271a2c0))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), true);
    }
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717271a210.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkValveValue_A5_b64* values = self->values;
    bool value = false;
    SetSectionBool_A5_b64(blackboard, section_id, &values[index].name, &value);
    Request_A5_b64(self, 2);
}

// Restores the valve from the blackboard: reads "Disabled" from the current scenario's section and
// requests state 3 when it is set, else state 2.
void fn_71007b2df0(UnkEmmyValve_A5_b64* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(g_unk_7101d47310->field_0x608));
    if (!section)
        return;
    bool disabled = false;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717271a210.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    fn_710006d11c(section, reinterpret_cast<const CStrId*>(&self->values[index].name), &disabled);
    if (disabled)
        Request_A5_b64(self, 3);
    else
        Request_A5_b64(self, 2);
}

// Closes the valve: the send of src/game/CCharClassEmmyValveComponent/CEmmyValveComponent_Send_b56.cpp
// (fn_71007b3050's hashes), then disables the owner's valve collision object and stores "Disabled" = true.
void fn_71007b3900(UnkEmmyValve_A5_b64* self) {
    // Declared ahead of a jump so that they get no lifetime markers: the original gives each its own slot.
    bool value;
    u32 mode;  // only the id word of the argument
    UnkTypedValue v;
    base::reflection::UnkReflTypeIdArg mask;
    goto start;
start:
    if (self->field_0x240 && !fn_710117ae38(self->field_0x240, 0xa0e33be014f44bd7, false) &&
        !fn_710117ae38(self->field_0x240, 0x6909d27d2c787fba, false)) {
        UnkAnimationComponent* target = self->field_0x240;
        mode = 0;
        UnkDesc_SpitSend* desc =
            fn_710117ae38(target, 0x6a17390fc912b900, false) ? &g_unk_7172769794 : &g_unk_7172769740;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0xa0e33be014f44bd7, 0xf4061ff25a2163d6, reinterpret_cast<base::reflection::UnkReflTypeIdArg*>(&mode), &g_unk_710164b958, desc, &mask);
    }
    if (UnkCollisionContainer* collision = self->owner->collision) {
        if (UnkCollisionObject* object = fn_71008640a4(collision, &g_unk_717271a2c0))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), false);
    }
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717271a210.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkValveValue_A5_b64* values = self->values;
    value = true;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8) {
            UnkPropertyName* name = &values[index].name;
            asm("" : "+r"(name));  // the original computes the name's address before the type getter
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, name, &v);
        }
    }
}
