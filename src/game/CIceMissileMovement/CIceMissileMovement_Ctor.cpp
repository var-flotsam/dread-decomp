// CIceMissileMovement: its constructor. The base constructor fn_7100aba0c0, the vtable, a set flag at
// +0x620 and 0x18 at +0x448, then the class-entry pass of base::core::CBaseObject (fn_7100023854).
// Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "game/CMissileMovement.h"
#include "types.h"

struct UnkNode;
struct UnkIceMissileMovement;
struct UnkMissileMovement;
extern void* g_unk_7101a95730[];

struct UnkIceMissileMovement {
    void** vtable;
    u8 pad_0x8[0x440];
    u32 field_0x448;
    u8 pad_0x44c[0x620 - 0x44c];
    bool field_0x620;
};

void fn_7100ab4a60(UnkIceMissileMovement* self) {
    fn_7100aba0c0(reinterpret_cast<UnkMissileMovement*>(self));
    self->vtable = g_unk_7101a95730 + 2;
    self->field_0x620 = true;
    self->field_0x448 = 0x18;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
