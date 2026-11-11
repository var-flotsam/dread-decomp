// Unit 2763 (tools/tu_map.py): a GUI object's colour flash update: takes the target's colour on the
// first update, restores it once the delay runs out, and while the period is positive blends between
// that colour and the mask colour (a triangle wave over the period), keeping the channels whose mask
// is negative. Names and layouts are placeholders.
#include "game/UnkTU2746.h"
#include "game_s2/UnkWeakHandle.h"
#include "../../game_s7/UnkGuiS7.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32;

struct UnkFadeColor_b0242 {
    f32 r, g, b, a;
};
float fmodf(float, float);

struct UnkFlashTarget_ovo0318 {
    u8 pad_0x0[0xa0];
    f32 color[4];  // 0xa0
};

struct UnkFlashUpdate_ovo0318 {
    void** vtable;
    bool pending;  // 0x8
    u8 pad_0x9[3];
    f32 mask[4];   // 0xc
    f32 period;    // 0x1c
    f32 delay;     // 0x20
    f32 base[4];   // 0x24
    f32 phase;     // 0x34
    UnkWeakHandle* handle;  // 0x38
} __attribute__((packed));
static_assert(__builtin_offsetof(UnkFlashUpdate_ovo0318, handle) == 0x38, "layout");

void fn_71012029e4(UnkFlashUpdate_ovo0318* self, f32 dt) {
    if (self->pending) {
        self->phase = 0.0f;
        UnkFlashTarget_ovo0318* target =
            self->handle ? reinterpret_cast<UnkFlashTarget_ovo0318*>(self->handle->target) : nullptr;
        __builtin_memcpy(self->base, target->color, sizeof(self->base));
        self->pending = false;
    }
    if (self->delay > 0.0f) {
        self->delay -= dt;
        if (self->delay <= 0.0f) {
            self->period = -1.0f;
            unk_gui::fn_71011cfc24(self->handle ? reinterpret_cast<unk_gui::UnkGuiObj*>(self->handle->target) : nullptr,
                                   reinterpret_cast<const unk_gui::UnkColor&>(self->base), 1);
        }
    }
    if (self->period > 0.0f) {
        self->phase = fmodf(self->phase + dt, self->period);
        f32 r = self->phase / self->period;
        f32 t = r < 0.5f ? r + r : 1.0f - (r + -0.5f) * 2.0f;
        UnkFadeColor_b0242 color = fn_71011c1764(reinterpret_cast<const UnkFadeColor_b0242*>(self->base),
                                                   reinterpret_cast<const UnkFadeColor_b0242*>(self->mask), t);
        if (self->mask[0] < 0.0f)
            reinterpret_cast<a_u32*>(&color)[0] = reinterpret_cast<const a_u32*>(self->base)[0];
        if (self->mask[1] < 0.0f)
            reinterpret_cast<a_u32*>(&color)[1] = reinterpret_cast<const a_u32*>(self->base)[1];
        if (self->mask[2] < 0.0f)
            reinterpret_cast<a_u32*>(&color)[2] = reinterpret_cast<const a_u32*>(self->base)[2];
        unk_gui::fn_71011cfc24(self->handle ? reinterpret_cast<unk_gui::UnkGuiObj*>(self->handle->target) : nullptr,
                               reinterpret_cast<const unk_gui::UnkColor&>(color), 1);
    }
}
