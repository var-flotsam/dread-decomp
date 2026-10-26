// Node routine that calls fn_710041a198, fn_71004917d0, fn_71004923f4, fn_71004930c8, sets its vtable, initialises or clears fields and makes a virtual call.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkBossAIComponent.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode : UnkBossAIComponent {
    u8 pad_0x528[0x2d0];
    u8 field_0x7f8;
};

extern void* g_unk_7101a3cab0[];
void fn_71004923f4(void*);
void fn_71004930c8(void*);
void fn_71004917d0(void*);

void fn_71004979dc(UnkNode* self) {
    fn_710041a198(self);
    self->vtable = &g_unk_7101a3cab0[2];
    fn_71004923f4(reinterpret_cast<char*>(self) + 0x528);
    fn_71004930c8(reinterpret_cast<char*>(self) + 0x618);
    fn_71004917d0(reinterpret_cast<char*>(self) + 0x708);
    self->field_0x7f8 = 1;
    *reinterpret_cast<u16*>(&self->field_0x4fc) = 257;
    self->field_0x409 = 0;
    reinterpret_cast<void (*)(UnkNode*)>(self->vtable[3])(self);
}
