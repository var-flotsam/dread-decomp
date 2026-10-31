// A constructor in the CCharClassGooplotAIComponent unit: the base constructor fn_7100416db8, the
// vtable, the members at +0x510 and +0x600 (fn_71005931d8, fn_7100593de8), the float at +0x3ac
// set to pi, then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "types.h"

// Declared as src/game/CCharClassYamplotXAIComponent/CCharClassYamplotXAIComponent.cpp does.
struct UnkObject_7100416db8;
void fn_7100416db8(UnkObject_7100416db8* self);
void fn_71005931d8(void* self);
void fn_7100593de8(void* self);
extern void* g_unk_7101a4cfa8[];

struct UnkGooplotDef {
    void** vtable;
    u8 pad_0x8[0x3a4];
    float field_0x3ac;
    u8 pad_0x3b0[0x160];
    u8 field_0x510[0xf0];
    u8 field_0x600[0xf0];
};

void fn_7100599434(UnkGooplotDef* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a4cfa8 + 2;
    fn_71005931d8(self->field_0x510);
    fn_7100593de8(self->field_0x600);
    self->field_0x3ac = 3.14159274f;
    reinterpret_cast<void (*)(UnkGooplotDef*)>(self->vtable[3])(self);
}
