// CRinkaUnitComponent's unit (tools/tu_map.py unit 981): the component's init. After the component
// base's (fn_71010c4228) it drops the counted reference at +0x260, binds the callbacks of its three
// states, registers them on its state machine (+0x1a8) and asks the machine for state 1.
// Names and layouts are placeholders.
#include "game/CComponent.h"
#include "game/UnkStateMachineFind.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100661220();
void fn_7100661224();
void fn_7100661228();
void fn_710066122c();
void fn_7100661230();
void fn_7100661234();

struct UnkRinkaMachine_l65f878;

struct UnkRinkaDelegate_l65f878 {
    void* object;
    void (*function)();
};

struct UnkRinkaState_l65f878 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previous);  // 0x10
    virtual void Leave(s32 next);      // 0x18
    UnkRinkaMachine_l65f878* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkRinkaDelegate_l65f878 field_0x30;
    u8 pad_0x40[0x50 - 0x40];
    UnkRinkaDelegate_l65f878 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkRinkaState_l65f878) == 0x70, "layout");

struct UnkRinkaMachine_l65f878 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEntered(UnkRinkaState_l65f878* previous, UnkRinkaState_l65f878* next);  // 0x20
    virtual void OnLeaving(UnkRinkaState_l65f878* current, UnkRinkaState_l65f878* next);   // 0x28

    void* owner;
    UnkRinkaState_l65f878* current;
    UnkRinkaState_l65f878* previous;
    u32 count;
    u32 states[0x20];  // UnkRinkaState_l65f878* entries, 4-byte aligned
    u32 pending;
    bool busy;

    UnkRinkaState_l65f878** States() { return reinterpret_cast<UnkRinkaState_l65f878**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkRinkaState_l65f878* state) {
        UnkRinkaState_l65f878** begin = States();
        UnkRinkaState_l65f878** end = begin + count;
        for (UnkRinkaState_l65f878** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    UnkRinkaState_l65f878* Find(u32 id) {
        return FindStateById(States(), count, id);
    }

    // Switches to state `id` now unless the machine is busy; `none` is the id of no state.
    void Request(u32 id, u32 none) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != id) {
            if (UnkRinkaState_l65f878* next = Find(id)) {
                if (current) {
                    current->Leave(id);
                    OnLeaving(current, next);
                }
                previous = current;
                current = next;
                next->Enter(previous ? previous->id : none);
                OnEntered(previous, next);
            }
        }
        pending = none;
    }
};

struct UnkRinkaUnit_l65f878 {
    u8 pad_0x0[0x58];
    UnkRinkaState_l65f878 states[3];  // 0x58
    UnkRinkaMachine_l65f878 machine;  // 0x1a8
    u32 field_0x258;
    UnkRefObject* ref;  // 0x260
};
static_assert(__builtin_offsetof(UnkRinkaUnit_l65f878, ref) == 0x260, "layout");

void fn_710065f878(UnkRinkaUnit_l65f878* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    self->field_0x258 = 0;
    if (UnkRefObject* ref = self->ref) {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
        self->ref = nullptr;
    }
    self->machine.owner = self;
    self->states[0].field_0x30 = {self, fn_7100661220};
    self->states[0].field_0x50 = {self, fn_7100661224};
    self->states[1].field_0x30 = {self, fn_7100661228};
    self->states[1].field_0x50 = {self, fn_710066122c};
    self->states[2].field_0x30 = {self, fn_7100661230};
    self->states[2].field_0x50 = {self, fn_7100661234};
    for (int i = 0; i < 3; ++i)
        self->machine.Add(&self->states[i]);
    self->machine.Request(1, 4);
}
