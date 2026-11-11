// Unit 1443 of tools/tu_map.py (0x710097061c-0x71009714bc, no name hint): the constructor over the
// base of unit 1440 (fn_7100969000): the vtable, the member at +0x90 (its vtable, cleared flags and
// runs, mode 3) and the flag at +0xcd. The same shape as unit 1441's (src/game/UnkUnit1441).
// Names and layouts are placeholders.
#include "game/CAutsharpAIComponent.h"
#include "game/UnkTU1440.h"
#include "types.h"

extern void* g_unk_7101a852e8[];  // the class's vtable

struct UnkUnit1443 {
    void** vtable;
    u8 pad_0x8[0x88];
    void** field_0x90;
    u8 pad_0x98[4];
    bool field_0x9c;
    u8 pad_0x9d[7];
    bool field_0xa4;
    u8 pad_0xa5[3];
    u8 field_0xa8[0xd];
    u8 pad_0xb5[3];
    u8 field_0xb8[0xd];
    u8 pad_0xc5[3];
    u32 field_0xc8;
    u8 pad_0xcc;
    bool field_0xcd;
};

void fn_710097061c(UnkUnit1443* self, u32 arg) {
    fn_7100969000(reinterpret_cast<char*>(self), arg);
    self->vtable = g_unk_7101a852e8 + 2;
    self->field_0x9c = false;
    self->field_0xa4 = false;
    __builtin_memset(self->field_0xa8, 0, sizeof(self->field_0xa8));
    __builtin_memset(self->field_0xb8, 0, sizeof(self->field_0xb8));
    self->field_0x90 = g_unk_7101a2e208 + 2;
    self->field_0xcd = false;
    self->field_0xc8 = 3;
}
