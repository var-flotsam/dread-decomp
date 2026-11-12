// CPoisonFlyDiveAttack: fn_7100632e34 runs the attack base step (fn_71003609b8), then, when the
// weak handle at +0x88 still has a target, hands the target the object at +0x70 (fn_71010cb2fc)
// and drops the handle; clears the words at +0x5c and +0x54 and lets the part at +0x80 of the
// manager g_unk_71726af468 (created on first use, flags 3) forget the actor (fn_710063a748) and
// update (fn_710063c670). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAttack.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU958.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkPoisonFlyDive : UnkAttackBase {
    u8 pad_0x20[0x34];
    u32 field_0x54;
    u8 pad_0x58[4];
    u32 field_0x5c;
    u8 pad_0x60[0x10];
    u8 field_0x70[0x18];
    UnkWeakHandle* field_0x88;
};

void fn_7100632e34(UnkPoisonFlyDive* self) {
    fn_71003609b8(self);
    if (UnkWeakHandle* handle = self->field_0x88) {
        if (UnkWeakTarget* target = handle->target)
            fn_71010cb2fc(target, self->field_0x70);
        ResetRef(&self->field_0x88);
    }
    self->field_0x5c = 0;
    self->field_0x54 = 0;
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 3));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    u8* part = manager->pad_0x0 + 0x80;
    fn_710063a748(part, self->field_0x8);
    fn_710063c670(part);
}
