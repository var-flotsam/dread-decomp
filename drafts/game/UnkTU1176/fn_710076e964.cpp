// Unit 1176 (CCentralUnitComponent): the component's constructor. Builds the state machine and its seven
// states, the callback sets, arrays and listeners, registers the states and enters state 1.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkMoveSlot_0xb0;

void fn_710024f694(void* self);
void fn_71007787c8(char* self);
void fn_71000309ac(UnkMoveSlot_0xb0* self);

extern void* g_unk_7101a6ec78[];
extern char g_unk_7101a6eed8[];
extern void* g_unk_7101a6ee28[];

void fn_710077b8f8(void* self);
void fn_710077b8c8(void* self);
void fn_710077b8e0(void* self);
void fn_710077bab4(void* self);
void fn_710077babc(void* self);
void fn_710077bad8(void* self);
void fn_710077bb10(void* self);
void fn_710077bb1c(void* self);
void fn_710077bb20(void* self);
void fn_710077bb24(void* self);
void fn_710077bb28(void* self);
void fn_710077bb2c(void* self);
void fn_710077bb30(void* self);
void fn_710077bb34(void* self);
void fn_710077bba8(void* self);
void fn_710077bbb0(void* self);
void fn_710077bbb4(void* self);
void fn_710077bbb8(void* self);
void fn_710077bbbc(void* self);
void fn_710077bbc0(void* self);
void fn_710077bbc4(void* self);

struct UnkStateMachine_l76e964;
struct UnkState_l76e964;

// A state's handler: the owner and the function called with it.
struct UnkStateHandler_l76e964 {
    void* owner = nullptr;
    void (*fn)(void*) = nullptr;

    void Set(void* o, void (*f)(void*)) {
        owner = o;
        fn = f;
    }
};

struct UnkStateVtbl_l76e964 {
    void* vf_0x0;
    void* vf_0x8;
    void (*Enter)(UnkState_l76e964* self, u32 previous_id);
    void (*Exit)(UnkState_l76e964* self, u32 next_id);
};

struct UnkState_l76e964 {
    const UnkStateVtbl_l76e964* vtable;
    UnkStateMachine_l76e964* machine = nullptr;
    u32 id;
    void* field_0x18 = nullptr;
    void* field_0x20 = nullptr;
    u32 field_0x28 = 2;
    u32 field_0x2c = 1;
    UnkStateHandler_l76e964 handlers[4];

    explicit UnkState_l76e964(u32 id)
        : vtable(reinterpret_cast<const UnkStateVtbl_l76e964*>(g_unk_7101a6ee28 + 2)), id(id) {}
};
static_assert(sizeof(UnkState_l76e964) == 0x70, "UnkState_l76e964 layout");

struct UnkStateMachineVtbl_l76e964 {
    void* vf_0x0[4];
    void (*OnEntered)(UnkStateMachine_l76e964* self, UnkState_l76e964* previous, UnkState_l76e964* state);
    void (*OnLeaving)(UnkStateMachine_l76e964* self, UnkState_l76e964* state, UnkState_l76e964* next);
};

struct UnkStateMachine_l76e964 {
    enum { kMaxStates = 16, kNoState = 8 };

    const UnkStateMachineVtbl_l76e964* vtable;
    void* owner = nullptr;
    UnkState_l76e964* current = nullptr;
    UnkState_l76e964* previous = nullptr;
    u32 count = 0;
    u8 storage[kMaxStates * sizeof(UnkState_l76e964*)];
    u32 pending_id = kNoState;
    u8 changing = 0;
    u8 field_0xd9 = 0;

    UnkStateMachine_l76e964()
        : vtable(reinterpret_cast<const UnkStateMachineVtbl_l76e964*>(g_unk_7101a6eed8 + 0x10)) {}

    UnkState_l76e964** items() { return reinterpret_cast<UnkState_l76e964**>(storage); }

    UnkState_l76e964* Find(u32 id) {
        for (UnkState_l76e964 **it = items(), **end = it + count; it != end; ++it) {
            if ((*it)->id == id)
                return *it;
        }
        return nullptr;
    }

    void Add(UnkState_l76e964* state) {
        for (UnkState_l76e964 **it = items(), **end = it + count; it != end; ++it) {
            if ((*it)->id == state->id)
                return;
        }
        state->machine = this;
        items()[count] = state;
        ++count;
    }

