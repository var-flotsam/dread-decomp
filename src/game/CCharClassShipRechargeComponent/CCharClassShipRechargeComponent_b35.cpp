// A constructor in the CCharClassShipRechargeComponent unit: the base constructor fn_7100a7b494, the
// vtable, two strings set up at +0x1b8 and +0x1e8 (fn_710007e6c4), then the post-construction virtual
// call (slot 3). Names and layouts are placeholders.
#include "game/CCharClassUsableComponent.h"
#include "types.h"

struct UnkShipRechargeDef {
    void** vtable;
    u8 pad_0x8[0x1b0];
    u8 field_0x1b8[0x30];
    u8 field_0x1e8[0x30];
};

void fn_710007e6c4(void*);
extern void* g_unk_7101a926f0[];

void fn_7100a8b37c(UnkShipRechargeDef* self) {
    fn_7100a7b494(self);
    self->vtable = &g_unk_7101a926f0[2];
    fn_710007e6c4(self->field_0x1b8);
    fn_710007e6c4(self->field_0x1e8);
    reinterpret_cast<void (*)(UnkShipRechargeDef*)>(self->vtable[3])(self);
}
