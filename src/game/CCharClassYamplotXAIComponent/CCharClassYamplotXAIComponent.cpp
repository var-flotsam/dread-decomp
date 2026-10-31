// A constructor in the CCharClassYamplotXAIComponent unit: the base constructor fn_7100416db8, the
// vtable (0x7101a6ad30), the members at +0x510 and +0x600 (fn_71006ec1f8, fn_71006ec5dc), two
// cleared fields, then the post-construction virtual call (slot 3). Names and layouts are
// placeholders.
#include "types.h"

struct UnkObject_7100416db8;
void fn_7100416db8(UnkObject_7100416db8* self);
void fn_71006ec1f8(void* self);
void fn_71006ec5dc(void* self);
extern void* g_unk_7101a6ad30[];

struct UnkYamplotXDef {
    void** vtable;
    u8 pad_0x8[0x508];
    u8 field_0x510[0xf0];
    u8 field_0x600[0xf0];
    u64 field_0x6f0;
    u32 field_0x6f8;
};

void fn_71006eede8(UnkYamplotXDef* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a6ad30 + 2;
    fn_71006ec1f8(self->field_0x510);
    fn_71006ec5dc(self->field_0x600);
    self->field_0x6f0 = 0;
    self->field_0x6f8 = 0;
    reinterpret_cast<void (*)(UnkYamplotXDef*)>(self->vtable[3])(self);
}
