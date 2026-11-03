// Constructor of a component in the CWaterTriggerChangeComponent unit: the component base
// constructor fn_71010c4148, the vtable, cleared fields from +0x40, the member at +0x68
// (fn_7100a68c38), three containers at +0xa8, +0xe8 and +0x128 (base vtable, empty array with step
// 10 and flags 0xf, allocator, final vtable), a set flag at +0x1a0 and cleared words after it, then
// the post-construction virtual call (slot 3). The containers' fields are written out flat (as
// sub-objects, Clang addresses a byte store from the container's base). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkStateObject;
void fn_71010c4148(UnkStateObject* self);
void fn_7100a68c38(char* self);
extern void* g_unk_7101a8f860[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];

struct UnkWaterTriggerChangeComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    u32 field_0x40;
    bool field_0x44;
    void* field_0x48;
    void* field_0x50;
    void* field_0x58;
    void* field_0x60;
    u8 field_0x68[0x38];
    void* field_0xa0;
    void** field_0xa8;
    u64 field_0xb0;
    u64 field_0xb8;
    u32 field_0xc0;
    bool field_0xc4;
    u32 field_0xc8;
    base::UnkAllocator* field_0xd0;
    u32 field_0xd8;
    u64 field_0xe0;
    void** field_0xe8;
    u64 field_0xf0;
    u64 field_0xf8;
    u32 field_0x100;
    bool field_0x104;
    u32 field_0x108;
    base::UnkAllocator* field_0x110;
    u32 field_0x118;
    u64 field_0x120;
    void** field_0x128;
    u64 field_0x130;
    u64 field_0x138;
    u32 field_0x140;
    bool field_0x144;
    u32 field_0x148;
    base::UnkAllocator* field_0x150;
    u32 field_0x158;
    u64 field_0x160;
    u8 pad_0x168[0x38];
    bool field_0x1a0;
    void* field_0x1a8;
    void* field_0x1b0;
};

void fn_7100a68e8c(UnkWaterTriggerChangeComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a8f860 + 2;
    self->field_0x40 = 0;
    self->field_0x44 = false;
    self->field_0x48 = nullptr;
    self->field_0x50 = nullptr;
    self->field_0x58 = nullptr;
    self->field_0x60 = nullptr;
    fn_7100a68c38(reinterpret_cast<char*>(self->field_0x68));
    self->field_0xa0 = nullptr;
    self->field_0xa8 = g_unk_7101a20998 + 2;
    self->field_0xb0 = 0;
    self->field_0xb8 = 0;
    self->field_0xc0 = 10;
    self->field_0xc4 = true;
    self->field_0xc8 = 0xf;
    self->field_0xd0 = fn_7100083990();
    self->field_0xd8 = 0;
    self->field_0xa8 = g_unk_7101a33fa0 + 2;
    self->field_0xe0 = 0;
    self->field_0xe8 = g_unk_7101a20998 + 2;
    self->field_0xf0 = 0;
    self->field_0xf8 = 0;
    self->field_0x100 = 10;
    self->field_0x104 = true;
    self->field_0x108 = 0xf;
    self->field_0x110 = fn_7100083990();
    self->field_0x118 = 0;
    self->field_0xe8 = g_unk_7101a33fa0 + 2;
    self->field_0x120 = 0;
    self->field_0x128 = g_unk_7101a20998 + 2;
    self->field_0x130 = 0;
    self->field_0x138 = 0;
    self->field_0x140 = 10;
    self->field_0x144 = true;
    self->field_0x148 = 0xf;
    self->field_0x150 = fn_7100083990();
    self->field_0x158 = 0;
    self->field_0x128 = g_unk_7101a33fa0 + 2;
    self->field_0x160 = 0;
    self->field_0x1a0 = true;
    self->field_0x1a8 = nullptr;
    self->field_0x1b0 = nullptr;
    reinterpret_cast<void (*)(UnkWaterTriggerChangeComponent*)>(self->vtable[3])(self);
}
