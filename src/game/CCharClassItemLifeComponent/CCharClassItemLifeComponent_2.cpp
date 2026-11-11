// The constructor variant of the CCharClassItemLifeComponent unit's component without the
// post-construction virtual call (fn_71008b4c88), otherwise as fn_71008b4d80 in
// CCharClassItemLifeComponent.cpp: the base constructor fn_71008bca30, the vtable, the member at
// +0xa8 (fn_71008c2da8) and the word at +0x118 set to 1. Names and layouts are placeholders.
#include "game/CCharClassLifeComponent.h"
#include "game/CDamageSourceFactor.h"
#include "types.h"

extern void* g_unk_7101a7de58[];

// The signature follows the declaration in src/game/CCharClassDoorLifeComponent/ (a derived component).
struct UnkDoorLifeComponent;

struct UnkItemLifeComponent2 {
    void** vtable;
    u8 pad_0x8[0xa0];
    u8 field_0xa8[0x70];
    u32 field_0x118;
};

void fn_71008b4c88(UnkDoorLifeComponent* component) {
    UnkItemLifeComponent2* self = reinterpret_cast<UnkItemLifeComponent2*>(component);
    fn_71008bca30(self);
    self->vtable = g_unk_7101a7de58 + 2;
    fn_71008c2da8(self->field_0xa8);
    self->field_0x118 = 1;
}
