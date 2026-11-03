// CBombMovement unit (tools/tu_map.py unit 1610): the constructor of CBombMovement_ctor.cpp
// (fn_7100aa9ee4) followed by the post-construction virtual call (slot 3). Names and the layout are
// placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkBombMovement_b46a3 {
    void** vtable;
    u8 unk_0x8[0x478];
    CFilePathStrId field_0x480;
    bool field_0x488;
    u32 field_0x48c[8];
    u64 field_0x4b0;

    void vf_0x18() { reinterpret_cast<void (*)(UnkBombMovement_b46a3*)>(vtable[3])(this); }
};

struct UnkComponent_0x5b8;  // src/game/CProjectileMovement/fn_7100ac2c5c.cpp
void fn_7100ac7224(UnkComponent_0x5b8* self);  // the base constructor
extern void* g_unk_7101a94c90[];

void fn_7100aac26c(UnkBombMovement_b46a3* self) {
    fn_7100ac7224(reinterpret_cast<UnkComponent_0x5b8*>(self));
    self->vtable = &g_unk_7101a94c90[2];
    new (&self->field_0x480) CFilePathStrId("", true);
    self->field_0x488 = false;
    self->field_0x4b0 = 0;
    __builtin_memset(self->field_0x48c, 0, sizeof(self->field_0x48c));
    self->vf_0x18();
}