    void Change(u32 id) {
        pending_id = id;
        if (changing)
            return;
        if (!(current && current->id == id)) {
            if (UnkState_l76e964* next = Find(id)) {
                if (current) {
                    current->vtable->Exit(current, id);
                    vtable->OnLeaving(this, current, next);
                }
                UnkState_l76e964* last = current;
                current = next;
                previous = last;
                u32 last_id = last ? last->id : kNoState;
                next->vtable->Enter(next, last_id);
                vtable->OnEntered(this, previous, next);
            }
        }
        pending_id = kNoState;
    }
};
static_assert(__builtin_offsetof(UnkStateMachine_l76e964, pending_id) == 0xa4, "UnkStateMachine layout");
static_assert(sizeof(UnkStateMachine_l76e964) == 0xb0, "UnkStateMachine layout");

// A callback set: the base vtable around an array, then the set's own vtable.
struct UnkCallbackSet_l76e964 {
    void** vtable;
    CRntVector<void*> list;
    u32 field_0x30;
    void* field_0x38;

    explicit UnkCallbackSet_l76e964(void** own_vtable) : vtable(g_unk_7101a20998 + 2), list(15) {
        field_0x38 = nullptr;
        field_0x30 = 0;
        vtable = own_vtable;
    }
};
static_assert(sizeof(UnkCallbackSet_l76e964) == 0x40, "UnkCallbackSet layout");

struct UnkCallback_l76e964 {
    void* field_0x0 = nullptr;
    void* owner = nullptr;
    void (*fn)(void*) = nullptr;

    UnkCallback_l76e964() = default;
    UnkCallback_l76e964(void* o, void (*f)(void*)) : owner(o), fn(f) {}
};

struct UnkListener_l76e964 {
    void** vtable = g_unk_7101a22d48 + 2;
    void* field_0x8 = nullptr;
    void* field_0x10 = nullptr;
    void* link = nullptr;
    u32 field_0x20 = 0;
    u8 field_0x24 = 0;
};

// A string id and an array, built by fn_71007787c8.
struct UnkStrIdList_l76e964 {
    char data[0x38];
    UnkStrIdList_l76e964() { fn_71007787c8(data); }
};

struct UnkMoveSlot_l76e964 {
    u8 data[0xb0];
    UnkMoveSlot_l76e964() { fn_71000309ac(reinterpret_cast<UnkMoveSlot_0xb0*>(this)); }
};

struct UnkBlock_l76e964 {
    u8 data[0xb0];
    UnkBlock_l76e964() { fn_710002f4e4(this); }
};

// A string id left empty: the pool's empty string.
struct UnkEmptyStrId_l76e964 {
    UnkStringData* data;
    UnkEmptyStrId_l76e964() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkCentralUnitComponent_l76e964 {
    enum { kStateCount = 7 };

    void** vtable;
    u8 pad_0x8[0x28];
    UnkStateMachine_l76e964 machine;  // 0x30
    UnkState_l76e964 states[kStateCount];  // 0xe0
    u32 field_0x3f0;
    UnkCallbackSet_l76e964 sets_0x3f8[5];
    CStrId name;  // 0x538
    UnkCallbackSet_l76e964 set_0x540;
    CRntVector<void*> list_0x580;
    UnkCallbackSet_l76e964 set_0x5a8;
    CRntVector<void*> list_0x5e8;
    CRntVector<void*> list_0x610;
    bool field_0x638;
    CRntVector<void*> list_0x640;
    UnkCallbackSet_l76e964 set_0x668;
    CRntVector<void*> list_0x6a8;
    u8 field_0x6d0;
    UnkCallback_l76e964 callbacks_0x6d8[6];
    UnkListener_l76e964 listener;  // 0x768
    UnkStrIdList_l76e964 str_list;  // 0x790
    UnkCallback_l76e964 callbacks_0x7c8[2];
    UnkMoveSlot_l76e964 move_slot;  // 0x7f8
    UnkBlock_l76e964 block_0x8a8;
    void* field_0x958;
    f32 scale_x;
    f32 scale_y;
    u32 field_0x968;
    UnkCallbackSet_l76e964 set_0x970;
    UnkEmptyStrId_l76e964 empty_name;  // 0x9b0
    UnkCallbackSet_l76e964 set_0x9b8;
    bool field_0x9f8;
};
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76e964, states) == 0xe0, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76e964, sets_0x3f8) == 0x3f8, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76e964, list_0x580) == 0x580, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76e964, listener) == 0x768, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76e964, move_slot) == 0x7f8, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76e964, set_0x970) == 0x970, "layout");
static_assert(__builtin_offsetof(UnkCentralUnitComponent_l76e964, field_0x9f8) == 0x9f8, "layout");

