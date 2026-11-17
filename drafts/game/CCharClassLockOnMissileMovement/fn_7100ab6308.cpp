// CCharClassLockOnMissileMovement's unit: the lock-on missile movement's constructor. After the base
// missile movement it sets up its two states and their machine (+0x760), a list of 20 positions, the
// path at +0x870, binds the second state's callbacks and enters state 1. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CMissileMovement.h"
#include "game/UnkXformNode.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

using base::global::CRntVector;

extern char g_unk_7101a95ac0[];  // the movement's vtable
extern char g_unk_7101a95ed0[];  // its states' vtable
extern char g_unk_7101a95f80[];  // its state machine's vtable

struct UnkLockOnMissileMovement;
void fn_7100ab9218(UnkLockOnMissileMovement* self);
void fn_7100ab929c(char* p0);
void fn_7100ab9348();

struct UnkLockOnMachine_lab6308;
struct UnkLockOnState_lab6308;

struct UnkLockOnDelegate_lab6308 {
    void* object;
    void (*function)();
};

struct UnkLockOnStateVtbl_lab6308 {
    void* pad_0x0[2];
    void (*vf_0x10)(UnkLockOnState_lab6308* self, u32 previous_id);  // entered
    void (*vf_0x18)(UnkLockOnState_lab6308* self, u32 next_id);      // left
};

struct UnkLockOnState_lab6308 {
    UnkLockOnStateVtbl_lab6308* vtable;
    UnkLockOnMachine_lab6308* machine;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    UnkLockOnDelegate_lab6308 field_0x30;
    UnkLockOnDelegate_lab6308 field_0x40;
    UnkLockOnDelegate_lab6308 field_0x50;
    u64 field_0x60;
    u64 field_0x68;

    explicit UnkLockOnState_lab6308(u32 id)
        : vtable(reinterpret_cast<UnkLockOnStateVtbl_lab6308*>(g_unk_7101a95ed0 + 0x10)), machine(nullptr),
          id(id), field_0x18(0), field_0x20(0), field_0x28(2), field_0x2c(1), field_0x30(), field_0x40(),
          field_0x50(), field_0x60(0), field_0x68(0) {}
};
static_assert(sizeof(UnkLockOnState_lab6308) == 0x70, "layout");

struct UnkLockOnMachineVtbl_lab6308 {
    void* pad_0x0[4];
    void (*vf_0x20)(UnkLockOnMachine_lab6308* self, UnkLockOnState_lab6308* previous, UnkLockOnState_lab6308* next);
    void (*vf_0x28)(UnkLockOnMachine_lab6308* self, UnkLockOnState_lab6308* current, UnkLockOnState_lab6308* next);
};

struct UnkLockOnMachine_lab6308 {
    static const u32 kNoState = 0x7fffffff;

    UnkLockOnMachineVtbl_lab6308* vtable;
    void* owner;
    UnkLockOnState_lab6308* current;
    UnkLockOnState_lab6308* previous;
    u32 count;
    u32 states[0x20];  // UnkLockOnState_lab6308* entries, 4-byte aligned
    u32 pending;       // 0xa4
    u8 changing;       // 0xa8
    u8 field_0xa9;
    u8 pad_0xaa[0xb0 - 0xaa];

    UnkLockOnMachine_lab6308()
        : vtable(reinterpret_cast<UnkLockOnMachineVtbl_lab6308*>(g_unk_7101a95f80 + 0x10)), owner(nullptr),
          current(nullptr), previous(nullptr), count(0), pending(kNoState), changing(0), field_0xa9(0) {}

    UnkLockOnState_lab6308** States() { return reinterpret_cast<UnkLockOnState_lab6308**>(states); }

