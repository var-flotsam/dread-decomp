// CChozoCommanderUltimateGrabAttack (unit 658): sets up the attack's state machine. Registers its
// twelve states (each once, by id), binds their callbacks to the attack, then requests state 1 and
// enters it unless the machine is locked. Names and layouts are placeholders.
#include "game/CChozoCommanderUltimateGrabAttack.h"
#include "game/UnkStateMachineFind.h"
#include "types.h"

struct UnkUltimateGrabMachine_43e248;

struct UnkUltimateGrabDelegate_43e248 {
    void* object;
    void (*function)();
};

struct UnkUltimateGrabState_43e248 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previous_id);
    virtual void Exit(s32 next_id);

    UnkUltimateGrabMachine_43e248* machine;
    s32 id;
    u8 pad_0x14[0x1c];
    UnkUltimateGrabDelegate_43e248 field_0x30;
    UnkUltimateGrabDelegate_43e248 field_0x40;
    UnkUltimateGrabDelegate_43e248 field_0x50;
    u8 pad_0x60[0x10];
};
static_assert(sizeof(UnkUltimateGrabState_43e248) == 0x70, "layout");

struct UnkUltimateGrabMachine_43e248 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEnter(UnkUltimateGrabState_43e248* previous, UnkUltimateGrabState_43e248* next);
    virtual void OnExit(UnkUltimateGrabState_43e248* current, UnkUltimateGrabState_43e248* next);

    void* owner;
    UnkUltimateGrabState_43e248* current;
    UnkUltimateGrabState_43e248* previous;
    u32 count;
    u32 states[0x20];  // UnkUltimateGrabState_43e248* entries, 4-byte aligned
    s32 pending;
    u8 locked;

    UnkUltimateGrabState_43e248** States() { return reinterpret_cast<UnkUltimateGrabState_43e248**>(states); }

    void Add(UnkUltimateGrabState_43e248* state) {
        UnkUltimateGrabState_43e248** begin = States();
        UnkUltimateGrabState_43e248** end = begin + count;
        for (UnkUltimateGrabState_43e248** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    UnkUltimateGrabState_43e248* Find(s32 id) {
        return FindStateById(States(), count, id);
    }

    void Change(s32 id) {
        UnkUltimateGrabState_43e248* cur = current;
        if (cur && cur->id == id)
            return;
        UnkUltimateGrabState_43e248* next = Find(id);
        if (!next)
            return;
        if (cur) {
            cur->Exit(id);
            OnExit(current, next);
        }
        previous = current;
        current = next;
        next->Enter(previous ? previous->id : 0x7fffffff);
        OnEnter(previous, next);
    }

    void Request(s32 id) {
        pending = id;
        if (locked)
            return;
        Change(id);
        pending = 0x7fffffff;
    }
};

struct UnkUltimateGrabStates_43e248 {
    u8 pad_0x0[0x88];
    UnkUltimateGrabMachine_43e248 machine;  // 0x88
    u8 pad_0x138[0x18];
    UnkUltimateGrabState_43e248 states[12];  // 0x150
};
static_assert(__builtin_offsetof(UnkUltimateGrabStates_43e248, states) == 0x150, "layout");

void fn_7100443174();
void fn_7100443178();
void fn_710044317c();
void fn_710044318c();
void fn_71004431bc();
void fn_71004431c0();
void fn_71004431d0();
void fn_7100443224();
void fn_7100443228();
void fn_7100443238();
void fn_7100443270();
void fn_7100443274();
void fn_7100443284();
void fn_71004432b4();
void fn_71004432b8(UnkEffect* self);
void fn_7100443364();
void fn_7100443374();
void fn_71004433e8();
void fn_71004433ec();
void fn_71004433fc();
void fn_7100443450();
void fn_7100443454();
void fn_7100443464();
void fn_710044349c();
void fn_71004434a0();
void fn_71004434b0();
void fn_7100443500();
void fn_7100443504();
void fn_71004435a8();

void fn_710043e248(UnkUltimateGrabStates_43e248* self) {
    UnkUltimateGrabMachine_43e248& m = self->machine;
    UnkUltimateGrabState_43e248* s = self->states;
    m.owner = self;

    m.Add(&s[0]);
    s[0].field_0x30 = {self, fn_7100443174};

    m.Add(&s[1]);
    s[1].field_0x30 = {self, fn_7100443178};
    s[1].field_0x50 = {self, fn_710044317c};

    m.Add(&s[2]);
    s[2].field_0x30 = {self, fn_710044318c};
    s[2].field_0x40 = {self, fn_71004431bc};
    s[2].field_0x50 = {self, fn_71004431c0};

    m.Add(&s[3]);
    s[3].field_0x30 = {self, fn_71004431d0};
    s[3].field_0x40 = {self, fn_7100443224};
    s[3].field_0x50 = {self, fn_7100443228};

    m.Add(&s[4]);
    s[4].field_0x30 = {self, fn_7100443238};
    s[4].field_0x40 = {self, fn_7100443270};
    s[4].field_0x50 = {self, fn_7100443274};

    m.Add(&s[5]);
    s[5].field_0x30 = {self, fn_7100443284};
    s[5].field_0x50 = {self, fn_71004432b4};

    m.Add(&s[6]);
    s[6].field_0x30 = {self, reinterpret_cast<void (*)()>(fn_71004432b8)};
    s[6].field_0x50 = {self, fn_7100443364};

    m.Add(&s[7]);
    s[7].field_0x30 = {self, fn_7100443374};
    s[7].field_0x40 = {self, fn_71004433e8};
    s[7].field_0x50 = {self, fn_71004433ec};

    m.Add(&s[8]);
    s[8].field_0x30 = {self, fn_71004433fc};
    s[8].field_0x40 = {self, fn_7100443450};
    s[8].field_0x50 = {self, fn_7100443454};

    m.Add(&s[9]);
    s[9].field_0x30 = {self, fn_7100443464};
    s[9].field_0x40 = {self, fn_710044349c};
    s[9].field_0x50 = {self, fn_71004434a0};

    m.Add(&s[10]);
    s[10].field_0x30 = {self, fn_71004434b0};
    s[10].field_0x40 = {self, fn_7100443500};
    s[10].field_0x50 = {self, fn_7100443504};

    m.Add(&s[11]);
    s[11].field_0x30 = {self, reinterpret_cast<void (*)()>(fn_7100443508)};
    s[11].field_0x50 = {self, fn_71004435a8};

    m.Request(1);
}
