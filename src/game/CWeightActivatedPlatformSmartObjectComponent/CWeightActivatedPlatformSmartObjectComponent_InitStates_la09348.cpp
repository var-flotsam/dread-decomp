// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): fn_7100a09348 sets up the
// platform's state machine (+0x180): registers its seven states (each once, by id) and binds their callbacks
// to the platform, fills the registry map at +0x560 under the owner's name, and caches a name's hash at +0x5a0.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

using base::global::CStrId;

struct UnkRefl_fn_7100a09710;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_7100a09710>(
    base::reflection::UnkReflRegistryMap*, const base::global::CStrId&);

struct UnkSmTimer_A6_b62;
void fn_7100a0d4f4(UnkSmTimer_A6_b62* self);
void fn_7100a0d618();
void fn_7100a0d61c();
void fn_7100a0d620();
void fn_7100a0d624();
void fn_7100a0d628();
void fn_7100a0d668();
void fn_7100a0d66c();
void fn_7100a0d670();
void fn_7100a0d688();
void fn_7100a0d68c();
void fn_7100a0d690();
void fn_7100a0d6d8();
void fn_7100a0d6dc();
void fn_7100a0d6e0();
void fn_7100a0d6f8();
void fn_7100a0d704();
void fn_7100a0d708();
void fn_7100a0d714();

struct UnkWapMachine_la09348;

struct UnkWapDelegate_la09348 {
    void* object;
    void (*function)();
};

struct UnkWapState_la09348 {
    void** vtable;
    UnkWapMachine_la09348* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkWapDelegate_la09348 field_0x30;
    UnkWapDelegate_la09348 field_0x40;
    UnkWapDelegate_la09348 field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkWapState_la09348) == 0x70, "layout");

struct UnkWapMachine_la09348 {
    void** vtable;
    void* owner;
    UnkWapState_la09348* current;
    UnkWapState_la09348* previous;
    u32 count;
    u32 states[0x20];  // UnkWapState_la09348* entries, 4-byte aligned
    UnkWapState_la09348** States() { return reinterpret_cast<UnkWapState_la09348**>(states); }
    u32 pending;
    bool busy;

    // Registers `state` unless one with its id is registered already.
    void Add(UnkWapState_la09348* state) {
        UnkWapState_la09348** begin = States();
        UnkWapState_la09348** end = begin + count;
        for (UnkWapState_la09348** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};

struct UnkWapOwnerInfo_la09348 {
    u8 pad_0x0[0x30];
    CStrId name;
};

struct UnkWapNamed_la09348 {
    u8 pad_0x0[0x58];
    CStrId name;
};

struct UnkWap_la09348 {
    u8 pad_0x0[0x18];
    UnkWapOwnerInfo_la09348* info;
    u8 pad_0x20[0x180 - 0x20];
    UnkWapMachine_la09348 machine;  // 0x180
    UnkWapState_la09348 states[7];  // 0x230
    u8 pad_0x540[0x560 - 0x540];
    base::reflection::UnkReflRegistryMap registry;  // 0x560
    u8 pad_0x590[0x598 - 0x590];
    UnkWapNamed_la09348* named;  // 0x598
    u64 name_hash;  // 0x5a0
};
static_assert(__builtin_offsetof(UnkWap_la09348, states) == 0x230, "layout");
static_assert(__builtin_offsetof(UnkWap_la09348, registry) == 0x560, "layout");
static_assert(__builtin_offsetof(UnkWap_la09348, name_hash) == 0x5a0, "layout");

static inline void Bind(UnkWap_la09348* self, UnkWapState_la09348* state, void (*a)(), void (*b)(), void (*c)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
}

void fn_7100a09348(UnkWap_la09348* self) {
    self->machine.owner = self;
    Bind(self, &self->states[0], reinterpret_cast<void (*)()>(fn_7100a0d4f4), fn_7100a0d618, fn_7100a0d61c);
    Bind(self, &self->states[1], fn_7100a0d620, fn_7100a0d624, fn_7100a0d628);
    Bind(self, &self->states[2], fn_7100a0d668, fn_7100a0d66c, fn_7100a0d670);
    Bind(self, &self->states[3], fn_7100a0d688, fn_7100a0d68c, fn_7100a0d690);
    Bind(self, &self->states[4], fn_7100a0d6d8, fn_7100a0d6dc, fn_7100a0d6e0);
    Bind(self, &self->states[6], fn_7100a0d6f8, fn_7100a0d704, fn_7100a0d708);
    self->machine.Add(&self->states[5]);
    self->states[5].field_0x30 = {self, fn_7100a0d714};

    base::reflection::refl_RegistryMapInit<UnkRefl_fn_7100a09710>(&self->registry, CStrId(self->info->name));
    self->name_hash = self->named->name.data->Key();
}
