// Unit 2574 (tools/tu_map.py): two easing curves of a timed effect (the duration at +0x8): the
// vector they return starts from the default g_unk_71016477a8 and gets an x from the elapsed time and
// a y and z that blend from 1 towards the first curve's end. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkOneVector3.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkTimedCurve_ovb0219 {
    u8 pad_0x0[8];
    f32 duration;  // 0x8
};

UnkVec3f fn_7100ecf7bc(UnkTimedCurve_ovb0219* self, f32 t) {
    UnkVec3f r = reinterpret_cast<const UnkVec3f&>(g_unk_71016477a8);
    f32 progress = t / self->duration;
    progress = progress < 1.0f ? progress : 1.0f;
    f32 left = 1.0f - progress;
    f32 v = 1.0f - left * left;
    r.x = t * 2700.0f * 0.01f;
    if (v >= 1.0f) {
        r.y = r.z = 0.1f;
    } else {
        f32 y = 1.0f;
        if (!(v < 0.0f)) {
            f32 w = 1.0f - v;
            y = v * 0.1f;
            y += w;
        }
        r.y = r.z = y;
    }
    return r;
}

UnkVec3f fn_7100ecf6cc(UnkTimedCurve_ovb0219* self, f32 t) {
    UnkVec3f r = reinterpret_cast<const UnkVec3f&>(g_unk_71016477a8);
    f32 progress = t / self->duration;
    progress = progress < 1.0f ? progress : 1.0f;
    f32 left = 1.0f - progress;
    f32 v = (f32)(1.0 - pow((f64)left, 3.0));
    f32 yz = 1.0f;
    if (v >= 1.0f) {
        r.x = 1.0f;
    } else if (v < 0.0f) {
        r.x = 0.0f;
        yz = 1.5f;
    } else {
        f32 w = 1.0f - v;
        r.x = w * 0.0f + v;
        yz = w * 1.5f + v;
    }
    r.y = r.z = yz;
    return r;
}
