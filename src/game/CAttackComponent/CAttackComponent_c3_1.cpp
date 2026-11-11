// CAttackComponent unit (tools/tu_map.py): a constructor (fn_7101135f04) and a destructor
// (fn_7101135ff0). The constructor runs the component base constructor fn_71010c4148, stores the
// same vtables, clears the members and sets up the three arrays (allocation flags 0xf). The
// destructor It stores the vtables of the object
// (0x7101cb2cb8, also at +0x40 with the second address point) and of the part at +0x98
// (0x7101cb2ec0), clears the half-word at +0x48, frees the three arrays at +0xa0, +0xc8 and +0xf0
// (last first) and runs the component base destructor fn_71010c4194. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CComponent.h"
#include "game/UnkComponent.h"
#include "types.h"

extern void* g_unk_7101cb2cb8[];
extern void* g_unk_7101cb2ec0[];

struct UnkStateObject;

// The part at +0x98: its own vtable and the three arrays.
struct UnkAttackPartBase_c3 {
    void** vtable;
    UnkAttackPartBase_c3() { vtable = g_unk_7101cb2ec0 + 2; }
};

struct UnkAttackPart_c3 : UnkAttackPartBase_c3 {
    base::UnkVector<u8> field_0x8;
    base::UnkVector<u8> field_0x30;
    base::UnkVector<u8> field_0x58;

    UnkAttackPart_c3() : field_0x8(0xf), field_0x30(0xf), field_0x58(0xf) {}
};

struct UnkAttackComponent_c3 : UnkBaseComponent {
    void** vtable_0x40;
    u16 field_0x48;
    u8 field_0x4a;
    u32 field_0x4c;
    s32 field_0x50;
    u8 field_0x54[0x2d];
    u8 pad_0x81[3];
    u32 field_0x84;
    u32 field_0x88;
    u32 pad_0x8c;
    u64 field_0x90;
    UnkAttackPart_c3 part;  // 0x98
    u32 field_0x118;
    u64 field_0x120;
    u64 field_0x128;
    u64 field_0x130;
};
static_assert(__builtin_offsetof(UnkAttackComponent_c3, field_0x118) == 0x118, "layout");

void fn_7101135f04(UnkAttackComponent_c3* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101cb2cb8 + 2;
    self->vtable_0x40 = g_unk_7101cb2cb8 + 62;
    self->field_0x48 = 0;
    self->field_0x4a = 0;
    self->field_0x4c = 0;
    self->field_0x50 = -1;
    self->field_0x84 = 0;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    __builtin_memset(self->field_0x54, 0, sizeof(self->field_0x54));
    new (&self->part) UnkAttackPart_c3();
    self->field_0x118 = 0;
    self->field_0x120 = 0;
    self->field_0x128 = 0;
    self->field_0x130 = 0;
}

void fn_7101135ff0(UnkAttackComponent_c3* self) {
    self->field_0x48 = 0;
    self->vtable = g_unk_7101cb2cb8 + 2;
    self->vtable_0x40 = g_unk_7101cb2cb8 + 62;
    self->part.vtable = g_unk_7101cb2ec0 + 2;
    self->part.field_0x58.Free();
    self->part.field_0x30.Free();
    self->part.field_0x8.Free();
    fn_71010c4194(self);
}