void fn_710076e964(UnkCentralUnitComponent_l76e964* self) {
    fn_710024f694(self);
    self->vtable = g_unk_7101a6ec78 + 2;
    new (&self->machine) UnkStateMachine_l76e964();
    for (u32 i = 0; i < UnkCentralUnitComponent_l76e964::kStateCount; i++)
        new (&self->states[i]) UnkState_l76e964(i + 1);
    self->field_0x3f0 = 0;
    for (UnkCallbackSet_l76e964& set : self->sets_0x3f8)
        new (&set) UnkCallbackSet_l76e964(g_unk_7101a33fa0 + 2);
    new (&self->name) CStrId(g_unk_7101d470e0);
    new (&self->set_0x540) UnkCallbackSet_l76e964(g_unk_7101a33fa0 + 2);
    new (&self->list_0x580) CRntVector<void*>(15);
    new (&self->set_0x5a8) UnkCallbackSet_l76e964(g_unk_7101a20950 + 2);
    new (&self->list_0x5e8) CRntVector<void*>(15);
    new (&self->list_0x610) CRntVector<void*>(15);
    self->field_0x638 = true;
    new (&self->list_0x640) CRntVector<void*>(15);
    new (&self->set_0x668) UnkCallbackSet_l76e964(g_unk_7101a20950 + 2);
    new (&self->list_0x6a8) CRntVector<void*>(15);
    self->field_0x6d0 = 0;
    new (&self->callbacks_0x6d8[0]) UnkCallback_l76e964();
    new (&self->callbacks_0x6d8[1]) UnkCallback_l76e964();
    new (&self->callbacks_0x6d8[2]) UnkCallback_l76e964(self, fn_710077b8f8);
    new (&self->callbacks_0x6d8[3]) UnkCallback_l76e964(self, fn_710077b8c8);
    new (&self->callbacks_0x6d8[4]) UnkCallback_l76e964(self, fn_710077b8e0);
    new (&self->callbacks_0x6d8[5]) UnkCallback_l76e964(self, fn_710077bab4);
    new (&self->listener) UnkListener_l76e964();
    new (&self->str_list) UnkStrIdList_l76e964();
    new (&self->callbacks_0x7c8[0]) UnkCallback_l76e964(self, fn_710077babc);
    new (&self->callbacks_0x7c8[1]) UnkCallback_l76e964(self, fn_710077bad8);
    new (&self->move_slot) UnkMoveSlot_l76e964();
    new (&self->block_0x8a8) UnkBlock_l76e964();
    self->field_0x958 = nullptr;
    self->scale_x = 1.0f;
    self->scale_y = 1.0f;
    self->field_0x968 = 0;
    new (&self->set_0x970) UnkCallbackSet_l76e964(g_unk_7101a20950 + 2);
    new (&self->empty_name) UnkEmptyStrId_l76e964();
    new (&self->set_0x9b8) UnkCallbackSet_l76e964(g_unk_7101a20950 + 2);
    self->field_0x9f8 = true;

    UnkStateMachine_l76e964& machine = self->machine;
    machine.owner = self;
    UnkState_l76e964& s1 = self->states[0];
    machine.Add(&s1);
    s1.handlers[1].Set(self, fn_710077bb10);
    UnkState_l76e964& s2 = self->states[1];
    machine.Add(&s2);
    s2.handlers[0].Set(self, fn_710077bb1c);
    s2.handlers[2].Set(self, fn_710077bb20);
    UnkState_l76e964& s3 = self->states[2];
    machine.Add(&s3);
    s3.handlers[0].Set(self, fn_710077bb24);
    s3.handlers[2].Set(self, fn_710077bb28);
    s3.handlers[1].Set(self, fn_710077bb2c);
    UnkState_l76e964& s4 = self->states[3];
    machine.Add(&s4);
    s4.handlers[0].Set(self, fn_710077bb30);
    s4.handlers[2].Set(self, fn_710077bb34);
    s4.handlers[1].Set(self, fn_710077bba8);
    UnkState_l76e964& s5 = self->states[4];
    machine.Add(&s5);
    s5.handlers[0].Set(self, fn_710077bbb0);
    s5.handlers[2].Set(self, fn_710077bbb4);
    UnkState_l76e964& s6 = self->states[5];
    machine.Add(&s6);
    s6.handlers[0].Set(self, fn_710077bbb8);
    s6.handlers[2].Set(self, fn_710077bbbc);
    s6.handlers[1].Set(self, fn_710077bbc0);
    UnkState_l76e964& s7 = self->states[6];
    machine.Add(&s7);
    s7.handlers[0].Set(self, fn_710077bbc4);
    machine.Change(1);
}
