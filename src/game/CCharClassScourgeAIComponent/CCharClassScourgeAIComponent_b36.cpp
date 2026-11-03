// Functions in the CCharClassScourgeAIComponent unit (tools/tu_map.py). Names and layouts are
// placeholders.
#include "types.h"

// Declared as src/game/CCharClassYamplotXAIComponent/CCharClassYamplotXAIComponent.cpp does.
struct UnkObject_7100416db8;
void fn_7100416db8(UnkObject_7100416db8* self);
void fn_7100699e48(void* self);
void fn_71007089a8(void* self);
extern void* g_unk_7101a61dc8[];

struct UnkScourgeDef {
    void** vtable;
    u8 pad_0x8[0x508];
    u8 field_0x510[0xf8];
    u8 field_0x608[0x40];
    u8 field_0x648[0x14];
    u8 pad_0x65c[0x8];
    u8 field_0x664[0x14];
};

// The constructor: the base constructor fn_7100416db8, the vtable, the members at +0x510 and
// +0x608 (fn_7100699e48, fn_71007089a8), two cleared 0x14-byte runs, then the post-construction
// virtual call (slot 3).
void fn_710069e970(UnkScourgeDef* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a61dc8 + 2;
    fn_7100699e48(self->field_0x510);
    fn_71007089a8(self->field_0x608);
    __builtin_memset(self->field_0x648, 0, sizeof(self->field_0x648));
    __builtin_memset(self->field_0x664, 0, sizeof(self->field_0x664));
    reinterpret_cast<void (*)(UnkScourgeDef*)>(self->vtable[3])(self);
}
