// CSuperMissileMovement: its constructor. The missile-movement base constructor fn_7100aba0c0, 0x18 at
// +0x448, the vtable, the colour at +0x4d8 set to (0, 1, 0.1, 1) through the colour's assignment, a set
// flag at +0x4e8, then virtual slot 3. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "game/CMissileMovement.h"
#include "types.h"

struct UnkSuperMissileMovement;
struct UnkMissileMovement;
extern void* g_unk_7101a96378[];

struct UnkSuperMissileMovement {
    void** vtable;
    u8 pad_0x8[0x440];
    u32 field_0x448;
    u8 pad_0x44c[0x4d8 - 0x44c];
    base::color::CColor4F color;
    bool field_0x4e8;
};

void fn_7100abfeec(UnkSuperMissileMovement* self) {
    fn_7100aba0c0(reinterpret_cast<UnkMissileMovement*>(self));
    self->field_0x448 = 0x18;
    self->vtable = g_unk_7101a96378 + 2;
    base::color::CColor4F c(0.0f, 1.0f, 0.1f, 1.0f);
    fn_7100030728(&self->color, &c);
    self->field_0x4e8 = true;
    reinterpret_cast<void (*)(UnkSuperMissileMovement*)>(self->vtable[3])(self);
}
