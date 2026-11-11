// CEmmyWakeUpComponent: its constructor (fn_71007d7724). The component base constructor
// fn_71010c4148, the base class's vtable, the string-id list at +0x40 (fn_71007d762c), then its own
// vtable, three cleared flags at +0x78, an allocator-backed object at +0x80 (base and own vtables,
// three small defaults, the allocator from fn_7100083990), an object at +0xb8 (its vtable and three
// cleared words), then the object's own vf_0x18. Names and layouts are placeholders.
#include "game/CComponent.h"
#include "game/CEmmyWakeUpComponent.h"
#include "types.h"

#include "base/UnkVector.h"

struct UnkStateObject;
extern void* g_unk_7101a72d70[];
extern void* g_unk_7101a72a18[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a22d48[];

struct UnkEmmyWakeUpComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    bool field_0x79;
    bool field_0x7a;
    u8 pad_0x7b[5];
    void** field_0x80;
    u64 field_0x88;
    u64 field_0x90;
    u32 field_0x98;
    bool field_0x9c;
    u32 field_0xa0;
    base::UnkAllocator* field_0xa8;
    u32 field_0xb0;
    u64 field_0xb8;
    void** field_0xc0;
    u64 field_0xc8;
    u64 field_0xd0;
    u64 field_0xd8;
};

void fn_71007d7724(UnkEmmyWakeUpComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a72d70 + 2;
    fn_71007d762c(reinterpret_cast<char*>(self->field_0x40));
    self->vtable = g_unk_7101a72a18 + 2;
    self->field_0x78 = false;
    self->field_0x79 = false;
    self->field_0x7a = false;
    self->field_0x80 = g_unk_7101a20998 + 2;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    self->field_0x98 = 10;
    self->field_0x9c = true;
    self->field_0xa0 = 15;
    self->field_0xa8 = fn_7100083990();
    self->field_0xb0 = 0;
    self->field_0x80 = g_unk_7101a20950 + 2;
    self->field_0xb8 = 0;
    self->field_0xc0 = g_unk_7101a22d48 + 2;
    __builtin_memset(&self->field_0xc8, 0, 24);
    reinterpret_cast<void (*)(UnkEmmyWakeUpComponent*)>(self->vtable[3])(self);
}
