// From unit 2607 of tools/tu_map.py: starts the environment fade's channel 8 (three colours): each colour fades
// from the light object's current colour, or from the first argument when there is no light object.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "base/reflection.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2607.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

namespace {
struct UnkLightColors_f984ac {
    u8 pad_0x0[0x3c];
    CVector4D first;  // 0x3c
    u8 pad_0x4c[0x10];
    CVector4D second;  // 0x5c
    CVector4D third;   // 0x6c
};
struct UnkLightTarget_f984ac {
    u8 pad_0x0[0x78];
    UnkLightColors_f984ac* colors;  // 0x78
};
struct UnkLightObject_f984ac {
    u8 pad_0x0[0x140];
    UnkLightTarget_f984ac* target;  // 0x140
};
struct UnkEnvManager_f984ac {
    u8 pad_0x0[0x498];
    UnkLightObject_f984ac* light;  // 0x498
};

struct UnkFadeLayout_f984ac {
    u8 pad_0x0[0x10c];
    CVector4D from8[3][2];  // 0x10c: each colour's start and end
    u8 pad_0x16c[0x1dc];
    f32 progress[29];  // 0x348
    f32 duration[29];  // 0x3bc
    bool done[29];     // 0x430
};
static_assert(__builtin_offsetof(UnkFadeLayout_f984ac, progress) == 0x348, "UnkFadeLayout_f984ac placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout_f984ac, done) == 0x430, "UnkFadeLayout_f984ac placeholder layout");
}  // namespace

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100f984ac(f32 time, UnkEnvironmentFade* self, const UnkVector4D* a, const UnkVector4D* b,
                   const UnkVector4D* c) {
    UnkFadeLayout_f984ac* s = reinterpret_cast<UnkFadeLayout_f984ac*>(self);
    s->duration[8] = time;
    s->done[8] = false;
    bool lo_ok = time - 0.001f <= 0.0f;
    bool hi_ok = time + 0.001f >= 0.0f;
    s->progress[8] = hi_ok && lo_ok ? 1.0f : 0.0f;
    UnkEnvManager_f984ac* manager = static_cast<UnkEnvManager_f984ac*>(g_unk_7101d47368);
    if (!manager) {
        manager = static_cast<UnkEnvManager_f984ac*>(fn_71000839a0(0x4c8, 0x3f));
        fn_7100d8ced8(manager);
        g_unk_7101d47368 = manager;
    }
    UnkLightObject_f984ac* light = manager->light;
    const CVector4D* third;
    if (!light) {
        Assign(s->from8[0][0], *a);
        Assign(s->from8[1][0], *a);
        third = a;
    } else {
        const CVector4D* first = &light->target->colors->first;
        Assign(s->from8[0][0], *first);
        const CVector4D* second = &light->target->colors->second;
        Assign(s->from8[1][0], *second);
        third = &light->target->colors->third;
    }
    Assign(s->from8[2][0], *third);
    Assign(s->from8[0][1], *a);
    Assign(s->from8[1][1], *b);
    Assign(s->from8[2][1], *c);
}
