// Constructor of a component in the CPowerGeneratorComponent unit, built on the activatable
// component of fn_7100878d24 (inlined; src/game/CCharClassActivatableComponent/): its own vtable,
// the member at +0x80 (fn_7100a7f434), two containers at +0xb8 and +0xf8 (base vtable, empty array
// with step 10 and flags 0xf, allocator, final vtable), a listener at +0x140, then the
// post-construction virtual call (slot 3). The containers' fields are written out flat (as
// sub-objects, Clang addresses a byte store from the container's base). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/CComponent.h"
#include "game/CDemolitionBlockComponent.h"
#include "types.h"

struct UnkStateObject;
void fn_7100a7f434(char* self);
extern void* g_unk_7101a7ac20[];
extern void* g_unk_7101a90f90[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern void* g_unk_7101a22d48[];

struct UnkPowerGeneratorComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    u32 field_0x7c;
    u8 field_0x80[0x38];
    void** field_0xb8;
    u64 field_0xc0;
    u64 field_0xc8;
    u32 field_0xd0;
    bool field_0xd4;
    u32 field_0xd8;
    base::UnkAllocator* field_0xe0;
    u32 field_0xe8;
    u64 field_0xf0;
    void** field_0xf8;
    u64 field_0x100;
    u64 field_0x108;
    u32 field_0x110;
    bool field_0x114;
    u32 field_0x118;
    base::UnkAllocator* field_0x120;
    u32 field_0x128;
    u64 field_0x130;
    u8 pad_0x138[0x8];
    void** field_0x140;
    void* field_0x148;
    void* field_0x150;
    void* field_0x158;
};

void fn_7100a7f688(UnkPowerGeneratorComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a7ac20 + 2;
    fn_71007b1528(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x78 = false;
    self->field_0x7c = 0;
    self->vtable = g_unk_7101a90f90 + 2;
    fn_7100a7f434(reinterpret_cast<char*>(self->field_0x80));
    self->field_0xb8 = g_unk_7101a20998 + 2;
    self->field_0xc0 = 0;
    self->field_0xc8 = 0;
    self->field_0xd0 = 10;
    self->field_0xd4 = true;
    self->field_0xd8 = 0xf;
    self->field_0xe0 = fn_7100083990();
    self->field_0xe8 = 0;
    self->field_0xb8 = g_unk_7101a33fa0 + 2;
    self->field_0xf0 = 0;
    self->field_0xf8 = g_unk_7101a20998 + 2;
    self->field_0x100 = 0;
    self->field_0x108 = 0;
    self->field_0x110 = 10;
    self->field_0x114 = true;
    self->field_0x118 = 0xf;
    self->field_0x120 = fn_7100083990();
    self->field_0x128 = 0;
    self->field_0xf8 = g_unk_7101a33fa0 + 2;
    self->field_0x130 = 0;
    self->field_0x140 = g_unk_7101a22d48 + 2;
    self->field_0x148 = nullptr;
    self->field_0x150 = nullptr;
    self->field_0x158 = nullptr;
    reinterpret_cast<void (*)(UnkPowerGeneratorComponent*)>(self->vtable[3])(self);
}
