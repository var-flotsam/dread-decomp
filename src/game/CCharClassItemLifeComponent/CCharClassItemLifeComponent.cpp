// Constructor of a component in the CCharClassItemLifeComponent unit: the base constructor
// fn_71008bca30, the vtable, the member at +0xa8 (fn_71008c2da8), the word at +0x118 set to 1, then
// the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "game/CCharClassLifeComponent.h"
#include "game/CDamageSourceFactor.h"
#include "types.h"

extern void* g_unk_7101a7de58[];

struct UnkItemLifeComponent {
    void** vtable;
    u8 pad_0x8[0xa0];
    u8 field_0xa8[0x70];
    u32 field_0x118;
};

void fn_71008b4d80(UnkItemLifeComponent* self) {
    fn_71008bca30(self);
    self->vtable = g_unk_7101a7de58 + 2;
    fn_71008c2da8(self->field_0xa8);
    self->field_0x118 = 1;
    reinterpret_cast<void (*)(UnkItemLifeComponent*)>(self->vtable[3])(self);
}
