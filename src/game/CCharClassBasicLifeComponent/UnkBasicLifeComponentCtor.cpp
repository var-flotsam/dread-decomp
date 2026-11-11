// Constructors of a component in the CCharClassBasicLifeComponent unit, without (fn_71008add20) and
// with (fn_71008ade18) the post-construction virtual call: the base constructor
// fn_71008bca30, the vtable, the member at +0xa8 (fn_71008c2da8), the flag at +0x118 set, then the
// post-construction virtual call (slot 3); a near copy of fn_71008b4d80
// (src/game/CCharClassItemLifeComponent/CCharClassItemLifeComponent.cpp). Names and layouts are
// placeholders.
#include "game/CCharClassLifeComponent.h"
#include "game/CDamageSourceFactor.h"
#include "types.h"

extern void* g_unk_7101a7db40[];

struct UnkBasicLifeComponent {
    void** vtable;
    u8 pad_0x8[0xa0];
    u8 field_0xa8[0x70];
    bool field_0x118;
};

static inline __attribute__((always_inline)) void InitBasicLife(UnkBasicLifeComponent* self) {
    fn_71008bca30(self);
    self->vtable = g_unk_7101a7db40 + 2;
    fn_71008c2da8(self->field_0xa8);
    self->field_0x118 = true;
}

void fn_71008add20(UnkBasicLifeComponent* self) {
    InitBasicLife(self);
}

void fn_71008ade18(UnkBasicLifeComponent* self) {
    InitBasicLife(self);
    reinterpret_cast<void (*)(UnkBasicLifeComponent*)>(self->vtable[3])(self);
}