    // Registers `state` unless one with its id is registered already.
    void Add(UnkLockOnState_lab6308* state) {
        UnkLockOnState_lab6308** begin = States();
        UnkLockOnState_lab6308** end = begin + count;
        for (UnkLockOnState_lab6308** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }

    // Leaves the current state for the registered one with `id`, unless it is current already.
    void Change(u32 id) {
        if (current && current->id == id)
            return;
        UnkLockOnState_lab6308** begin = States();
        UnkLockOnState_lab6308** end = begin + count;
        UnkLockOnState_lab6308** p = begin;
        for (; p != end; ++p)
            if ((*p)->id == id)
                break;
        if (p == end)
            return;
        UnkLockOnState_lab6308* next = *p;
        if (!next)
            return;
        if (current) {
            current->vtable->vf_0x18(current, id);
            vtable->vf_0x28(this, current, next);
        }
        previous = current;
        current = next;
        u32 previous_id = previous ? previous->id : kNoState;
        next->vtable->vf_0x10(next, previous_id);
        vtable->vf_0x20(this, previous, next);
    }

    // Asks for state `id`; it is entered now unless a change is under way.
    void Request(u32 id) {
        pending = id;
        if (!changing) {
            Change(pending);
            pending = kNoState;
        }
    }
};
static_assert(sizeof(UnkLockOnMachine_lab6308) == 0xb0, "layout");

// The 0x18-byte member at +0x810.
struct UnkLockOnTarget_lab6308 {
    void* field_0x0;
    void* field_0x8;
    u32 field_0x10;
    bool field_0x14;
    bool field_0x15;
};

struct UnkLockOnMissile_lab6308 {
    void* vtable;
    u8 pad_0x8[0x448 - 0x8];
    u32 field_0x448;
    u8 pad_0x44c[0x620 - 0x44c];
    void* field_0x620;
    u8 pad_0x628[0x62c - 0x628];
    u32 field_0x62c[12];
    u8 pad_0x65c[0x680 - 0x65c];
    UnkLockOnState_lab6308 states[2];  // 0x680
    UnkLockOnMachine_lab6308 machine;  // 0x760
    UnkLockOnTarget_lab6308 field_0x810;
    u8 pad_0x828[0x830 - 0x828];
    CRntVector<UnkVec3f> positions;  // 0x830
    void* field_0x858;
    void* field_0x860;
    bool field_0x868;
    u8 pad_0x869[0x870 - 0x869];
    u8 path[0x132f8 - 0x870];  // built by fn_71002acfe4
    bool field_0x132f8;
};
static_assert(__builtin_offsetof(UnkLockOnMissile_lab6308, machine) == 0x760, "layout");
static_assert(__builtin_offsetof(UnkLockOnMissile_lab6308, positions) == 0x830, "layout");
static_assert(__builtin_offsetof(UnkLockOnMissile_lab6308, field_0x132f8) == 0x132f8, "layout");

static inline void Bind(UnkLockOnMissile_lab6308* self, UnkLockOnState_lab6308* state, void (*a)(), void (*b)(),
                        void (*c)()) {
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
}

void fn_7100ab6308(void* movement) {
    auto* self = static_cast<UnkLockOnMissile_lab6308*>(movement);
    fn_7100aba0c0(reinterpret_cast<UnkMissileMovement*>(self));
    self->vtable = g_unk_7101a95ac0 + 0x10;
    self->field_0x620 = nullptr;
    for (u32& word : self->field_0x62c)
        word = 0;
    new (&self->states[0]) UnkLockOnState_lab6308(1);
    new (&self->states[1]) UnkLockOnState_lab6308(2);
    new (&self->machine) UnkLockOnMachine_lab6308();
    self->field_0x810.field_0x0 = nullptr;
    self->field_0x810.field_0x8 = nullptr;
    self->field_0x810.field_0x10 = 0;
    self->field_0x810.field_0x14 = true;
    self->field_0x810.field_0x15 = false;
    new (&self->positions) CRntVector<UnkVec3f>(0xf);
    self->positions.Reserve(20);
    self->field_0x858 = nullptr;
    self->field_0x860 = nullptr;
    self->field_0x868 = false;
    fn_71002acfe4(self->path);
    self->field_0x132f8 = true;

    self->field_0x448 = 0x18;
    self->machine.owner = self;
    self->machine.Add(&self->states[0]);
    self->machine.Add(&self->states[1]);
    Bind(self, &self->states[1], reinterpret_cast<void (*)()>(fn_7100ab9218),
         reinterpret_cast<void (*)()>(fn_7100ab929c), fn_7100ab9348);
    self->machine.Request(1);
}
