// CMapAcquisitionComponent unit (tools/tu_map.py): fn_7100a854fc runs the usable component's state setup,
// registers the component's four states on its machine (+0x548) and binds their callbacks, then fills the
// registry map at +0x7c0 under the owner's name. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

using base::global::CStrId;

struct UnkRefl_fn_7100a85774;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_7100a85774>(
    base::reflection::UnkReflRegistryMap*, const base::global::CStrId&);

struct UnkStateObject;
void fn_7100a9ea64(UnkStateObject* self);

void fn_7100a884c8();
void fn_7100a884cc();
void fn_7100a884d0();
void fn_7100a884d4();
void fn_7100a884d8();
void fn_7100a884dc();
void fn_7100a884e0();
void fn_7100a884e4();
void fn_7100a8858c();
void fn_7100a88600();
void fn_7100a88604();
void fn_7100a88608();

struct UnkMapMachine_la854fc;

struct UnkMapDelegate_la854fc {
    void* object;
    void (*function)();
};

struct UnkMapState_la854fc {
    void** vtable;
    UnkMapMachine_la854fc* machine;
    u32 id;
    u8 pad_0x14[0x30 - 0x14];
    UnkMapDelegate_la854fc field_0x30;
    UnkMapDelegate_la854fc field_0x40;
    UnkMapDelegate_la854fc field_0x50;
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkMapState_la854fc) == 0x70, "layout");

struct UnkMapMachine_la854fc {
    void** vtable;
    void* owner;
    UnkMapState_la854fc* current;
    UnkMapState_la854fc* previous;
    u32 count;
    u32 states[0x20];  // UnkMapState_la854fc* entries, 4-byte aligned
    UnkMapState_la854fc** States() { return reinterpret_cast<UnkMapState_la854fc**>(states); }
    u32 pending;
    bool busy;
    bool field_0xa9;

    // Registers `state` unless one with its id is registered already.
    void Add(UnkMapState_la854fc* state) {
        UnkMapState_la854fc** begin = States();
        UnkMapState_la854fc** end = begin + count;
        for (UnkMapState_la854fc** p = begin; p != end; ++p)
            if ((*p)->id == state->id)
                return;
        state->machine = this;
        *end = state;
        ++count;
    }
};
static_assert(sizeof(UnkMapMachine_la854fc) == 0xb0, "layout");

struct UnkMapOwnerInfo_la854fc {
    u8 pad_0x0[0x30];
    CStrId name;
};

struct UnkMapAcquisition_la854fc {
    u8 pad_0x0[0x18];
    UnkMapOwnerInfo_la854fc* info;
    u8 pad_0x20[0x548 - 0x20];
    UnkMapMachine_la854fc machine;  // 0x548
    UnkMapState_la854fc states[4];  // 0x5f8
    u8 pad_0x7b8[0x7c0 - 0x7b8];
    base::reflection::UnkReflRegistryMap registry;  // 0x7c0
};
static_assert(__builtin_offsetof(UnkMapAcquisition_la854fc, states) == 0x5f8, "layout");
static_assert(__builtin_offsetof(UnkMapAcquisition_la854fc, registry) == 0x7c0, "layout");

static inline void Bind(UnkMapAcquisition_la854fc* self, UnkMapState_la854fc* state, void (*a)(), void (*b)(),
                        void (*c)()) {
    self->machine.Add(state);
    state->field_0x30 = {self, a};
    state->field_0x50 = {self, b};
    state->field_0x40 = {self, c};
}

void fn_7100a854fc(UnkMapAcquisition_la854fc* self) {
    fn_7100a9ea64(reinterpret_cast<UnkStateObject*>(self));
    self->machine.owner = self;
    Bind(self, &self->states[0], fn_7100a884c8, fn_7100a884cc, fn_7100a884d0);
    Bind(self, &self->states[1], fn_7100a884d4, fn_7100a884d8, fn_7100a884dc);
    Bind(self, &self->states[2], fn_7100a884e0, fn_7100a884e4, fn_7100a8858c);
    Bind(self, &self->states[3], fn_7100a88600, fn_7100a88604, fn_7100a88608);

    base::reflection::refl_RegistryMapInit<UnkRefl_fn_7100a85774>(&self->registry, CStrId(self->info->name));
}
