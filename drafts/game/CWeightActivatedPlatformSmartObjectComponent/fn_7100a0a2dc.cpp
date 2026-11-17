// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): fn_7100a0a2dc returns the
// platform's state machine (+0x180) to its "none" state 6, clears the "Finished" flag in the current scenario's blackboard
// section when it is stored there, and makes the linked usable component unusable. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkWapState_la0a2dc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previous);
    virtual void Leave(s32 next);
    u64 field_0x8;
    u32 id;
};

// The machine keeps its state pointers 4-aligned, right after the count.
typedef UnkWapState_la0a2dc* __attribute__((aligned(4))) UnkWapStatePtr_la0a2dc;

struct UnkWapMachine_la0a2dc {
    static const u32 kLast = 7;
    static const u32 kNone = 6;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEnter(UnkWapState_la0a2dc* previous, UnkWapState_la0a2dc* next);
    virtual void OnLeave(UnkWapState_la0a2dc* current, UnkWapState_la0a2dc* next);

    u64 field_0x8;
    UnkWapState_la0a2dc* current;
    UnkWapState_la0a2dc* previous;
    u32 count;
    UnkWapStatePtr_la0a2dc states[16];
    u32 pending;
    bool busy;

    UnkWapState_la0a2dc* Find(u32 id) {
        return FindStateById(states, count, id);
    }

    // Inlined in the original; the barrier in Find makes it too large for the inliner otherwise.
    __attribute__((always_inline)) void Request(u32 id) {
        pending = id;
        if (id > kLast || busy)
            return;
        UnkWapState_la0a2dc* cur = current;
        if (!cur || cur->id != id) {
            UnkWapState_la0a2dc* next = Find(id);
            if (!next && id != 0) {
                pending = kNone;
                return;
            }
            if (cur) {
                cur->Leave(id);
                OnLeave(current, next);
            }
            previous = current;
            current = next;
            if (next) {
                next->Enter(previous ? previous->id : kNone);
                OnEnter(previous, next);
            }
        }
        pending = kNone;
    }
};

struct UnkWapUsableLink_la0a2dc {
    u8 pad_0x0[0x10];
    UnkUsableComponent_b49a4* usable;
};

struct UnkWapValue_la0a2dc {
    CStrId name;
    u64 field_0x8;
};

struct UnkWap_la0a2dc {
    u8 pad_0x0[0xb8];
    UnkWapUsableLink_la0a2dc* usable_link;  // 0xb8
    u8 pad_0xc0[0x180 - 0xc0];
    UnkWapMachine_la0a2dc machine;  // 0x180
    u8 pad_0x230[0x570 - 0x230];
    UnkWapValue_la0a2dc* values;  // 0x570
    u64* keys;
    u32 count;
};

static_assert(__builtin_offsetof(UnkWap_la0a2dc, machine) == 0x180, "layout");
static_assert(__builtin_offsetof(UnkWapMachine_la0a2dc, pending) == 0xa4, "layout");
static_assert(__builtin_offsetof(UnkWap_la0a2dc, values) == 0x570, "layout");

// The index of the "Finished" property's value (one past the last when it has none).
inline u64 FinishedIndex(const UnkWap_la0a2dc* self) {
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_71727434e0.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    return index;
}

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_7100a0a2dc(UnkWeightPlatform_b49a4* platform) {
    UnkWap_la0a2dc* self = reinterpret_cast<UnkWap_la0a2dc*>(platform);
    self->machine.Request(6);

    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    bool stored;
    {
        CStrId section = fn_71011a2fbc(manager->field_0x608);
        u64 index = FinishedIndex(self);
        UnkWapValue_la0a2dc* values = self->values;
        stored = fn_71010474f4(blackboard, section, values[index].name);
    }
    if (stored) {
        CStrId section = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        u64 index = FinishedIndex(self);
        UnkWapValue_la0a2dc* values = self->values;
        bool finished = false;
        SetSectionBool(blackboard, section, reinterpret_cast<UnkPropertyName*>(&values[index].name), &finished);
    }

    if (UnkWapUsableLink_la0a2dc* link = self->usable_link) {
        if (UnkUsableComponent_b49a4* usable = link->usable)
            fn_7100a9ffcc(usable, false);
    }
}
