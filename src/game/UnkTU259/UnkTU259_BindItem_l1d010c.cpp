// Unit 259 (the graphics system's unit): binds one draw item's program and sets its shader constants
// (the named constants, the tint, the depth parameters), then draws it. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU177.h"
#include "game/UnkTU180.h"
#include "game/UnkTU230.h"
#include "game/UnkTU259.h"
#include "game/msapi/material/SAlpha.h"
#include "types.h"

using base::color::CColor4F;
using base::global::UnkStringData;
using base::math::CVector3D;

struct UnkGraphicsSystem;
struct UnkRenderItem_b0247;
struct UnkGfx258_b0247;
struct UnkGfxStrId;

extern void* g_unk_716e595c98 __attribute__((visibility("hidden")));
extern CVector3D g_unk_7101647b3c __attribute__((visibility("hidden")));

// Binds the program; true when it was bound anew (other files declare it void).

struct UnkDrawItem_l1d010c;
void fn_71001d2fc0(UnkGraphicsSystem* self, UnkDrawItem_l1d010c* item);

struct UnkDrawItem_l1d010c {
    u8 pad_0x0[0x20];
    void* kind;  // 0x20
    u8 pad_0x28[0x8];
    u8 field_0x30;
    u8 pad_0x31[0x80 - 0x31];
    u8 field_0x80;
    u8 pad_0x81[0xac - 0x81];
    u32 field_0xac;
    u8 field_0xb0;
    u8 pad_0xb1[0x390 - 0xb1];
    void* program;  // 0x390
    u8 pad_0x398[0x8];
    f32 depth;  // 0x3a0
};

struct UnkNamedConstant_l1d010c {
    UnkStringData* name;
    f32 values[4];
};

struct UnkShadeColor_l1d010c {
    CVector3D rgb;
    f32 a;
};

struct UnkGfx_l1d010c {
    u8 pad_0x0[0x8];
    UnkNamedConstant_l1d010c* constants;  // 0x8
    u8 pad_0x10[0x8];
    u32 constant_count;  // 0x18
    u8 pad_0x1c[0x317a998 - 0x1c];
    u32 pass;  // 0x317a998
    u32 mode;  // 0x317a99c
    u8 pad_0x317a9a0[0x317aae0 - 0x317a9a0];
    CColor4F tint;  // 0x317aae0
    u8 pad_0x317aaf0[0x317acc0 - 0x317aaf0];
    UnkDrawItem_l1d010c* current_item;  // 0x317acc0
    u8 pad_0x317acc8[0x317aea0 - 0x317acc8];
    u64 tint_location;    // 0x317aea0
    u64 depth_location;   // 0x317aea8
    u64 depth_location2;  // 0x317aeb0
    u8 pad_0x317aeb8[0x317af68 - 0x317aeb8];
    void* state;    // 0x317af68
    void* program;  // 0x317af70
    u8 pad_0x317af78[0x3183672 - 0x317af78];
    u8 field_0x3183672;
    u8 pad_0x3183673[0x31836ac - 0x3183673];
    u32 field_0x31836ac;
    f32 last_depth;      // 0x31836b0
    f32 depth_params[4];  // 0x31836b4
};

static_assert(__builtin_offsetof(UnkGfx_l1d010c, depth_params) == 0x31836b4, "");

// The callers pass a flag this function never reads.
void fn_71001d010c(UnkGfx_l1d010c* self, UnkDrawItem_l1d010c* item, bool flag) {
    void* program = item->program;
    if (!program)
        return;
    if (item->kind == g_unk_716e595c98)
        self->field_0x31836ac = 0;
    bool bound = fn_71001cc608(reinterpret_cast<UnkGraphicsSystem*>(self), program);
    auto* state = static_cast<UnkUniformTables*>(fn_71001aa028(program));
    SAlpha_ApplyMaterialStateByFlags(reinterpret_cast<UnkRenderItem_b0247*>(item), 0x7ff);
    for (UnkNamedConstant_l1d010c* constant = self->constants;
         constant != &self->constants[self->constant_count]; constant++) {
        if (constant->name == &g_unk_7101d47108->empty)
            continue;
        u64 location;
        fn_71001aa0cc(self->program, 1, reinterpret_cast<const UnkGfxStrId*>(&constant->name), &location);
        if (location)
            fn_710015e1f0(self->state, 1, location, 4, constant->values);
    }
    if (self->field_0x3183672 && !item->field_0x80 && !item->field_0x30) {
        u32 pass = self->pass;
        if ((pass == 0x10 || pass == 1) && self->mode != 2 && item->field_0xb0 && item->field_0xac == 4) {
            fn_710015a8c0(2);
            fn_710015aa20(0);
        }
    }
    if (bound) {
        u64 location = 0;
        if (self->tint_location) {
            UnkShadeColor_l1d010c color;
            color.rgb = g_unk_7101647b3c;
            color.a = self->tint.a;
            fn_710015e1f0(state, 0, self->tint_location, 4, reinterpret_cast<const f32*>(&color));
            if (fn_71001aa0b4(program, 1, 0xd2, &location)) {
                CColor4F tint(self->tint);
                fn_710015e1f0(state, 1, location, 4, &tint.r);
            }
        }
    }
    self->current_item = item;
    f32 depth = item->depth;
    u64 depth_location = self->depth_location;
    if (!depth_location)
        depth_location = self->depth_location2;
    if (depth_location) {
        f32 last = self->last_depth;
        if (!(depth + 0.001f >= last) || !(depth - 0.001f <= last)) {
            self->last_depth = depth;
            self->depth_params[3] = depth * 0.5f;
            fn_710015e1f0(self->state, 0, depth_location, 4, self->depth_params);
        }
    }
    u64 location;
    if (fn_71001aa0b4(program, 0, 0xe7, &location)) {
        CColor4F black(0.0f, 0.0f, 0.0f, 1.0f);
        fn_710015e2d0(state, 0, location, 4, 1, &black);
    }
    fn_71001d2fc0(reinterpret_cast<UnkGraphicsSystem*>(self), item);
}
