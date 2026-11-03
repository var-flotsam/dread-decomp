// CProjectileMovement unit (tools/tu_map.py unit 1623): whether the movement should end: its
// timer (+0x520) runs down by `dt` and ends it when it runs out; otherwise, unless the owner's
// flag at +0x1b5 is set or the range (+0x480) is negative, it ends when the target (the weak
// handle at +0x230) is out of range, or there is none, and the owner query fn_7100d7bea0 fails.
// Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"

bool fn_7100d7bea0(UnkActor* owner, bool flag);

struct UnkProjectileMovement7_b46a3 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1f8];
    UnkWeakHandle* target;  // 0x230
    u8 pad_0x238[0x248];
    f32 range;  // 0x480
    u8 pad_0x484[0x9c];
    f32 timer;  // 0x520
};

u32 fn_7100ac43ec(UnkProjectileMovement7_b46a3* self, f32 dt) {
    if (self->timer > 0.0f) {
        self->timer = self->timer - dt;
        if (self->timer <= 0.0f)
            return true;
    }
    UnkActor* owner = self->owner;
    if (!reinterpret_cast<u8*>(owner)[0x1b5]) {
        f32 range = self->range;
        if (range >= 0.0f) {
            UnkWeakHandle* h = self->target;
            bool far = true;
            if (h && h->target) {
                UnkActor* t = reinterpret_cast<UnkActor*>(h->target);
                f32 ox = owner->field_0x44, oy = owner->field_0x54, oz = owner->field_0x64;
                f32 tx = t->field_0x44, ty = t->field_0x54, tz = t->field_0x64;
                f32 dx = ox - tx;
                f32 dy = oy - ty;
                f32 dz = oz - tz;
                far = dx * dx + dy * dy + dz * dz > range * range;
            }
            if (far && !fn_7100d7bea0(owner, false))
                return 1;
        }
    }
    return 0;
}
