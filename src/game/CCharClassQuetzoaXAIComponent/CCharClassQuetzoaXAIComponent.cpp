// A constructor in the CCharClassQuetzoaXAIComponent unit: the CCharClassQuetzoaAIComponent
// constructor fn_71006490e8, the vtable, the member at +0x710 (fn_710063d208), the float at +0x4e0
// (1.2), two cleared flags, then the post-construction virtual call (slot 3). Names and layouts
// are placeholders.
#include "game/UnkBossAIComponent.h"
#include "types.h"

void fn_71006490e8(void* self);
void fn_710063d208(void* self);
extern void* g_unk_7101a58e88[];

struct UnkQuetzoaXDef : UnkBossAIComponent {
    u8 pad_0x528[0x1e8];
    u8 field_0x710[0x10];
};

void fn_710063eedc(UnkQuetzoaXDef* self) {
    fn_71006490e8(self);
    self->vtable = g_unk_7101a58e88 + 2;
    fn_710063d208(self->field_0x710);
    *reinterpret_cast<float*>(reinterpret_cast<u8*>(self) + 0x4e0) = 1.2f;
    self->field_0x409 = 0;
    reinterpret_cast<u8*>(self)[0x50c] = 0;
    reinterpret_cast<void (*)(UnkQuetzoaXDef*)>(self->vtable[3])(self);
}
