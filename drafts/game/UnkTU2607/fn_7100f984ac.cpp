// From unit 2607 of tools/tu_map.py: starts the environment fade's channel 8 (three colours): each colour fades
// from the light object's current colour, or from the first argument when there is no light object.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkEnvironmentFade;

namespace {
struct UnkLightColors {
    u8 pad_0x0[0x3c];
    CVector4D first;  // 0x3c
    u8 pad_0x4c[0x10];
    CVector4D second;  // 0x5c
    CVector4D third;   // 0x6c
};
struct UnkLightTarget {
    u8 pad_0x0[0x78];
    UnkLightColors* colors;  // 0x78
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
    u8 pad_0x0[0x10c];
    CVector4D from8[3][2];  // 0x10c: each colour's start and end
    u8 pad_0x16c[0x1dc];
    f32 progress[29];  // 0x348
    f32 duration[29];  // 0x3bc
    bool done[29];     // 0x430
};
static_assert(__builtin_offsetof(UnkFadeLayout, progress) == 0x348, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, done) == 0x430, "UnkFadeLayout placeholder layout");
}  // namespace

void fn_7100f984ac(f32 time, UnkEnvironmentFade* self, const UnkVector4D* a, const UnkVector4D* b,
                   const UnkVector4D* c) {
    UnkFadeLayout* s = reinterpret_cast<UnkFadeLayout*>(self);
    s->duration[8] = time;
    s->done[8] = false;
    bool lo_ok = time - 0.001f <= 0.0f;
    bool hi_ok = time + 0.001f >= 0.0f;
    s->progress[8] = hi_ok && lo_ok ? 1.0f : 0.0f;
    UnkEnvManager* manager = static_cast<UnkEnvManager*>(g_unk_7101d47368);
    if (!manager) {
        manager = static_cast<UnkEnvManager*>(fn_71000839a0(0x4c8, 0x3f));
        fn_7100d8ced8(manager);
        g_unk_7101d47368 = manager;
    }
    UnkLightObject* light = manager->light;
    const CVector4D* third;
    if (!light) {
        fn_710007b518(&s->from8[0][0], a);
        fn_710007b518(&s->from8[1][0], a);
        third = a;
    } else {
        const CVector4D* first = &light->target->colors->first;
        fn_710007b518(&s->from8[0][0], first);
        const CVector4D* second = &light->target->colors->second;
        fn_710007b518(&s->from8[1][0], second);
        third = &light->target->colors->third;
    }
    fn_710007b518(&s->from8[2][0], third);
    fn_710007b518(&s->from8[0][1], a);
    fn_710007b518(&s->from8[1][1], b);
    fn_710007b518(&s->from8[2][1], c);
}
