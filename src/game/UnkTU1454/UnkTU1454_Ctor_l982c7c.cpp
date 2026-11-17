// Unit 1454: the constructor of the object fn_71009878fc destroys: a state itself (id given), two state
// machines with three and two states, a few flags, then fn_7100982e34 registers the states. Names and
// layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a86030[];  // the object's vtable
extern void* g_unk_7101a86278[];  // the first machine's vtable
extern void* g_unk_7101a86238[];  // the second machine's vtable
extern void* g_unk_7101a86188[];  // the first machine's states' vtable
extern void* g_unk_7101a860d8[];  // the second machine's states' vtable

struct Unk_l982e34;
void fn_7100982e34(Unk_l982e34* self);

struct UnkMachine_l982c7c;

// A state: vtable, machine, id, a reflected value and three callbacks (0x70 bytes).
struct UnkStateBase_l982c7c {
    void** vtable;
    UnkMachine_l982c7c* machine;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;

    void InitFields(u32 state_id) {
        machine = nullptr;
        id = state_id;
        field_0x18 = 0;
        field_0x20 = 0;
        field_0x28 = 2;
        field_0x2c = 1;
    }
};

struct UnkState_l982c7c : UnkStateBase_l982c7c {
    void* slots[8];

    void Init(void** vt, u32 state_id) {
        InitFields(state_id);
        vtable = vt + 2;
        for (int i = 7; i >= 0; --i)
            slots[i] = nullptr;
    }
};
static_assert(sizeof(UnkState_l982c7c) == 0x70, "layout");

struct UnkMachine_l982c7c {
    void** vtable;
    void* owner;
    UnkState_l982c7c* current;
    UnkState_l982c7c* previous;
    u32 count;
    u32 states[0x20];  // UnkState_l982c7c* entries, 4-byte aligned
    u32 pending;
    bool busy;
    bool field_0xa9;

    void Init(void** vt, u32 first) {
        vtable = vt + 2;
        owner = nullptr;
        current = nullptr;
        previous = nullptr;
        count = 0;
        pending = first;
        busy = false;
        field_0xa9 = false;
    }
};
static_assert(sizeof(UnkMachine_l982c7c) == 0xb0, "layout");

struct UnkQuad_l982c7c {
    u32 v[4];

    UnkQuad_l982c7c() : v{0, 0, 0, 0} {}
};

struct Unk_l982c7c : UnkStateBase_l982c7c {
    UnkMachine_l982c7c machine;         // 0x30
    UnkMachine_l982c7c other_machine;   // 0xe0
    UnkState_l982c7c states[3];         // 0x190
    UnkState_l982c7c other_states[2];   // 0x2e0
    bool field_0x3c0;
    UnkQuad_l982c7c field_0x3c4;
    u32 field_0x3d4;
    u32 field_0x3d8;
    bool field_0x3dc;
    u64 field_0x3e0;
    bool field_0x3e8;
    bool field_0x3e9;
    u32 field_0x3ec;
    bool field_0x3f0;
};
static_assert(__builtin_offsetof(Unk_l982c7c, other_states) == 0x2e0, "layout");
static_assert(__builtin_offsetof(Unk_l982c7c, field_0x3f0) == 0x3f0, "layout");

void fn_7100982c7c(Unk_l982c7c* self, u32 id) {
    self->vtable = g_unk_7101a86030 + 2;
    self->InitFields(id);
    self->machine.Init(g_unk_7101a86278, 4);
    self->other_machine.Init(g_unk_7101a86238, 3);
    self->states[0].Init(g_unk_7101a86188, 1);
    self->states[1].Init(g_unk_7101a86188, 2);
    self->states[2].Init(g_unk_7101a86188, 3);
    self->other_states[0].Init(g_unk_7101a860d8, 1);
    self->other_states[1].Init(g_unk_7101a860d8, 2);
    self->field_0x3c0 = true;
    self->field_0x3d8 = 0;
    self->field_0x3dc = false;
    self->field_0x3ec = 0;
    self->field_0x3f0 = false;
    self->field_0x3c4 = UnkQuad_l982c7c();
    self->field_0x3e8 = false;
    self->field_0x3e9 = false;
    self->field_0x3e0 = 0;
    fn_7100982e34(reinterpret_cast<Unk_l982e34*>(self));
}
