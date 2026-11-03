// CCharClassChozoCommanderXLifeComponent: its constructor (fn_71008ae790). The base constructor
// fn_71008bca30, the vtable, the floats 10 and 0.7 at +0xa4, 3500 at +0x18, then the class-entry pass
// of base::core::CBaseObject (fn_7100023854). Names and layouts are placeholders.
#include "types.h"

struct UnkNode;
void fn_71008bca30(void* self);
void fn_7100023854(UnkNode* node);
extern void* g_unk_7101a7dbf0[];

struct UnkChozoCommanderXLife {
    void** vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x84];
    f32 field_0xa4;
    f32 field_0xa8;
};

void fn_71008ae790(UnkChozoCommanderXLife* self) {
    fn_71008bca30(self);
    self->vtable = g_unk_7101a7dbf0 + 2;
    self->field_0xa4 = 10.0f;
    self->field_0xa8 = 0.7f;
    self->field_0x18 = 3500;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
