// CMagnetSlidingBlockComponent's unit (tools/tu_map.py 1412): the component's constructor. After the base
// component's constructor it builds the five sliding states and their state machine, the embedded object at
// +0x358, the tunables, an empty string id and the two callback lists. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a82960[];
extern void* g_unk_7101a82ba8[];  // the states' vtable
extern void* g_unk_7101a82c58[];
void fn_71009216d8(char* self);  // base::refl_StrIdListCtor<..>, instantiated in src/clones

struct UnkMagnetDelegate_l91e260 {
    void* object;
    void (*function)();
};

struct UnkMagnetMachine_l91e260;

struct UnkMagnetState_l91e260 {
    void** vtable;
    UnkMagnetMachine_l91e260* machine;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    UnkMagnetDelegate_l91e260 field_0x30;
    UnkMagnetDelegate_l91e260 field_0x40;
    UnkMagnetDelegate_l91e260 field_0x50;
    u64 field_0x60;
    u64 field_0x68;
};
static_assert(sizeof(UnkMagnetState_l91e260) == 0x70, "layout");

struct UnkMagnetMachine_l91e260 {
    void** vtable;
    void* owner;
    UnkMagnetState_l91e260* current;
    UnkMagnetState_l91e260* previous;
    u32 count;
    u32 states[0x20];  // UnkMagnetState_l91e260* entries, 4-byte aligned
    u32 pending;
    bool busy;
    bool field_0xa9;
};
static_assert(sizeof(UnkMagnetMachine_l91e260) == 0xb0, "layout");

// A list of callbacks (0x40 bytes) whose derived type also holds a ref-counted owner.
struct UnkMagnetCallbackList_l91e260 {
    void** vtable;
    void* data;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    base::UnkAllocator* allocator;
    u32 field_0x30;
    void* owner;

    void Init() {
        vtable = g_unk_7101a20998 + 2;
        data = nullptr;
        count = 0;
        capacity = 0;
        field_0x18 = 10;
        field_0x1c = true;
        field_0x20 = 15;
        allocator = fn_7100083990();
        field_0x30 = 0;
        vtable = g_unk_7101a33fa0 + 2;
        owner = nullptr;
    }
};
static_assert(sizeof(UnkMagnetCallbackList_l91e260) == 0x40, "layout");

struct UnkMagnetSlidingBlock_l91e260 {
    void** vtable;
    u8 pad_0x8[0x40 - 0x8];
    void* char_class;                        // 0x40
    bool field_0x48;                         // 0x48
    UnkMagnetState_l91e260 states[5];        // 0x50
    UnkMagnetMachine_l91e260 machine;        // 0x280
    u64 field_0x330;
    u32 field_0x338;
    u8 pad_0x33c[4];
    bool field_0x340;
    u32 field_0x344;
    u32 field_0x348;
    u32 field_0x34c;
    u32 field_0x350;
    u8 pad_0x354[4];
    u8 field_0x358[0xb0];  // built by fn_710002f4e4
    f32 field_0x408;
    f32 field_0x40c;
    f32 field_0x410;
    f32 field_0x414;
    CStrId field_0x418;
    bool field_0x420;
    UnkMagnetCallbackList_l91e260 field_0x428;
    char field_0x468[0x38];  // a string-id list
    UnkMagnetCallbackList_l91e260 field_0x4a0;
    u64 field_0x4e0;
    s64 field_0x4e8;
};
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91e260, machine) == 0x280, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91e260, field_0x350) == 0x350, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91e260, field_0x408) == 0x408, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91e260, field_0x418) == 0x418, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l91e260, field_0x4e8) == 0x4e8, "layout");

static inline void InitState(UnkMagnetState_l91e260* state, u32 id) {
    state->machine = nullptr;
    state->id = id;
    state->field_0x18 = 0;
    state->field_0x20 = 0;
    state->field_0x28 = 2;
    state->field_0x2c = 1;
    state->vtable = g_unk_7101a82ba8 + 2;
    state->field_0x30 = {nullptr, nullptr};
    state->field_0x40 = {nullptr, nullptr};
    state->field_0x50 = {nullptr, nullptr};
    state->field_0x60 = 0;
    state->field_0x68 = 0;
}

void fn_710091e260(UnkMagnetSlidingBlock_l91e260* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a82960 + 2;
    self->char_class = nullptr;
    self->field_0x48 = false;

    InitState(&self->states[0], 1);
    InitState(&self->states[1], 2);
    InitState(&self->states[2], 3);
    InitState(&self->states[3], 4);
    InitState(&self->states[4], 5);

    UnkMagnetMachine_l91e260& machine = self->machine;
    machine.vtable = g_unk_7101a82c58 + 2;
    machine.owner = nullptr;
    machine.current = nullptr;
    machine.previous = nullptr;
    machine.count = 0;
    machine.pending = 6;
    machine.busy = false;
    machine.field_0xa9 = false;

    self->field_0x330 = 0;
    self->field_0x338 = 0;
    self->field_0x340 = false;
    self->field_0x344 = 0;
    self->field_0x348 = 0;
    self->field_0x350 = 0;
    fn_710002f4e4(self->field_0x358);
    self->field_0x408 = 1.0f;
    self->field_0x40c = 1.0f;
    self->field_0x410 = 0.0f;
    self->field_0x414 = 1.0f;

    self->field_0x418.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x418.data = &g_unk_7101d47108->empty;
    self->field_0x420 = false;

    self->field_0x428.Init();
    fn_71009216d8(self->field_0x468);
    self->field_0x4a0.Init();
    self->field_0x4e0 = 0;
    self->field_0x4e8 = -1;
}