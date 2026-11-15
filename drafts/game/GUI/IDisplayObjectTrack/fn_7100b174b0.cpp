// GUI::IDisplayObjectTrack: evaluates a keyframe track between key `index` and the next one: step, linear
// or Catmull-Rom (mode 3, optionally wrapping). Names and layouts are placeholders.
#include "types.h"

struct UnkKey1671 {
    f32 field_0x0;
    f32 value;  // 0x4
};

struct UnkKeys1671 {
    UnkKey1671* data;
    u32 count;  // 0x8
};

f32 fn_7100b174b0(u32 mode, UnkKeys1671* keys, u32 index, bool wrap, f32 t) {
    switch (mode) {
    case 1: {
        f32 inv = 1.0f - t;
        u32 next = index + 1;
        f32 a = keys->data[index].value;
        f32 b = keys->data[next].value;
        return inv * a + b * t;
    }
    case 3: {
        u32 next = index + 1;
        f32 p2 = keys->data[next].value;
        f32 p1 = keys->data[index].value;
        f32 d = p2 - p1;
        u32 prev = index - 1;
        f32 p0;
        if ((s32)prev >= 0) {
            p0 = keys->data[prev].value;
        } else if (wrap) {
            p0 = keys->data[keys->count - 1].value;
        } else {
            p0 = p1 - (d + d);
        }
        f32 p3;
        u32 after = index + 2;
        if (after < keys->count)
            p3 = keys->data[after].value;
        else if (wrap)
            p3 = keys->data[0].value;
        else
            p3 = p1 + (d + d);
        f32 t2 = t * t;
        f32 t3 = t2 * t;
        f32 a = (p1 + p1) + (p2 - p0) * t;
        f32 b = (p0 + p0) + p1 * -5.0f + p2 * 4.0f - p3;
        f32 c = p1 * 3.0f - p0 - p2 * 3.0f + p3;
        return (a + t2 * b + t3 * c) * 0.5f;
    }
    default:
        return keys->data[index].value;
    }
}
