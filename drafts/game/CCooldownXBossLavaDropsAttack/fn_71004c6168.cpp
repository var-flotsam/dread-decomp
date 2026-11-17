// CCooldownXBossLavaDropsAttack's unit (tools/tu_map.py): the attack's constructor. After the attack base's
// constructor it builds the state machine at +0x68 with its four states, a listener at +0x328, registers the
// states with their callbacks and asks the machine for state 1. Names and layouts are placeholders.
#include <new>

#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCooldownXBossAttack.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern void* g_unk_7101a3e6c0[];  // the attack's vtable
extern void* g_unk_7101a3e878[];  // the member's vtable
extern void* g_unk_7101a3e7c8[];  // the states' vtable
extern void* g_unk_7101a3e820[];  // the states' base vtable

void fn_71004c92f4();
void fn_71004c92fc();
void fn_71004c9300();
void fn_71004c9304();
void fn_71004c9308();
void fn_71004c930c();
void fn_71004c933c();
void fn_71004c9340();
void fn_71004c9350();
void fn_71004c9354();
void fn_71004c9358();
void fn_71004c935c();

struct UnkLavaDropsMachine_l4c6168;

struct UnkLavaDropsDelegate_l4c6168 {
    void* object;
    void (*function)();
    UnkLavaDropsDelegate_l4c6168() : object(nullptr), function(nullptr) {}
};

struct UnkLavaDropsState_l4c6168;
struct UnkLavaDropsStateVtable_l4c6168 {
    void* unk_0x0[2];
    void (*entered)(UnkLavaDropsState_l4c6168* self, s32 previous);  // 0x10
    void (*left)(UnkLavaDropsState_l4c6168* self, s32 next);         // 0x18
};

struct UnkLavaDropsStateBase_l4c6168 {
    UnkLavaDropsStateVtable_l4c6168* vtable;
    UnkLavaDropsMachine_l4c6168* machine;
    u32 id;
    base::reflection::UnkReflValue value;  // 0x18

    explicit UnkLavaDropsStateBase_l4c6168(u32 id_) : machine(nullptr), id(id_) {
        vtable = reinterpret_cast<UnkLavaDropsStateVtable_l4c6168*>(g_unk_7101a3e820 + 2);
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
    }
};

struct UnkLavaDropsState_l4c6168 : UnkLavaDropsStateBase_l4c6168 {
    UnkLavaDropsDelegate_l4c6168 field_0x30;
    UnkLavaDropsDelegate_l4c6168 field_0x40;
    UnkLavaDropsDelegate_l4c6168 field_0x50;
    UnkLavaDropsDelegate_l4c6168 field_0x60;

    explicit UnkLavaDropsState_l4c6168(u32 id_) : UnkLavaDropsStateBase_l4c6168(id_) {
        vtable = reinterpret_cast<UnkLavaDropsStateVtable_l4c6168*>(g_unk_7101a3e7c8 + 2);
    }
};
static_assert(sizeof(UnkLavaDropsState_l4c6168) == 0x70, "layout");

struct UnkLavaDropsMachineVtable_l4c6168 {
    void* unk_0x0[4];
    void (*entered)(UnkLavaDropsMachine_l4c6168* self, UnkLavaDropsState_l4c6168* previous,
                    UnkLavaDropsState_l4c6168* next);  // 0x20
    void (*left)(UnkLavaDropsMachine_l4c6168* self, UnkLavaDropsState_l4c6168* current,
                 UnkLavaDropsState_l4c6168* next);  // 0x28
};

struct UnkLavaDropsMachine_l4c6168 {
    UnkLavaDropsMachineVtable_l4c6168* vtable;
    void* owner;
    UnkLavaDropsState_l4c6168* current;
    UnkLavaDropsState_l4c6168* previous;
    u32 count;
    u32 states[0x20];  // UnkLavaDropsState_l4c6168* entries, 4-byte aligned
    UnkLavaDropsState_l4c6168** States() { return reinterpret_cast<UnkLavaDropsState_l4c6168**>(states); }
    u32 pending;
    bool busy;
    bool field_0xa9;

