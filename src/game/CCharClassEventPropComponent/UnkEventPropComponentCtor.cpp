// Constructors of two components in the CCharClassEventPropComponent unit. fn_71007dcc98 is a near
// copy of fn_7100878d24 (src/game/CCharClassActivatableComponent/CCharClassActivatableComponent.cpp):
// the component base constructor fn_71010c4148, the vtable, the member at +0x40 (fn_71007d762c),
// three cleared flags, then the post-construction virtual call (slot 3). fn_71007db53c builds a
// derived component on that one (inlined): its own vtable, two cleared ranges, two containers on the
// default allocator, then the same virtual call. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CComponent.h"
#include "game/CEmmyWakeUpComponent.h"
#include "types.h"

struct UnkStateObject;
extern void* g_unk_7101a72d70[];
extern void* g_unk_7101a72ba8[];

struct UnkEventPropComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    bool field_0x79;
    bool field_0x7a;
};

static inline __attribute__((always_inline)) void InitEventPropComponent(UnkEventPropComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a72d70 + 2;
    fn_71007d762c(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x78 = false;
    self->field_0x79 = false;
    self->field_0x7a = false;
}

void fn_71007dcc98(UnkEventPropComponent* self) {
    InitEventPropComponent(self);
    reinterpret_cast<void (*)(UnkEventPropComponent*)>(self->vtable[3])(self);
}

struct UnkEventPropDerived : UnkEventPropComponent {
    void* field_0x80;
    void* field_0x88;
    void* field_0x90;
    void* field_0x98;
    u32 field_0xa0;
    u32 field_0xa4;
    base::UnkAllocator* field_0xa8;
    void* field_0xb0;
    s64 field_0xb8;
    u32 field_0xc0;
    bool field_0xc4;
    bool field_0xc5;
    bool field_0xc6;
    void* field_0xc8;
    void* field_0xd0;
    u32 field_0xd8;
    bool field_0xdc;
    u32 field_0xe0;
    base::UnkAllocator* field_0xe8;
    bool field_0xf0;
    bool field_0xf1;
};

void fn_71007db53c(UnkEventPropDerived* self) {
    InitEventPropComponent(self);
    self->vtable = g_unk_7101a72ba8 + 2;
    self->field_0x80 = nullptr;
    self->field_0x88 = nullptr;
    self->field_0x90 = nullptr;
    self->field_0x98 = nullptr;
    self->field_0xa0 = 10;
    self->field_0xa4 = 14;
    self->field_0xa8 = fn_7100083990();
    self->field_0xb0 = nullptr;
    self->field_0xb8 = -1;
    self->field_0xc0 = 0;
    self->field_0xc4 = false;
    self->field_0xc5 = false;
    self->field_0xc6 = false;
    self->field_0xc8 = nullptr;
    self->field_0xd0 = nullptr;
    self->field_0xd8 = 10;
    self->field_0xdc = true;
    self->field_0xe0 = 15;
    self->field_0xe8 = fn_7100083990();
    self->field_0xf0 = true;
    self->field_0xf1 = true;
    reinterpret_cast<void (*)(UnkEventPropDerived*)>(self->vtable[3])(self);
}
