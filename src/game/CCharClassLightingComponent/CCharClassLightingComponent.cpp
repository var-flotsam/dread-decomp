// In the CCharClassLightingComponent unit: for each light in the array at +0x60 whose description
// (+0x8, then its +0x8) carries `id` at +0x30, applies `arg` and `value` through fn_71008e60fc.
// Names and layouts are placeholders.
#include "types.h"

struct UnkLightingDesc {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkLightingDescHolder {
    u8 pad_0x0[0x8];
    UnkLightingDesc* desc;
};

struct UnkLightingLight {
    u8 pad_0x0[0x8];
    UnkLightingDescHolder* holder;
};

struct UnkLightingComponent {
    u8 pad_0x0[0x60];
    UnkLightingLight** lights;
    u32 count;
};

void fn_71008e60fc(UnkLightingLight* light, void* arg, f32 value);

void fn_71008e7f18(UnkLightingComponent* self, const u64* id, void* arg, f32 value) {
    for (UnkLightingLight **p = self->lights, **end = p + self->count; p != end; ++p) {
        UnkLightingLight* light = *p;
        UnkLightingDesc* desc = light->holder->desc;
        u64 key = *id;
        if (desc->id == key)
            fn_71008e60fc(light, arg, value);
    }
}