    // Registers `state` unless one with its id is registered already.
    void Add(UnkLavaDropsState_l4c6168* state) {
        UnkLavaDropsState_l4c6168** begin = States();
        UnkLavaDropsState_l4c6168** end = begin + count;
        for (UnkLavaDropsState_l4c6168** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    UnkLavaDropsState_l4c6168* Find(u32 id) {
        UnkLavaDropsState_l4c6168** begin = States();
        UnkLavaDropsState_l4c6168** end = begin + count;
        for (UnkLavaDropsState_l4c6168** p = begin; p != end; ++p)
            if ((*p)->id == id)
                return *p;
        return nullptr;
    }

    void Request(u32 id) {
        pending = id;
        if (busy)
            return;
        UnkLavaDropsState_l4c6168* old = current;
        if (!old || old->id != id) {
            if (UnkLavaDropsState_l4c6168* next = Find(id)) {
                if (old) {
                    old->vtable->left(old, id);
                    vtable->left(this, current, next);
                }
                previous = current;
                current = next;
                next->vtable->entered(next, previous ? previous->id : 5);
                vtable->entered(this, previous, next);
            }
        }
        pending = 5;
    }
};
static_assert(sizeof(UnkLavaDropsMachine_l4c6168) == 0xb0, "layout");

struct UnkLavaDropsListener_l4c6168 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
};

struct UnkLavaDropsAttack_l4c6168 {
    void** vtable;
    u8 pad_0x8[0x60];
    UnkLavaDropsMachine_l4c6168 machine;  // 0x68
    u64 field_0x118;
    UnkLavaDropsState_l4c6168 states[4];  // 0x120
    u64 field_0x2e0;
    u64 field_0x2e8;
    UnkVec3f field_0x2f0;
    u8 pad_0x2fc[0x24];
    void* field_0x320;
    UnkLavaDropsListener_l4c6168 listener;  // 0x328
};
static_assert(__builtin_offsetof(UnkLavaDropsAttack_l4c6168, listener) == 0x328, "layout");

static inline void Bind(UnkLavaDropsAttack_l4c6168* self, UnkLavaDropsState_l4c6168* state, void (*a)(),
                        void (*b)(), void (*c)()) {
    self->machine.Add(state);
    state->field_0x30.object = self;
    state->field_0x30.function = a;
    state->field_0x50.object = self;
    state->field_0x50.function = b;
    state->field_0x40.object = self;
    state->field_0x40.function = c;
}

struct UnkNode;

void fn_71004c6168(UnkLavaDropsAttack_l4c6168* self) {
    fn_71004c1894(reinterpret_cast<UnkNode*>(self));
    self->vtable = g_unk_7101a3e6c0 + 2;

    UnkLavaDropsMachine_l4c6168& machine = self->machine;
    machine.vtable = reinterpret_cast<UnkLavaDropsMachineVtable_l4c6168*>(g_unk_7101a3e878 + 2);
    machine.current = nullptr;
    machine.previous = nullptr;
    machine.count = 0;
    machine.pending = 5;
    machine.busy = false;
    machine.field_0xa9 = false;

    self->field_0x118 = 0;
    new (&self->states[0]) UnkLavaDropsState_l4c6168(1);
    new (&self->states[1]) UnkLavaDropsState_l4c6168(2);
    new (&self->states[2]) UnkLavaDropsState_l4c6168(3);
    new (&self->states[3]) UnkLavaDropsState_l4c6168(4);
    self->field_0x2e0 = 0;
    self->field_0x2e8 = 0;
    self->field_0x2f0.x = 0.0f;
    self->field_0x2f0.y = 0.0f;
    self->field_0x2f0.z = 0.0f;
    self->field_0x320 = nullptr;
    self->listener.vtable = g_unk_7101a22d48 + 2;
    self->listener.field_0x8 = 0;
    self->listener.field_0x10 = 0;
    self->listener.field_0x18 = 0;
    self->listener.field_0x20 = 0;

    machine.owner = self;
    Bind(self, &self->states[0], fn_71004c92f4, fn_71004c92fc, fn_71004c9300);
    Bind(self, &self->states[1], fn_71004c9304, fn_71004c9308, fn_71004c930c);
    Bind(self, &self->states[2], fn_71004c933c, fn_71004c9340, fn_71004c9350);
    Bind(self, &self->states[3], fn_71004c9354, fn_71004c9358, fn_71004c935c);
    machine.Request(1);
}
