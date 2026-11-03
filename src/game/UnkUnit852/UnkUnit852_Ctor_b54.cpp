// The constructor (fn_710059c5d8) of the class of translation unit 852 (no reflected class; built at +0x138
// of the AI manager, src/game/CAIManager/CTunableAIManager/fn_7100da7e2c.cpp): its vtable, two containers
// at +0x8 and +0x78 set up by fn_710059c644 (10 entries, flags 0x13), and a cleared field at +0xe8. Names
// and layouts are placeholders.
#include "types.h"

void fn_710059c644(u8* container, u32 a, u32 b, u32 c, u64 d);
extern void* g_unk_7101a4d8d8[];

struct UnkUnit852 {
    void** vtable;
    u8 field_0x8[0x70];
    u8 field_0x78[0x70];
    u64 field_0xe8;
};

void fn_710059c5d8(UnkUnit852* self) {
    self->vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101a4d8d8) + 0x10);
    fn_710059c644(self->field_0x8, 10, 0, 0x13, 0);
    fn_710059c644(self->field_0x78, 10, 0, 0x13, 0);
    self->field_0xe8 = 0;
}
