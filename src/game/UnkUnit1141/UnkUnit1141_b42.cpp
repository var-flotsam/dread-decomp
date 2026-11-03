// Unit 1141 (tools/tu_map.py, no name hint). fn_7100714700: hands a timeline's motion over `frames`
// to its target (src/game/UnkTimelineMotion.cpp calls it): while there are frames and a mode, the
// target keeps the frames (+0x6c), the per-frame translation (the matrix's last column divided by the
// frames with mode bit 1, else the zero vector; +0x60), the matrix (+0x0), the mode (+0x5c) and a
// flag 8 in its flags (+0x58), minus the bits of g_unk_717270ec80. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/UnkZeroVector3.h"
#include "types.h"

using base::math::CVector3D;

extern __attribute__((visibility("hidden"))) u32 g_unk_717270ec80;

// Declared as src/game/UnkTimelineMotion.cpp does; the mode is its first word.
struct UnkMotionMode;

struct UnkMotionTarget_b42 {
    UnkMatrix44 matrix;  // 0x0
    u8 pad_0x40[0x18];
    u32 flags;  // 0x58
    u32 mode;  // 0x5c
    CVector3D step;  // 0x60
    f32 frames;  // 0x6c
};

static inline CVector3D Step_b42(f32 frames, const UnkMatrix44* motion) {
    f32 inv = 1.0f / frames;
    return {inv * motion->m[3], inv * motion->m[7], inv * motion->m[11]};
}

void fn_7100714700(f32 frames, void* target, const UnkMatrix44* motion, const UnkMotionMode* mode) {
    UnkMotionTarget_b42* self = static_cast<UnkMotionTarget_b42*>(target);
    const u32* bits = reinterpret_cast<const u32*>(mode);
    if (!(frames > 0.0f) || *bits == 0)
        return;
    self->frames = frames;
    // The mode's bit 1 is read as a byte (it may alias the store above); the vector moves as words.
    u32 w[3];
    if (*reinterpret_cast<const u8*>(mode) & 2) {
        CVector3D v = Step_b42(frames, motion);
        __builtin_memcpy(w, &v, sizeof(w));
    } else {
        __builtin_memcpy(w, &g_unk_710164779c, sizeof(w));
    }
    u32* out = reinterpret_cast<u32*>(&self->step);
    out[0] = w[0];
    out[1] = w[1];
    out[2] = w[2];
    UnkCopyMatrix(&self->matrix, *motion);
    self->flags = (self->flags | 8) & ~g_unk_717270ec80;
    self->mode = *bits;
}
