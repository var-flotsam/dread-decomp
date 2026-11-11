// Unit 1386 (tools/tu_map.py, no name hint): like fn_71008e8df4 (src/game/UnkUnit1385/UnkUnit1385_b43.cpp) it
// pushes the object's values at +0xac, +0xb0 to its owner (+0x120): the first, at least zero, into the light
// reached through the owner's +0x138 and +0x78 (+0x7c); the second, at least 5, through fn_710020c9ac on the
// owner's member at +0x20. Then it points the light's vector at +0x30 along its own direction, the length
// being the third value (+0xb4), marks it dirty (+0x1b8) and updates the +0x138 object (fn_71001bdc3c).
// Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU251.h"
#include "game/UnkTU290.h"
#include "types.h"

// The store may alias any other access: the owner pointer is read again after it.
typedef f32 __attribute__((may_alias)) UnkAliasF32_b0187i;

struct UnkLight_b0187i {
    u8 pad_0x0[0x30];
    f32 direction[3];  // 0x30
    u8 pad_0x3c[0x40];
    f32 value;  // 0x7c
    u8 pad_0x80[0x138];
    bool dirty;  // 0x1b8
};
static_assert(__builtin_offsetof(UnkLight_b0187i, dirty) == 0x1b8, "UnkLight_b0187i layout");

struct UnkLightHolder_b0187i {
    u8 pad_0x0[0x78];
    UnkLight_b0187i* light;  // 0x78
};

struct UnkLightOwner_b0187i {
    u8 pad_0x0[0x20];
    u8 member[0x118];  // 0x20
    UnkLightHolder_b0187i* holder;  // 0x138
};

struct UnkLightSelf_b0187i {
    u8 pad_0x0[0xac];
    f32 a;  // 0xac
    f32 b;  // 0xb0
    f32 c;  // 0xb4
    u8 pad_0xb8[0x68];
    UnkLightOwner_b0187i* owner;  // 0x120
};
static_assert(__builtin_offsetof(UnkLightSelf_b0187i, owner) == 0x120, "UnkLightSelf_b0187i layout");

static inline f32 Max_b0187i(f32 x, f32 y) { return x < y ? y : x; }

void fn_71008e966c(UnkLightSelf_b0187i* self) {
    f32 a = self->a;
    f32 b = self->b;
    f32 length = self->c;
    *reinterpret_cast<UnkAliasF32_b0187i*>(&self->owner->holder->light->value) = Max_b0187i(a, 0.0f);
    fn_710020c9ac(self->owner->member, Max_b0187i(b, 5.0f));
    UnkLightOwner_b0187i* owner = self->owner;
    UnkLight_b0187i* light = owner->holder->light;
    f32 inv = 1.0f / (sqrtf(light->direction[0] * light->direction[0] + light->direction[1] * light->direction[1] +
                            light->direction[2] * light->direction[2]) + 0.0001f);
    f32 x = light->direction[0] * inv;
    f32 y = light->direction[1] * inv;
    f32 z = light->direction[2] * inv;
    UnkLightHolder_b0187i* holder = owner->holder;
    light = holder->light;
    light->direction[0] = length * x;
    light->direction[1] = length * y;
    light->direction[2] = length * z;
    light->dirty = true;
    fn_71001bdc3c(holder);
}
