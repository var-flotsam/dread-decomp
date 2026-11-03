// The breakable tile group component's constructor (tools/tu_map.py: the CBreakableTileGroupComponent
// unit): base constructor, vtable, two arrays (allocation flags 0xf; the second, of words, reserved
// for 5), a cleared pair, a callback bound to the component (fn_71007610bc), a member built out of
// line at +0xc0 and a cleared word at +0xf8. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101a6e3d8[];

struct UnkBreakableTileGroup_b46a2;
void fn_71007610bc();
void fn_71009f1910(UnkBreakableTileGroup_b46a2* self);
void fn_710075d058(void* member);

struct UnkBreakableTileGroup_b46a2 {
    void** vtable;
    u8 pad_0x8[0x40];
    u32 field_0x48;
    u8 pad_0x4c[4];
    base::UnkVector<u64> list_0x50{0xf};
    base::UnkVector<u32> list_0x78{0xf};
    u64 field_0xa0;
    u64 field_0xa8;
    UnkBreakableTileGroup_b46a2* owner;
    void (*callback)();
    u8 field_0xc0[0x38];
    u32 field_0xf8;
};

void fn_71007584c0(UnkBreakableTileGroup_b46a2* self) {
    fn_71009f1910(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a6e3d8 + 0x10);
    self->field_0x48 = 0;
    new (&self->list_0x50) base::UnkVector<u64>(0xf);
    new (&self->list_0x78) base::UnkVector<u32>(0xf);
    self->list_0x78.Reserve(5);
    self->field_0xa0 = 0;
    self->field_0xa8 = 0;
    self->owner = self;
    self->callback = fn_71007610bc;
    fn_710075d058(self->field_0xc0);
    self->field_0xf8 = 0;
}
