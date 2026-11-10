// Unit 2189 (tools/tu_map.py): two views of one object's vector (through its first word): its
// field at +0x10 and its four components rotated one place (the w first) are handed to the render
// manager as pass parameters (fn_7100219294), the second one only while the main camera ctrl is not
// the CAnimatedCameraCtrl one. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkPassSource_b0217 {
    f32 w;       // 0x0
    f32 x;       // 0x4
    f32 y;       // 0x8
    f32 z;       // 0xc
    f32 field_0x10;
};

struct UnkPassOwner_b0217 {
    UnkPassSource_b0217* source;
};

struct UnkPassVec4_b0217 {
    f32 x, y, z, w;
};

__attribute__((always_inline)) void fn_7100c7ce14(UnkPassOwner_b0217* self) {
    f32 value = self->source->field_0x10;
    fn_7100219294(g_unk_7101d47300, 0xcc53b066013b1983ull, &value, 0);
    UnkPassSource_b0217* s = self->source;
    UnkPassVec4_b0217 v;
    u64 xy;
    __builtin_memcpy(&xy, &s->x, 8);
    f32 z = s->z;
    f32 w = s->w;
    __builtin_memcpy(&v.x, &xy, 8);
    v.z = z;
    v.w = w;
    fn_7100219294(g_unk_7101d47300, 0xf91950417c43bfaaull, &v.x, 0);
}

void fn_7100c7cea4(UnkPassOwner_b0217* self) {
    if (fn_7100deabe4(g_unk_7101d47310)) {
        UnkCameraCtrl* ctrl = fn_71010e2080(fn_7100defcec(g_unk_7101d47310));
        if (!ctrl)
            return;
        const base::reflection::CType* type = ctrl->vf_0x0();
        if (!fn_710009d50c(type, fn_71010e7f90()))
            return;
    }
    fn_7100c7ce14(self);
}
