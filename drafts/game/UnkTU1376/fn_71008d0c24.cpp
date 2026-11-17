// CDoorLifeComponent's unit (tools/tu_map.py unit 1376): blends the door's slot colour towards the base colour
// at +0x6e8 scaled by the target level, over a time from the level difference and speed (at once when zero),
// and remembers the target and the timer. Names and layouts are placeholders.
#include <math.h>

#include "base/color/CColor4F.h"
#include "game/UnkSlots.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU253.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;

struct UnkV4_b47 {
    f32 x, y, z, w;
    UnkV4_b47() {}
    UnkV4_b47(f32 x_, f32 y_, f32 z_, f32 w_) : x(x_), y(y_), z(z_), w(w_) {}
    UnkV4_b47(const UnkV4_b47& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

struct UnkDoorGfx_l8d0c24 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};
struct UnkDoorHolder_l8d0c24 {
    u8 pad_0x0[0x50];
    UnkDoorGfx_l8d0c24* gfx;
};
struct UnkDoorOwner_l8d0c24 {
    u8 pad_0x0[0x228];
    UnkDoorHolder_l8d0c24* holder;
};
struct UnkDoorState_l8d0c24 {
    u8 pad_0x0[0x10];
    s32 id;
};

struct UnkDoorLifeA5_b61_l8d0c24 {
    u8 pad_0x0[0x30];
    UnkDoorOwner_l8d0c24* owner;
    u8 pad_0x38[0x430 - 0x38];
    UnkDoorState_l8d0c24* current;
    u8 pad_0x438[0x63c - 0x438];
    f32 timer;
    f32 level;
    u8 pad_0x644[0x6e8 - 0x644];
    CColor4F colour;
    u8 pad_0x6f8[0xa10 - 0x6f8];
    u32 slot;
};
static_assert(__builtin_offsetof(UnkDoorLifeA5_b61_l8d0c24, colour) == 0x6e8, "layout");
static_assert(__builtin_offsetof(UnkDoorLifeA5_b61_l8d0c24, slot) == 0xa10, "layout");

// Blends from `from` (t = 0) to 1 (t = 1), t clamped to 0..1.
static inline f32 Brighten(f32 from, f32 t) {
    if (t >= 1.0f)
        return 1.0f;
    if (t < 0.0f)
        return from;
    return t + (1.0f - t) * from;
}

void fn_71008d0c24(UnkDoorLifeA5_b61* door, f32 target, f32 speed) {
    auto* self = reinterpret_cast<UnkDoorLifeA5_b61_l8d0c24*>(door);
    UnkDoorState_l8d0c24* current = self->current;
    if (current && speed == 0.0f && target != 0.0f && current->id == 3)
        return;
    UnkDoorGfx_l8d0c24* gfx = self->owner->holder->gfx;
    CColor4F colour(1.0f, 1.0f, 1.0f, 1.0f);
    if (gfx->slots->info->count <= self->slot)
        return;
    fn_71001c287c(gfx->slots, self->slot, 0, reinterpret_cast<CVector4D*>(&colour), 1);
    f32 span = fabsf(target - colour.a) * speed;
    f32 time = span * span;
    colour = self->colour;

    f32 capped = !(target >= 1.0f) ? target : 1.0f;
    f32 above = (capped - 0.5f) * 2.0f;
    f32 bright = !(target <= 0.5f) ? above : 0.0f;
    UnkV4_b47 scaled = fn_7100030830(reinterpret_cast<const UnkV4_b47*>(&colour), Brighten(0.75f, bright));
    colour = reinterpret_cast<const CColor4F&>(scaled);
    colour.a = target <= 0.0f ? 0.0f : target;

    if (time > 0.0f) {
        fn_71001c2ec0(gfx->slots, self->slot, 0, reinterpret_cast<CVector4D*>(&colour), 1, time);
        self->level = target;
        if (target > 0.0f)
            self->timer = time;
    } else {
        fn_71001c2764(gfx->slots, self->slot, 0, reinterpret_cast<const CVector4D*>(&colour), 1, true);
        if (speed > 0.0f) {
            self->level = target;
            if (target > 0.0f)
                self->timer = 0.001f;
        }
    }
}
