// CAccessPointComponent unit (tools/tu_map.py, unit 1581): the access point's constructor. After the usable
// component's constructor it builds a callback set, a chained map of 11 buckets, the state machine with its
// four states, a listener, two arrays, the registry map and an empty string id. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101a8fe10[];
extern void* g_unk_7101a90320[];
extern void* g_unk_7101a90270[];
void fn_7100a73c74();
void fn_7100a72a28(char* self);  // base::refl_StrIdListCtor<..>, instantiated in src/clones
char* fn_7100065efc(char* self);

struct UnkApCallbacks_la6c9f4 {
    void** vtable;
    CRntVector<void*> items;  // 0x8
    u32 field_0x30;
    u64 field_0x38;
    CStrId id_0x40;
    CStrId id_0x48;
    u16 field_0x50;
    CStrId id_0x58;
};
static_assert(sizeof(UnkApCallbacks_la6c9f4) == 0x60, "layout");

struct UnkApDelegate_la6c9f4 {
    void* object;
    void (*function)();
};

struct UnkApMachine_la6c9f4;

struct UnkApState_la6c9f4 {
    void** vtable;
    UnkApMachine_la6c9f4* machine;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    UnkApDelegate_la6c9f4 field_0x30;
    UnkApDelegate_la6c9f4 field_0x40;
    UnkApDelegate_la6c9f4 field_0x50;
    u64 field_0x60;
    u64 field_0x68;
};
static_assert(sizeof(UnkApState_la6c9f4) == 0x70, "layout");

struct UnkApMachine_la6c9f4 {
    void** vtable;
    void* owner;
    UnkApState_la6c9f4* current;
    UnkApState_la6c9f4* previous;
    u32 count;
    u32 states[0x20];  // UnkApState_la6c9f4* entries, 4-byte aligned
    u32 pending;
    bool busy;
    bool field_0xa9;
    u8 pad_0xaa[0xb8 - 0xaa];
};
static_assert(sizeof(UnkApMachine_la6c9f4) == 0xb8, "layout");

struct UnkAp_la6c9f4 {
    void** vtable;
    u8 pad_0x8[0x540 - 0x8];
    UnkApCallbacks_la6c9f4 callbacks;  // 0x540
    UnkChainedMap map;  // 0x5a0
    UnkApMachine_la6c9f4 machine;  // 0x5e8
    UnkApState_la6c9f4 states[4];  // 0x6a0
    u64 field_0x860;
    UnkApDelegate_la6c9f4 field_0x868;
    void** listener_vtable;  // 0x878
    u64 field_0x880;
    u64 field_0x888;
    u64 field_0x890;
    CRntVector<void*> listener_items;  // 0x898
    CRntVector<void*> list;  // 0x8c0
    u64 field_0x8e8;
    s64 field_0x8f0;
    u64 field_0x8f8;
    s64 field_0x900;
    u32 field_0x908;
    f32 field_0x90c;
    u32 field_0x910;
    u32 field_0x914;
    u32 field_0x918;
    bool field_0x91c;
    bool field_0x91d;
    f32 field_0x920;
    base::reflection::UnkReflRegistryMap registry;  // 0x928
    u8 pad_0x958[0x960 - 0x958];
    u64 field_0x960;
    u64 field_0x968;
    u32 field_0x970;
    f32 field_0x974;
    u8 field_0x978[0x18];
    CStrId name;  // 0x990
    bool field_0x998;
};
static_assert(__builtin_offsetof(UnkAp_la6c9f4, map) == 0x5a0, "layout");

static_assert(__builtin_offsetof(UnkAp_la6c9f4, field_0x918) == 0x918, "layout");
static_assert(__builtin_offsetof(UnkAp_la6c9f4, registry) == 0x928, "layout");
static_assert(__builtin_offsetof(UnkAp_la6c9f4, field_0x998) == 0x998, "layout");

static inline void InitState(UnkApState_la6c9f4* state, u32 id) {
    state->machine = nullptr;
    state->id = id;
    state->field_0x18 = 0;
    state->field_0x20 = 0;
    state->field_0x28 = 2;
    state->field_0x2c = 1;
    state->vtable = g_unk_7101a90270 + 2;
    state->field_0x30 = {nullptr, nullptr};
    state->field_0x40 = {nullptr, nullptr};
    state->field_0x50 = {nullptr, nullptr};
    state->field_0x60 = 0;
    state->field_0x68 = 0;
}

// Typed parameter: fn_7100a6b000 (CAccessPointCommanderComponent_Construct_la6b000.cpp) declares it with void*,
// which changes how the member addresses are formed; that declaration takes this type instead.
void fn_7100a6c9f4(UnkAp_la6c9f4* self) {
    fn_7100a9e244(self);
    self->vtable = g_unk_7101a8fe10 + 2;

    UnkApCallbacks_la6c9f4& callbacks = self->callbacks;
    callbacks.vtable = g_unk_7101a20998 + 2;
    new (&callbacks.items) CRntVector<void*>(15);
    callbacks.field_0x30 = 0;
    callbacks.vtable = g_unk_7101a33fa0 + 2;
    callbacks.field_0x38 = 0;
    new (&callbacks.id_0x40) CStrId(g_unk_710162e2ed, true);
    new (&callbacks.id_0x48) CStrId(g_unk_710162e2ed, true);
    callbacks.field_0x50 = 0;
    new (&callbacks.id_0x58) CStrId(g_unk_7101d470e0);

    new (&self->map) UnkChainedMap;
    self->map.ResizeBuckets(11);

    UnkApMachine_la6c9f4& machine = self->machine;
    machine.owner = nullptr;
    machine.vtable = g_unk_7101a90320 + 2;
    machine.current = nullptr;
    machine.previous = nullptr;
    machine.count = 0;
    machine.pending = 5;
    machine.busy = false;
    machine.field_0xa9 = false;

    InitState(&self->states[0], 1);
    InitState(&self->states[1], 2);
    InitState(&self->states[2], 4);
    InitState(&self->states[3], 3);

    self->field_0x860 = 0;
    self->field_0x868 = {self, fn_7100a73c74};

    self->listener_vtable = g_unk_7101a22d48 + 2;
    self->field_0x880 = 0;
    self->field_0x888 = 0;
    self->field_0x890 = 0;
    new (&self->listener_items) CRntVector<void*>(15);
    new (&self->list) CRntVector<void*>(15);

    self->field_0x8e8 = 0;
    self->field_0x8f0 = -1;
    self->field_0x8f8 = 0;
    self->field_0x900 = -1;
    self->field_0x908 = 0;
    self->field_0x90c = 0.5f;
    self->field_0x910 = 0;
    self->field_0x918 = 0;
    self->field_0x91c = false;
    self->field_0x91d = true;
    self->field_0x920 = 1.0f;
    fn_7100a72a28(reinterpret_cast<char*>(&self->registry));
    self->field_0x960 = 0;
    self->field_0x968 = 0;
    self->field_0x970 = 0;
    self->field_0x974 = 1.0f;
    fn_7100065efc(reinterpret_cast<char*>(self->field_0x978));

    self->name.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name.data = &g_unk_7101d47108->empty;
    self->field_0x998 = false;
}
