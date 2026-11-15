// Unit 2689 (CPlayerMovement hint): while the game manager's input part at +0x38 reports an axis outside
// +-0.001, scales the axis by a weight that falls as fn_7100f0e230 approaches 0.1 and adds it, times dt, to
// the values at +0x6c and +0x60; a value pointing against the axis' sign is cleared first, one with it
// resets both. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkInput_l0daf1c;
struct UnkManagerHead_l0daf1c {
    u8 pad_0x0[0x38];
    u8 field_0x38[1];
};

bool fn_7100f0e228(void* input);
f32 fn_7100f0e4cc(void* input);
s32 fn_7100f0e4d4(void* input);
f32 fn_7100f0e230(void* input);

struct UnkMovement_l0daf1c {
    u8 pad_0x0[0x60];
    f32 field_0x60;
    u8 pad_0x64[8];
    f32 field_0x6c;
};

static inline void* Input() {
    return reinterpret_cast<UnkManagerHead_l0daf1c*>(g_unk_7101d47310)->field_0x38;
}

void fn_71010daf1c(void*, f32 dt, UnkMovement_l0daf1c* self) {
    if (!fn_7100f0e228(Input()))
        return;
    f32 axis = fn_7100f0e4cc(Input());
    if (axis + 0.001f >= 0.0f && axis - 0.001f <= 0.0f)
        return;
    s32 sign = fn_7100f0e4d4(Input());
    if (sign == 0) {
        if (self->field_0x6c != 0.0f)
            self->field_0x6c = 0.0f;
    } else if (sign > 0) {
        if (self->field_0x6c >= 0.0f) {
            self->field_0x6c = 0.0f;
            self->field_0x60 = 0.0f;
            return;
        }
        if (self->field_0x6c != 0.0f)
            self->field_0x6c = 0.0f;
    } else {
        if (!(self->field_0x6c > 0.0f)) {
            self->field_0x6c = 0.0f;
            self->field_0x60 = 0.0f;
            return;
        }
        if (self->field_0x6c != 0.0f)
            self->field_0x6c = 0.0f;
    }
    f32 value = fn_7100f0e4cc(Input());
    f32 closeness = fn_7100f0e230(Input());
    f32 t = !(closeness >= 0.1f) ? closeness : 0.1f;
    f32 weight = 1.0f - (0.1f - t) / 0.1f;
    weight = !(closeness <= 0.0f) ? weight : 0.0f;
    f32 step = value * weight * dt;
    if (self->field_0x6c != 0.0f)
        step = step * 0.5f;
    self->field_0x6c += step;
    self->field_0x60 += step;
}
