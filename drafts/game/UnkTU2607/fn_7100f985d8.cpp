// From unit 2607 of tools/tu_map.py: starts the environment fade's channel 9 (a 2D value); it fades from the
// light object's current range, or from the target when there is no light object.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::math::CVector2D;

struct UnkEnvironmentFade;

namespace {
struct UnkLightRange {
    u8 pad_0x0[0x7c];
    f32 low;   // 0x7c
    f32 high;  // 0x80
};
struct UnkLightTarget {
    u8 pad_0x0[0x78];
    UnkLightRange* range;  // 0x78
};
struct UnkLightObject {
    u8 pad_0x0[0x140];
    UnkLightTarget* target;  // 0x140
};
struct UnkEnvManager {
    u8 pad_0x0[0x498];
    UnkLightObject* light;  // 0x498
};

struct UnkFadeLayout {
    u8 pad_0x0[0x16c];
    CVector2D from9;  // 0x16c
    CVector2D to9;    // 0x174
    u8 pad_0x17c[0x1cc];
    f32 progress[29];  // 0x348
    f32 duration[29];  // 0x3bc
    bool done[29];     // 0x430
};
static_assert(__builtin_offsetof(UnkFadeLayout, progress) == 0x348, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, done) == 0x430, "UnkFadeLayout placeholder layout");
}  // namespace

void fn_7100f985d8(f32 time, UnkEnvironmentFade* self, const CVector2D* to) {
    UnkFadeLayout* s = reinterpret_cast<UnkFadeLayout*>(self);
    s->duration[9] = time;
    s->done[9] = false;
    bool lo_ok = time - 0.001f <= 0.0f;
    bool hi_ok = time + 0.001f >= 0.0f;
    s->progress[9] = hi_ok && lo_ok ? 1.0f : 0.0f;
    UnkEnvManager* manager = static_cast<UnkEnvManager*>(g_unk_7101d47368);
    if (!manager) {
        manager = static_cast<UnkEnvManager*>(fn_71000839a0(0x4c8, 0x3f));
        fn_7100d8ced8(manager);
        g_unk_7101d47368 = manager;
    }
    f32 x;
    f32 y;
    if (manager->light) {
        UnkLightRange* range = manager->light->target->range;
        f32 a = range->low;
        f32 b = range->high;
        x = a < b ? a : b;
        y = a < b ? b : a + 0.001f;
    } else {
        x = to->x;
        y = to->y;
    }
    s->from9.x = x;
    s->from9.y = y;
    s->to9.x = to->x;
    s->to9.y = to->y;
}
