// CPolypFallPattern's unit (tools/tu_map.py unit 871): a constructor (fn_71005af734). The base
// constructor (fn_71005aceb8), the vtable 0x7101a50368, a -1 and three fives, then three empty
// arrays with flags 0xf. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CPolypFallPattern.h"
#include "types.h"

void fn_71005aceb8(void* self);

struct UnkPolypPattern_A3_b66 {
    void** vtable;
    u8 pad_0x8[0xe4];
    s32 field_0xec;
    f32 field_0xf0;
    u32 field_0xf4;
    u32 field_0xf8;
    base::UnkVector<void*> field_0x100;
    base::UnkVector<void*> field_0x128;
    base::UnkVector<void*> field_0x150;
};

void fn_71005af734(UnkPolypPattern_A3_b66* self) {
    fn_71005aceb8(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a50368) + 2;
    self->field_0xec = -1;
    self->field_0xf0 = 5.0f;
    self->field_0xf4 = 5;
    self->field_0xf8 = 5;
    new (&self->field_0x100) base::UnkVector<void*>(0xf);
    new (&self->field_0x128) base::UnkVector<void*>(0xf);
    new (&self->field_0x150) base::UnkVector<void*>(0xf);
}
