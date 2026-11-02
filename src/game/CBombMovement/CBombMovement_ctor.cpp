// CBombMovement's constructor: the projectile movement's base constructor (fn_7100ac7224, as in
// src/game/CProjectileMovement/fn_7100ac2c5c.cpp), the vtable, a file path id built from "" and
// cleared fields. Names and the layout are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkBombMovement {
    void** vtable;
    u8 unk_0x8[0x478];
    CFilePathStrId field_0x480;
    bool field_0x488;
    u32 field_0x48c[8];
    u64 field_0x4b0;
};

struct UnkComponent_0x5b8;  // src/game/CProjectileMovement/fn_7100ac2c5c.cpp
void fn_7100ac7224(UnkComponent_0x5b8* self);  // the base constructor
extern void* g_unk_7101a94c90[];

void fn_7100aa9ee4(UnkBombMovement* self) {
    fn_7100ac7224(reinterpret_cast<UnkComponent_0x5b8*>(self));
    self->vtable = &g_unk_7101a94c90[2];
    new (&self->field_0x480) CFilePathStrId("", true);
    self->field_0x488 = false;
    self->field_0x4b0 = 0;
    __builtin_memset(self->field_0x48c, 0, sizeof(self->field_0x48c));
}
