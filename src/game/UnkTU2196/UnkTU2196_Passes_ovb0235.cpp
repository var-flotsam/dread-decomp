// A unit with no known name (tools/tu_map.py unit 2196, 0x7100c7f50c-0x7100c7f6cc): the pass-parameter
// handlers src/game_aca57c/fn_7100c79a80.cpp passes its sampled values to. fn_7100c7f50c hands the render
// manager three views of the values (the field at +0x10, the first four floats, and the two floats at
// +0x14 followed by a zero word) as pass parameters (fn_7100219294); fn_7100c7f5c8 does the same unless
// the main camera ctrl is the CAnimatedCameraCtrl one (as src/game/UnkTU2189/UnkTU2189_Camera_ovb0217.cpp
// does for its pair). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

// The sampled values the pass parameters start with (src/game_aca57c/fn_7100c79a80.cpp's UnkSampleParams).
struct UnkSampleParams;

struct UnkPassValues_b0235 {
    f32 v0, v1, v2, v3;
    f32 field_0x10;
    f32 v5, v6;
};

struct UnkPassHead_b0235 {
    UnkPassValues_b0235* values;
};

struct UnkPassPair_b0235 {
    u64 xy;
    u32 z;
    u32 w;
};

struct UnkPassTail_b0235 {
    f32 a;
    f32 b;
    u64 zero;
};

__attribute__((always_inline)) void fn_7100c7f50c(UnkSampleParams* params) {
    auto* self = reinterpret_cast<UnkPassHead_b0235*>(params);
    f32 value = self->values->field_0x10;
    fn_7100219294(g_unk_7101d47300, 0x38bb3d82ecfaffaaull, &value, 0);
    UnkPassValues_b0235* s = self->values;
    UnkPassPair_b0235 pair;
    u64 xy;
    __builtin_memcpy(&xy, &s->v0, 8);
    u32 z = *reinterpret_cast<u32*>(&s->v2);
    u32 w = *reinterpret_cast<u32*>(&s->v3);
    __builtin_memcpy(&pair.xy, &xy, 8);
    pair.z = z;
    pair.w = w;
    fn_7100219294(g_unk_7101d47300, 0x95a2e304fd05cda8ull, reinterpret_cast<f32*>(&pair), 0);
    s = self->values;
    UnkPassTail_b0235 tail = {s->v5, s->v6, 0};
    fn_7100219294(g_unk_7101d47300, 0xb37573f01782316aull, reinterpret_cast<f32*>(&tail), 0);
}

void fn_7100c7f5c8(UnkSampleParams* params) {
    if (fn_7100deabe4(g_unk_7101d47310)) {
        UnkCameraCtrl* ctrl = fn_71010e2080(fn_7100defcec(g_unk_7101d47310));
        if (!ctrl)
            return;
        const base::reflection::CType* type = ctrl->vf_0x0();
        if (!fn_710009d50c(type, fn_71010e7f90()))
            return;
    }
    fn_7100c7f50c(params);
}
