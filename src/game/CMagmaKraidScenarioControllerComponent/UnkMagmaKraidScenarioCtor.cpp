// Constructor of a component in the CMagmaKraidScenarioControllerComponent unit: the component base
// constructor fn_71010c4148, the vtable, two containers at +0x40 and +0x80 (base vtable, empty
// array with step 10 and flags 0xf, final vtable, allocator), then the post-construction virtual
// call (slot 3). The container is laid out as in CAutsharpSpawnPointComponent.cpp. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkStateObject;
void fn_71010c4148(UnkStateObject* self);
extern void* g_unk_7101a81aa0[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];

struct UnkMagmaKraidContainer {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    base::UnkAllocator* field_0x28;
    u32 field_0x30;
    u64 field_0x38;
};

static inline __attribute__((always_inline)) void InitContainer(UnkMagmaKraidContainer* c) {
    c->vtable = g_unk_7101a20998 + 2;
    c->field_0x8 = 0;
    c->field_0x10 = 0;
    c->field_0x18 = 10;
    c->field_0x1c = true;
    c->field_0x20 = 0xf;
    c->field_0x28 = fn_7100083990();
    c->field_0x30 = 0;
    c->vtable = g_unk_7101a33fa0 + 2;
    c->field_0x38 = 0;
}

struct UnkMagmaKraidScenarioController {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkMagmaKraidContainer field_0x40;
    UnkMagmaKraidContainer field_0x80;
};

void fn_7100917dcc(UnkMagmaKraidScenarioController* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a81aa0 + 2;
    InitContainer(&self->field_0x40);
    InitContainer(&self->field_0x80);
    reinterpret_cast<void (*)(UnkMagmaKraidScenarioController*)>(self->vtable[3])(self);
}
