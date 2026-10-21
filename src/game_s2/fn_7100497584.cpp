// Node routine that calls fn_710041a198, fn_71004917d0, fn_71004923f4, fn_71004930c8, sets its vtable and initialises or clears fields.
#include <new>

#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x401];
    u8 field_0x409;
    u8 pad_0x40a[0xf2];
    u16 field_0x4fc;
    u8 pad_0x4fe[0x2fa];
    u8 field_0x7f8;
};

void fn_710041a198(UnkNode*);
extern void* g_unk_7101a3cab0[];
void fn_71004923f4(void*);
void fn_71004930c8(void*);
void fn_71004917d0(void*);

void fn_7100497584(UnkNode* self) {
    fn_710041a198(self);
    self->vtable = &g_unk_7101a3cab0[2];
    fn_71004923f4(reinterpret_cast<char*>(self) + 0x528);
    fn_71004930c8(reinterpret_cast<char*>(self) + 0x618);
    fn_71004917d0(reinterpret_cast<char*>(self) + 0x708);
    self->field_0x7f8 = 1;
    self->field_0x4fc = 257;
    self->field_0x409 = 0;
}
