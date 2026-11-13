// More callers that request a state from a component's small state machine (the template of
// src/game/UnkStateMachineSet_b60.cpp, copied here: the machine of src/game/UnkStateMachineUpdate_b59.cpp
// embedded at some offset, followed by a "busy" flag at +0xa8). One template per (last state, "none"
// value), instantiated per component and machine offset. Names are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CComponent.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2772.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkSmState_b61 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b61* __attribute__((aligned(4))) UnkSmStatePtr_b61;

struct UnkSmMachine_b61 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b61* previous, UnkSmState_b61* next);
    virtual void vf_0x28(UnkSmState_b61* current, UnkSmState_b61* next);

    u64 field_0x8;
    UnkSmState_b61* current;
    UnkSmState_b61* previous;
    u32 count;
    UnkSmStatePtr_b61 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_b61 {
    u8 pad_0x0[Offset];
    UnkSmMachine_b61 machine;
};

__attribute__((always_inline)) static inline bool FindState_b61(UnkSmMachine_b61* self, u32 id,
                                                               UnkSmState_b61*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b61);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_b61* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b61)) {
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
__attribute__((always_inline)) static inline void SwitchState_b61(UnkSmMachine_b61* self, u32 id,
                                                                 UnkSmState_b61* current) {
    if (!current || current->id != id) {
        UnkSmState_b61* found;
        UnkSmState_b61* next;
        u32 enter;
        if (FindState_b61(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            next = nullptr;
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_b61*, s32);
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
__attribute__((always_inline)) static inline void RequestState_b61(UnkSmMachine_b61* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_b61<None>(self, id, self->current);
}

// Constant-id requests followed by the unit's own call.
void fn_7100aca57c(UnkSmOwner_b61<0x130>*);

void fn_7100aca484(UnkSmOwner_b61<0x130>* self) {
    RequestState_b61<2, 3>(&self->machine, 1);
    fn_7100aca57c(self);
}

void fn_7100dd905c(UnkSmOwner_b61<0>* self) {
    RequestState_b61<8, 9>(&self->machine, 7);
}

struct UnkSmFlagged_b61 {
    UnkSmMachine_b61 machine;
    u8 pad_0xb0[0x269 - 0xb0];
    bool field_0x269;
};
void fn_7100dd3624(UnkSmFlagged_b61*);
extern bool g_unk_71727bdf80 __attribute__((visibility("hidden")));

void fn_7100dd3520(UnkSmFlagged_b61* self) {
    self->field_0x269 = false;
    fn_7100dd3624(self);
    g_unk_71727bdf80 = false;
    RequestState_b61<8, 9>(&self->machine, 2);
}

// A machine at +0x30 whose requests can wait for a delay (fn_7100fd0d40).
struct UnkSmDelayed_b61 {
    u8 pad_0x0[0x30];
    UnkSmMachine_b61 machine;
    u8 pad_0xe0[0xb60 - 0xe0];
    u32 field_0xb60;
    bool field_0xb64;
    u32 delayed_id;
    f32 delay;
    f32 elapsed;
};

// Requests `id` once `delay` has run out (not matched: the switch's blocks are laid out differently).
void fn_7100fd0d40(UnkSmDelayed_b61& self, u32 id, f32 delay);

// Requests the stored id; the busy flag is read before the pending id is stored.
void fn_7100fd0c14(UnkSmDelayed_b61* self) {
    self->field_0xb64 = true;
    u32 id = self->field_0xb60;
    UnkSmMachine_b61* m = &self->machine;
    u32* pending = &m->pending;
    bool busy = m->busy;
    *pending = id;
    if (id > 7 || busy)
        return;
    SwitchState_b61<0x7fffffff>(m, id, m->current);
}

struct UnkSmDelayedOwner_b61 {
    UnkSmMachine_b61 machine;
    u8 pad_0xb0[0xb4 - 0xb0];
    u8 field_0xb4;
    u8 pad_0xb5[0x2b8 - 0xb5];
    UnkSmDelayed_b61 delayed;
};

// Takes its owner by reference (dereferenceable: the flag test is speculated into a ccmp). Its one
// caller also passes two stack addresses (x1, x2) that nothing here uses; x2 reaches the leaving
// state's vf_0x18 untouched (tools/forwarded_args.py).
void fn_7100fdb8b8(UnkSmDelayedOwner_b61& self, void* /*a*/, void* /*b*/) {
    RequestState_b61<7, 0x7fffffff>(&self.machine, 3);
    UnkSmState_b61* current = self.machine.current;
    if (current && ((current->id == 3) & (self.field_0xb4 == 0)))
        fn_7100fd0d40(self.delayed, 1, 0.5f);
}

// The request after a base call: CUsableComponent (fn_7100a9f1f8) and CAccessPointComponent, which
// calls it as its base.
struct UnkParent;

__attribute__((noinline)) void fn_7100a9f1f8(UnkSmOwner_b61<0x100>* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    RequestState_b61<7, 8>(&self->machine, 1);
}

struct UnkAccessPointSm_b61 {
    u8 pad_0x0[0x4f8];
    bool field_0x4f8;
    u8 pad_0x4f9[0x591 - 0x4f9];
    bool field_0x591;
    u8 pad_0x592[0x5e8 - 0x592];
    UnkSmMachine_b61 machine;
};

void fn_7100a6d418(UnkAccessPointSm_b61* self) {
    fn_7100a9f1f8(reinterpret_cast<UnkSmOwner_b61<0x100>*>(self));
    self->field_0x4f8 = false;
    RequestState_b61<4, 5>(&self->machine, 1);
}

struct UnkUsableSm_b61 {
    u8 pad_0x0[0x4f8];
    bool field_0x4f8;
    u8 pad_0x4f9[0x548 - 0x4f9];
    UnkSmMachine_b61 machine;
    u8 pad_0x5f8[0x7b8 - 0x5f8];
    bool field_0x7b8;
    u8 pad_0x7b9[0x804 - 0x7b9];
    u32 field_0x804;
};

void fn_7100a8524c(UnkUsableSm_b61* self) {
    fn_7100a9f1f8(reinterpret_cast<UnkSmOwner_b61<0x100>*>(self));
    self->field_0x804 = 0;
    self->field_0x4f8 = false;
    RequestState_b61<5, 6>(&self->machine, 1);
}

// After the base call and the request: hands +0x40 to the owner's object (fn_71008656d8), then passes a
// copy of a string id to fn_7100ac9530.
struct UnkSmIdHolder_b61 {
    u8 pad_0x0[0x30];
    CStrId id;
};
struct UnkSmObjOwner_b61 {
    u8 pad_0x0[0x2a0];
    void* object;
};
struct UnkSmIdUser_b61 {
    u8 pad_0x0[0x18];
    UnkSmIdHolder_b61* holder;
    u8 pad_0x20[0x30 - 0x20];
    UnkSmObjOwner_b61* owner;
    u8 pad_0x38[0x40 - 0x38];
    u8 field_0x40[0x10];
    u8 pad_0x50[0x130 - 0x50];
    UnkSmMachine_b61 machine;
    u8 pad_0x1e0[0x200 - 0x1e0];
    u8 field_0x200[8];
};
void fn_7100ac9530(void* self, CStrId id);

void fn_7100ac939c(UnkSmIdUser_b61* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    void* object = self->owner->object;
    RequestState_b61<2, 3>(&self->machine, 1);
    fn_71008656d8(object, 0x10000, 0x10, self->field_0x40);
    fn_7100ac9530(self->field_0x200, self->holder->id);
}

static inline void Checkpoint_b61(const char* name) {
    UnkGameManager* manager = g_unk_7101d47310;
    CRntString checkpoint(name);
    fn_7100dfaf88(manager, &checkpoint, CRntString(""), true);
}

void fn_7100a705bc(UnkAccessPointSm_b61* self) {
    self->field_0x591 = true;
    RequestState_b61<4, 5>(&self->machine, 1);
    Checkpoint_b61("AccessPointCheckpoint");
}

void fn_7100a866bc(UnkUsableSm_b61* self) {
    self->field_0x7b8 = true;
    RequestState_b61<5, 6>(&self->machine, 1);
    Checkpoint_b61("MapAdquisitionCheckpoint");
}

// CCharClassLockOnMissileMovement's unit (machine at +0x760): after a base call, requests a state and
// resets the movement.
struct UnkLockOnSm_b61;
void fn_7100aba644(UnkLockOnSm_b61*);
void fn_7100aba68c(UnkLockOnSm_b61*);
void fn_7100ab6bb8(UnkLockOnSm_b61*);
void fn_7100ab9838(void*);

struct UnkLockOnItem_b61 {
    u64 field_0x0;
    u8 item[8];
};

struct UnkLockOnSm_b61 {
    u8 pad_0x0[0x678];
    u32 field_0x678;
    u8 pad_0x67c[0x760 - 0x67c];
    UnkSmMachine_b61 machine;
    u8 pad_0x810[0x810 - 0x810];
    u8 field_0x810[0x18];
    bool field_0x828;
    u8 pad_0x829[0x838 - 0x829];
    u32 field_0x838;
    u8 pad_0x83c[0x858 - 0x83c];
    UnkLockOnItem_b61* field_0x858;
    u8 pad_0x860[0x868 - 0x860];
    bool field_0x868;
    u8 pad_0x869[0x132f8 - 0x869];
    bool field_0x132f8;
};

void fn_7100ab6954(UnkLockOnSm_b61* self) {
    fn_7100aba644(self);
    RequestState_b61<7, 0x7fffffff>(&self->machine, 2);
    self->field_0x678 = fn_71012395a0() & 3;
    self->field_0x868 = false;
}

void fn_7100ab6a60(UnkLockOnSm_b61* self) {
    fn_7100aba68c(self);
    self->field_0x838 = 0;
    RequestState_b61<7, 0x7fffffff>(&self->machine, 1);
    bool* done = &self->field_0x132f8;
    asm("" : "+r"(done));
    if (*done)
        fn_7100ab6bb8(self);
    if (UnkLockOnItem_b61* item = self->field_0x858) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(item->item), 0);
        self->field_0x858 = nullptr;
    }
    self->field_0x868 = false;
    self->field_0x828 = false;
    fn_7100ab9838(self->field_0x810);
    *done = true;
}

// CUsableComponent's unit: a flag picks the state to request.
void fn_7100a86810(UnkUsableSm_b61* self) {
    bool flag = reinterpret_cast<bool*>(self)[0x539];
    self->field_0x7b8 = false;
    if (flag)
        RequestState_b61<5, 6>(&self->machine, 4);
    else
        RequestState_b61<5, 6>(&self->machine, 2);
}

// CAccessPointComponent's unit: two flags pick the state to request.
void fn_7100a701c8(UnkAccessPointSm_b61* self) {
    u8* raw = reinterpret_cast<u8*>(self);
    self->field_0x591 = false;
    raw[0x998] = false;
    if (raw[0x590] || !raw[0x91d])
        RequestState_b61<4, 5>(&self->machine, 3);
    else
        RequestState_b61<4, 5>(&self->machine, 4);
}

// CWeightActivatedPlatformSmartObjectComponent's unit: requests state 7, stores true under the
// component's property (g_unk_71727434e0) in the blackboard section of the current scenario unless the
// blackboard is locked (as CElevatorUsableComponent_b48.cpp), then notifies the linked object.
void fn_7100332c80(CBlackboard::CSection*, UnkPropertyName*, const UnkTypedValue*);

struct UnkPlatformValue_b61 {
    UnkPropertyName name;
    u64 field_0x8;
};
struct UnkPlatformLink_b61 {
    u8 pad_0x0[0x10];
    void* object;
};
struct UnkPlatformSm_b61 {
    u8 pad_0x0[0xb8];
    UnkPlatformLink_b61* link;
    u8 pad_0xc0[0x180 - 0xc0];
    UnkSmMachine_b61 machine;
    u8 pad_0x230[0x570 - 0x230];
    UnkPlatformValue_b61* values;
    u64* keys;
    u32 count;
};

static inline void SetSectionBool_b61(CBlackboard* blackboard, const CStrId& section,
                                      UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

// Stores true under the property (the section id is released before the caller goes on).
static inline void StoreTrue_b61(UnkPlatformSm_b61* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    bool value;
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
    UnkPlatformValue_b61* values = self->values;
    value = true;
    SetSectionBool_b61(blackboard, section_id, &values[index].name, &value);
}

void fn_7100a0a08c(UnkPlatformSm_b61* self) {
    RequestState_b61<7, 6>(&self->machine, 7);
    StoreTrue_b61(self);
    if (UnkPlatformLink_b61* link = self->link) {
        if (void* object = link->object)
            fn_7100aa0034(object);
    }
}

// The machine's update: applies the pending state, then updates the current state and applies what it
// asked for, at most ten times, while marked busy (requests meanwhile only record the id).
template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Apply_b61(UnkSmMachine_b61* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_b61<None>(self, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Update_b61(UnkSmMachine_b61* self, f32 dt) {
    self->busy = true;
    if (self->pending != None)
        Apply_b61<Last, None>(self);
    u32 i = 0;
    while (UnkSmState_b61* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == None)
            break;
        Apply_b61<Last, None>(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

#define UNK_SM_UPDATE_B61(addr, offset, last, none)       \
    void fn_##addr(UnkSmOwner_b61<offset>* self, f32 dt) { \
        Update_b61<last, none>(&self->machine, dt);         \
    }

UNK_SM_UPDATE_B61(7100a0a584, 0x180, 7, 6)
UNK_SM_UPDATE_B61(7100dc8e04, 8, 3, 4)
