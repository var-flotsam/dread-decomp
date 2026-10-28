// From unit 223 of tools/tu_map.py (0x710019bd2c-0x710019c69c): the four corners of the screen in
// clip space, kept in a global for full-screen passes. The graphics system sets them up when it
// adds its camera (src/engine/UnkGraphicsSystem.cpp). Names are placeholders.
// Holds fn_710019c198.
#include "types.h"

// The corners as (x, y) pairs: (-1, -1), (-1, 1), (1, 1), (1, -1) (reached through the GOT).
extern f32 unk_7101a1e1b8[8];

// A clip-space coordinate moved by half a pixel of the viewport's width or height (src/lifted).
f32 fn_71000768c0(f32 x);
f32 fn_71000768f0(f32 y);

// Sets the corners, moved by half a pixel when `half_pixel` is set.
void fn_710019c198(void* /*camera*/, bool half_pixel) {
    f32* c = unk_7101a1e1b8;
    f32 last;
    if (half_pixel) {
        c[0] = fn_71000768c0(-1.0f);
        c[1] = fn_71000768f0(-1.0f);
        c[2] = fn_71000768c0(-1.0f);
        c[3] = fn_71000768f0(1.0f);
        c[4] = fn_71000768c0(1.0f);
        c[5] = fn_71000768f0(1.0f);
        c[6] = fn_71000768c0(1.0f);
        last = fn_71000768f0(-1.0f);
    } else {
        last = -1.0f;
        c[0] = -1.0f;
        c[1] = -1.0f;
        c[2] = -1.0f;
        c[3] = 1.0f;
        c[4] = 1.0f;
        c[5] = 1.0f;
        c[6] = 1.0f;
    }
    c[7] = last;
}
