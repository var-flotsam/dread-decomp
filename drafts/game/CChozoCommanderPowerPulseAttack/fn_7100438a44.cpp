// CChozoCommanderPowerPulseAttack: whether the attack can start. The commander list must be
// empty and the target must be in front of the owner, or the owner's movement must allow it.
// Names and layouts are placeholders.

#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"

struct UnkPowerPulse_l438a44 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

struct UnkCommanderList_l438a44 {
    void* data;
    u32 count;
};

bool fn_710042bb38(UnkPowerPulse_l438a44* self);
UnkCommanderList_l438a44* fn_7100459f94();
u32 fn_71010cab0c(void* movement);

struct Pos_l438a44 {
    f32 x, y, z;
};

static inline Pos_l438a44 PosOf(const UnkActor* a) { return Pos_l438a44{a->field_0x44, a->field_0x54, a->field_0x64}; }

bool fn_7100438a44(UnkPowerPulse_l438a44* self) {
    if (!fn_710042bb38(self) || fn_7100459f94()->count != 0)
        return false;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return false;
    UnkActor* owner = self->owner;
    Pos_l438a44 t = PosOf(target);
    Pos_l438a44 o = PosOf(owner);
    f32 dx = t.x - o.x;
    f32 dy = t.y - o.y;
    f32 dz = t.z - o.z;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    bool ahead = dx * m.m[2] + dy * m.m[6] + dz * m.m[10] >= 0.0f;
    u32 front = ahead;
    asm("" : "+r"(front));
    if (ahead)
        return true;
    void* movement = self->owner->field_0x218;
    if (!movement)
        return false;
    return (fn_71010cab0c(movement) | front) & 1;
}
