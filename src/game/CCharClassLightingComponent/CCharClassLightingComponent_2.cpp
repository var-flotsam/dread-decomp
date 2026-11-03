// Copies of fn_71008e7f18 (CCharClassLightingComponent.cpp) in the same unit: for each light in the
// array at +0x60 whose description (+0x8, then its +0x8) carries `id` at +0x30, apply `arg` and `value`
// through a per-copy setter. Names and layouts are placeholders.
#include "types.h"

struct UnkLightingDesc2 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkLightingDescHolder2 {
    u8 pad_0x0[0x8];
    UnkLightingDesc2* desc;
};

struct UnkLightingLight2 {
    u8 pad_0x0[0x8];
    UnkLightingDescHolder2* holder;
};

struct UnkLightingComponent2 {
    u8 pad_0x0[0x60];
    UnkLightingLight2** lights;
    u32 count;
};

void fn_71008e5f14(UnkLightingLight2* light, void* arg, f32 value);
void fn_71008e5f8c(UnkLightingLight2* light, void* arg, f32 value);
void fn_71008e6004(UnkLightingLight2* light, void* arg, f32 value);

#define LIGHTING_APPLY_BY_ID(name, setter)                                                      \
    void name(UnkLightingComponent2* self, const u64* id, void* arg, f32 value) {                \
        for (UnkLightingLight2 **p = self->lights, **end = p + self->count; p != end; ++p) {     \
            UnkLightingLight2* light = *p;                                                       \
            UnkLightingDesc2* desc = light->holder->desc;                                        \
            u64 key = *id;                                                                       \
            if (desc->id == key)                                                                 \
                setter(light, arg, value);                                                       \
        }                                                                                        \
    }

LIGHTING_APPLY_BY_ID(fn_71008e7d98, fn_71008e5f14)
LIGHTING_APPLY_BY_ID(fn_71008e7e18, fn_71008e5f8c)
LIGHTING_APPLY_BY_ID(fn_71008e7e98, fn_71008e6004)
