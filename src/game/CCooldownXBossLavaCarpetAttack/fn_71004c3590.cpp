// Node routine that calls fn_71004c1894, fn_7100d62b04, sets its vtable and initialises or clears fields.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCooldownXBossAttack.h"
#include "game/UnkPacked.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x60];
    u64 field_0x68;
    void** field_0x70;
    u64 field_0x78;
    u64 field_0x80;
    u64 field_0x88;
    u8 pad_0x90[0x10];
    u8 field_0xa0[0x120];  // a POLYCOLLECTION2D shape (fn_7100d62b04)
    u64 field_0x1c0;
    u8 pad_0x1c8[0x8];
    u64 field_0x1d0;
    u32 field_0x1d8;
    u8 pad_0x1dc[0x5a0];
    UnkPacked<u64> field_0x77c;
    u16 field_0x784;
    u8 field_0x786;
};


void fn_71004c3590(UnkNode* self) {
    fn_71004c1894(self);
    self->vtable = &g_unk_7101a3e5b8[2];
    self->field_0x70 = &g_unk_7101a22d48[2];
    self->field_0x68 = 0;
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    self->field_0x88 = 0;
    fn_7100d62b04(self->field_0xa0);
    self->field_0x1c0 = 0xffffffff00000000ull;
    self->field_0x1d0 = 0;
    self->field_0x1d8 = 0;
    self->field_0x77c.v = 0;
    self->field_0x784 = 1;
    self->field_0x786 = 0;
}
