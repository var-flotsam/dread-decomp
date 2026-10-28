// CDoorLifeComponent's unit (its static initialiser is src/game_s7/fn_71008da6c0.cpp, which
// registers the CDoorLifeComponent::SState enum and the "NextState" property): fn_71008d586c sends
// a door's state machine (at +0x420, as in src/game/CWeightActivablePropComponent/fn_7100acb14c.cpp) to state 3, from
// state 1 (when forced or allowed) or 2, and can write the new state to the blackboard section of
// the current scenario. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkState {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkState* __attribute__((aligned(4))) UnkStatePtr;

// A state machine whose state change can be held back (field_0xa8).
struct UnkStateMachine {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkState* previous, UnkState* next);  // after entering
    virtual void vf_0x28(UnkState* current, UnkState* next);   // after leaving

    u64 field_0x8;
    UnkState* current;
    UnkState* previous;
    u32 count;
    UnkStatePtr states[16];
    s32 pending;
    bool field_0xa8;
};

static inline UnkState* FindState(UnkStateMachine* self, s32 id) {
    for (UnkStatePtr *p = self->states, *e = p + self->count; p != e; ++p) {
        if ((*p)->id == id)
            return *p;
    }
    return nullptr;
}

// Asks for the state `id`: unless changes are held back, the state with that id (if any and not the
// current one) is entered at once and the request cleared.
static inline void RequestState(UnkStateMachine* self, s32 id) {
    self->pending = id;
    if (self->field_0xa8)
        return;
    UnkState* current = self->current;
    if (!current || current->id != id) {
        if (UnkState* next = FindState(self, id)) {
            if (current) {
                current->vf_0x18(id);
                self->vf_0x28(self->current, next);
            }
            UnkState* prev = self->current;
            self->current = next;
            self->previous = prev;
            next->vf_0x10(prev ? prev->id : 0x7fffffff);
            self->vf_0x20(self->previous, next);
        }
    }
    self->pending = 0x7fffffff;
}

// A named value of the component (0x10 bytes; the property's name first).
struct UnkDoorPropertyValue {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkDoorLife {
    u8 pad_0x0[0x40c];
    s32 field_0x40c;  // the state written to the blackboard
    u8 pad_0x410[0x10];
    UnkStateMachine field_0x420;
    u8 pad_0x4d0[0x154];
    f32 field_0x624;
    u8 pad_0x628[0x48];
    // The component's named values: their names' ids (keys) and values, `count` of each.
    UnkDoorPropertyValue* values;
    base::global::UnkStringData** keys;
    u32 count;
    u8 pad_0x684[0x28];
    bool field_0x6ac;
    u8 pad_0x6ad;
    bool field_0x6ae;
    u8 pad_0x6af[0x369];
    UnkGuiKeyList field_0xa18;  // the keys fn_71008d586c adds (fn_710002fd10)
};
static_assert(__builtin_offsetof(UnkDoorLife, field_0x624) == 0x624, "UnkDoorLife layout");
static_assert(__builtin_offsetof(UnkDoorLife, field_0x6ac) == 0x6ac, "UnkDoorLife layout");
static_assert(__builtin_offsetof(UnkDoorLife, field_0xa18) == 0xa18, "UnkDoorLife layout");

// Whether the door may open now.
bool fn_71008d5c1c(UnkDoorLife* self);
CStrId fn_71011a2fbc(UnkGameManagerSub_0x608* self);

// A typed value as the section setter takes it (as in src/game/CXParasiteAIComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// The "NextState" property and the CDoorLifeComponent::SState enum (src/game_s7/fn_71008da6c0.cpp).
struct UnkEnumType;
extern UnkProperty g_unk_7172731160;
extern UnkEnumType g_unk_7172731200;

// Stores the typed value under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionValue(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name,
                                   const base::reflection::CType* type, void* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = type;
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

// The named value of the component whose name is `key` (one past the last when there is none).
static inline UnkDoorPropertyValue* FindValue(UnkDoorLife* self, base::global::UnkStringData* key) {
    u64 i = 0;
    for (; i < self->count; ++i) {
        if (self->keys[i] == key)
            break;
    }
    return &self->values[i];
}

// Sends the door to state 3, from state 1 (when `force` is set or fn_71008d5c1c allows it; else
// field_0x6ac is set) or 2; in state 3 a running timer (field_0x624) is cancelled and false
// returned. With `notify`, the state 3 is stored in the blackboard section of the current scenario
// under "NextState". A non-empty `id` is added to the list at +0xa18 first.
// NON_MATCHING(minor): block layout and register allocation: the original places the notify part
// right after the state checks and the two state switches at the end, sharing their entering code;
// ours keeps two copies, folds the previous state to null when there was no current one, and
// post-increments the pointer of the state search.
bool fn_71008d586c(UnkDoorLife* self, bool force, bool notify, const CStrId* id) {
    if (id->data != &g_unk_7101d47108->empty)
        fn_710002fd10(&self->field_0xa18, const_cast<CStrId*>(id));
    if (UnkState* current = self->field_0x420.current) {
        switch (current->id) {
        case 1:
            if (force || fn_71008d5c1c(self))
                RequestState(&self->field_0x420, 3);
            else
                self->field_0x6ac = true;
            break;
        case 2:
            RequestState(&self->field_0x420, 3);
            break;
        case 3:
            if (self->field_0x624 > 0.0f) {
                self->field_0x624 = -1.0f;
                self->field_0x6ae = false;
            }
            return false;
        }
    }
    if (notify) {
        UnkGameManager* gm = g_unk_7101d47310;
        self->field_0x40c = 3;
        CStrId section = fn_71011a2fbc(gm->field_0x608);
        UnkDoorPropertyValue* value = FindValue(self, g_unk_7172731160.name.data);
        SetSectionValue(&gm->field_0x968, section, &value->name,
                        reinterpret_cast<const base::reflection::CType*>(&g_unk_7172731200), &self->field_0x40c);
    }
    return true;
}
