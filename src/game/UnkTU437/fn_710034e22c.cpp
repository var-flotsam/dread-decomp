// Unit 437 (CAimComponent): turns the aim light on or off. On, it takes a light from the light manager
// when it has none, sets its colour, points it along the aim segment and places it at the segment's end;
// off, it switches the light off and hands it back. Names and layouts are placeholders.
#include <math.h>

#include "base/color/CColor4F.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector4D.h"
#include "game/UnkTU251.h"
#include "game/UnkTU290.h"
#include "game/UnkTU437.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkListedObj;
struct UnkToggle;
struct UnkMatrixNode;
UnkListedObj* fn_7100e0b9a0(char* self);
void fn_7100e0bf48(char* self, UnkListedObj* obj);
void fn_710020c4b8(UnkToggle* self, bool value);

struct UnkAimLightData_l34e22c {
    u8 pad_0x0[0x30];
    f32 direction[3];               // 0x30
    base::math::CVector4D color;    // 0x3c
    u8 pad_0x4c[0x30];
    u32 field_0x7c;                 // 0x7c
    u8 pad_0x80[0x68];
    f32 field_0xe8;                 // 0xe8
    u8 pad_0xec[0xcc];
    u8 flags[6];                    // 0x1b8: [0] dirty
};
static_assert(__builtin_offsetof(UnkAimLightData_l34e22c, field_0xe8) == 0xe8, "layout");
static_assert(__builtin_offsetof(UnkAimLightData_l34e22c, flags) == 0x1b8, "layout");

struct UnkAimLightHolder_l34e22c {
    u8 pad_0x0[0x78];
    UnkAimLightData_l34e22c* data;  // 0x78
    u8 pad_0x80[0x14];
    f32 field_0x94;                 // 0x94
};

struct UnkAimLight_l34e22c {
    u8 pad_0x0[0x20];
    u8 node[0x118];                     // 0x20
    UnkAimLightHolder_l34e22c* holder;  // 0x138
};

struct UnkAimLightSelf_l34e22c {
    u8 pad_0x0[0x4f8];
    UnkVec3f from;  // 0x4f8
    UnkVec3f to;    // 0x504
    u8 pad_0x510[0x18];
    bool enabled;   // 0x528
    u8 pad_0x529[0x427];
    UnkAimLight_l34e22c* light;      // 0x950
    base::color::CColor4F color;     // 0x958
};
static_assert(__builtin_offsetof(UnkAimLightSelf_l34e22c, light) == 0x950, "layout");

static inline char* LightManager() {
    if (!g_unk_71726b1688) {
        void* p = fn_71000839a0(0x198, 0x3f);
        fn_7100359118(p);
        g_unk_71726b1688 = p;
    }
    return static_cast<char*>(g_unk_71726b1688);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710034e22c(UnkAimLightSelf_l34e22c* self) {
    bool enabled = self->enabled;
    UnkAimLight_l34e22c* light = self->light;
    if (enabled) {
        if (!light) {
            self->light = reinterpret_cast<UnkAimLight_l34e22c*>(fn_7100e0b9a0(LightManager()));
            fn_710020c4b8(reinterpret_cast<UnkToggle*>(self->light->node), false);
            self->light->holder->data->flags[5] = 0;
            light = self->light;
        }
        {
            base::math::UnkVector4D color;
            base::math::UnkVector4D c = fn_7100030878(&self->color);
            color = base::math::UnkVector4D(c.x <= 0.0f ? 0.0f : c.x, c.y <= 0.0f ? 0.0f : c.y,
                                            c.z <= 0.0f ? 0.0f : c.z, c.w <= 0.0f ? 0.0f : c.w);
            fn_710007b518(&light->holder->data->color, &color);
        }
        fn_710020c8ac(self->light->node, true);

        f32 dx = (self->to.x - self->from.x) * 0.5f;
        f32 dy = (self->to.y - self->from.y) * 0.5f;
        f32 dz = (self->to.z - self->from.z) * 0.5f;
        f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
        UnkVec3f position = self->to;
        UnkAimLightHolder_l34e22c* holder = self->light->holder;
        UnkAimLightData_l34e22c* data = holder->data;
        data->direction[0] = dx * inv * 50.0f;
        data->direction[1] = dy * inv * 50.0f;
        data->direction[2] = dz * inv * 50.0f;
        data->flags[0] = 1;
        fn_71001bdc3c(holder);
        fn_710020ca64(reinterpret_cast<UnkMatrixNode*>(self->light->node), &position);
        holder = self->light->holder;
        holder->field_0x94 = 10.0f;
        holder->data->field_0xe8 = 10.0f;
        fn_710020c9ac(self->light->node, 22.0f);
        self->light->holder->data->field_0x7c = 0;
    } else if (light) {
        fn_710020c8ac(light->node, false);
        fn_7100e0bf48(LightManager(), reinterpret_cast<UnkListedObj*>(self->light));
        self->light = nullptr;
    }
}
