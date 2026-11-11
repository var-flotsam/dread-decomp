// CAbilityShinespark's unit: maps the current state (fn_710032f678) through a u32 flat map (values,
// keys, count at +0x400) to a direction 1-8 and writes the unit vector of the tunable angle for it
// (cos, sin, 1), with x mirrored for the odd directions 3, 5 and 7. Names are placeholders.
#include <math.h>

#include "game/CAbilityShinespark.h"
#include "types.h"

struct UnkShinesparkAngles_b56 {
    u8 pad_0x0[0x58];
    f32 angle_0x58;
    f32 angle_0x5c;
    f32 angle_0x60;
    f32 angle_0x64;
};
struct UnkTunable;
extern UnkTunable g_unk_71726b0378;

struct UnkDirOut_b56 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkDirMap_b56 {
    u32* values;
    u32* keys;
    u32 count;
};

struct UnkShinespark_b56 {
    u8 pad_0x0[0x400];
    UnkDirMap_b56 directions;  // 0x400
};

static inline u32* Find(UnkDirMap_b56* map, const u32* key) {
    for (u64 i = 0; i != map->count; ++i) {
        if (map->keys[i] == *key) {
            return &map->values[i];
        }
    }
    return map->values + map->count;
}

static inline void SetDir(UnkDirOut_b56* out, f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);
    out->x = c;
    out->y = s;
    out->z = 1.0f;
}

static inline void SetDirMirrored(UnkDirOut_b56* out, f32 angle) {
    f32 c = -cosf(angle);
    f32 s = sinf(angle);
    out->x = c;
    out->y = s;
    out->z = 1.0f;
}

void fn_710032f4cc(UnkShinespark_b56* self, UnkDirOut_b56* out) {
    u32 state = fn_710032f678(self, out);
    UnkDirMap_b56* map = &self->directions;
    u32* it = Find(map, &state);
    if (it == map->values + map->count) {
        return;
    }
    const UnkShinesparkAngles_b56* t = reinterpret_cast<const UnkShinesparkAngles_b56*>(&g_unk_71726b0378);
    switch (*it) {
    case 1:
        SetDir(out, t->angle_0x58 + 0.001f);
        break;
    case 2:
        SetDir(out, t->angle_0x5c + 0.001f);
        break;
    case 3:
        SetDirMirrored(out, t->angle_0x5c + 0.001f);
        break;
    case 4:
        SetDir(out, t->angle_0x60 + 0.001f);
        break;
    case 5:
        SetDirMirrored(out, t->angle_0x60 + 0.001f);
        break;
    case 6:
        SetDir(out, t->angle_0x64 + 0.001f);
        break;
    case 7:
        SetDirMirrored(out, t->angle_0x64 + 0.001f);
        break;
    case 8:
        SetDir(out, t->angle_0x64 - 0.001f);
        break;
    }
}
