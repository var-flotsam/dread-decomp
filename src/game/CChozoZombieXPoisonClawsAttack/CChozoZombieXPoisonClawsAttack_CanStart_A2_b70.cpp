// CChozoZombieXPoisonClawsAttack's unit (the name from reflection): the attack can start when CAttack's
// test passes, the owner (+0x8) has a target and the target is within the range at +0x60 of the owner
// (squared distances). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkSelf;
u32 fn_7100360b84(UnkSelf*);

struct UnkPoisonClawsAttack_A2_b70 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x50];
    f32 range;  // 0x60
};

// `arg` and `mode` (x1, w2) reach the base check untouched in the original (tools/forwarded_args.py).
bool fn_710049e7d4(UnkPoisonClawsAttack_A2_b70* self, void* arg, u32 mode) {
    bool ok = reinterpret_cast<bool (*)(UnkPoisonClawsAttack_A2_b70*, void*, u32)>(fn_7100360b84)(self, arg, mode);
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!ok || !target)
        return false;
    UnkActor* owner = self->owner;
    f32 x = owner->field_0x44;
    f32 y = owner->field_0x54;
    f32 z = owner->field_0x64;
    target = fn_7100d7c990(owner);
    f32 dx = x - target->field_0x44;
    f32 dy = y - target->field_0x54;
    f32 tz = target->field_0x64;
    f32 d = dx * dx + dy * dy;
    f32 dz = z - tz;
    d += dz * dz;
    return !(d > self->range * self->range);
}
