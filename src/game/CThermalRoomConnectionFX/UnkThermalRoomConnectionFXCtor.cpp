// Constructor of a component in the CThermalRoomConnectionFX unit, built on the activatable
// component of fn_7100878d24 (inlined; src/game/CCharClassActivatableComponent/): its own vtable,
// two containers at +0x80 and +0xc0 (base vtable, empty array with step 10 and flags 0xf,
// allocator, final vtable), cleared fields from +0xf8, 1.0 at +0x7c; then, while the flag at +0x78
// is clear, the virtual call at slot 17, and the post-construction virtual call (slot 3). The
// containers' fields are written out flat. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CComponent.h"
#include "game/CDemolitionBlockComponent.h"
#include "types.h"

struct UnkStateObject;
extern void* g_unk_7101a7ac20[];
extern void* g_unk_7101a7b058[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];

struct UnkThermalRoomConnectionFX {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    f32 field_0x7c;
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
    u32 field_0xd8;
    bool field_0xdc;
    u32 field_0xe0;
    base::UnkAllocator* field_0xe8;
    u32 field_0xf0;
    u64 field_0xf8;
    void* field_0x100;
    void* field_0x108;
    void* field_0x110;
    void* field_0x118;
    u32 field_0x120;
};

void fn_710087b9dc(UnkThermalRoomConnectionFX* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a7ac20 + 2;
    fn_71007b1528(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x78 = false;
    self->field_0x7c = 0;
    self->vtable = g_unk_7101a7b058 + 2;
    self->field_0x80 = g_unk_7101a20998 + 2;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    self->field_0x98 = 10;
    self->field_0x9c = true;
    self->field_0xa0 = 0xf;
    self->field_0xa8 = fn_7100083990();
    self->field_0xb0 = 0;
    self->field_0x80 = g_unk_7101a20950 + 2;
    self->field_0xb8 = 0;
    self->field_0xc0 = g_unk_7101a20998 + 2;
    self->field_0xc8 = 0;
    self->field_0xd0 = 0;
    self->field_0xd8 = 10;
    self->field_0xdc = true;
    self->field_0xe0 = 0xf;
    self->field_0xe8 = fn_7100083990();
    self->field_0xf0 = 0;
    self->field_0xc0 = g_unk_7101a20950 + 2;
    self->field_0xf8 = 0;
    self->field_0x100 = nullptr;
    self->field_0x108 = nullptr;
    self->field_0x110 = nullptr;
    self->field_0x118 = nullptr;
    self->field_0x120 = 0;
    self->field_0x7c = 1.0f;
    if (!self->field_0x78)
        reinterpret_cast<void (*)(UnkThermalRoomConnectionFX*)>(self->vtable[17])(self);
    reinterpret_cast<void (*)(UnkThermalRoomConnectionFX*)>(self->vtable[3])(self);
}
