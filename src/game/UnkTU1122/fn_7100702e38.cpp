// Unit 1122: a constructor on the unit 1124 object (fn_7100703814, which takes the id): two vtables and
// cleared state with a state word of 6. Names and layouts are placeholders.
#include "game/UnkTU1124.h"
#include "game/behaviortree/CGoToSideAction.h"
#include "types.h"

extern void* g_unk_7101a6bc28[];

struct UnkCleared_l702e38 {
    u8 bytes[13];
};

struct UnkObject_l702e38 {
    void** vtable;
    u8 pad_0x8[0xa8];
    void** field_0xb0;
    UnkCleared_l702e38 field_0xb8;
    u8 pad_0xc5[7];
    u8 field_0xcc;
    u64 field_0xd0;
    u64 field_0xd8;
    u8 field_0xe0;
    u8 pad_0xe1[3];
    u32 field_0xe4;
    u8 pad_0xe8;
    u8 field_0xe9;
};
static_assert(__builtin_offsetof(UnkObject_l702e38, field_0xe9) == 0xe9, "layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100702e38(UnkObject_l702e38* self, u32 id) {
    fn_7100703814(reinterpret_cast<UnkUnit1124Object*>(self), id);
    void** vtable = g_unk_7101a6bc28 + 2;
    void** sub_vtable = g_unk_7101a25fb0 + 2;
    self->field_0xcc = 0;
    self->field_0xe0 = 0;
    self->field_0xd0 = 0;
    self->field_0xd8 = 0;
    self->vtable = vtable;
    self->field_0xb0 = sub_vtable;
    self->field_0xb8 = UnkCleared_l702e38();
    self->field_0xe9 = 0;
    self->field_0xe4 = 6;
}
