// CAbilityShinespark's unit: the direction state (0 to 8) for a direction vector: none when it points too
// little forward, else by its angle against the tunable limits (one set on the ground, one in the air),
// split by whether it points along the actor's facing. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/CAbilityShinespark.h"
#include "types.h"

struct UnkDirOut_b56 {
    f32 x;
    f32 y;
    f32 z;
};

// The angle limits, from the steepest down: four on the ground, four in the air.
struct UnkShinesparkTunable_l32f678 {
    u8 pad_0x0[0x58];
    f32 ground_up;      // 0x58
    f32 ground_high;    // 0x5c
    f32 ground_middle;  // 0x60
    f32 ground_low;     // 0x64
    f32 air_up;         // 0x68
    f32 air_high;       // 0x6c
    f32 air_middle;     // 0x70
    f32 air_low;        // 0x74
};

// The actor's movement helpers: each has the facing at +0x58.
struct UnkFacing_l32f678 {
    u8 pad_0x0[0x58];
    f32 facing;
};
struct UnkMover_l32f678 {
    u8 pad_0x0[0x58];
    f32 facing;
    u8 pad_0x5c[0x2325 - 0x5c];
    bool field_0x2325;
};
struct UnkActor_l32f678 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f8 - 0x78];
    UnkFacing_l32f678* field_0x1f8;
    u8 pad_0x200[0x8];
    UnkMover_l32f678* field_0x208;
};
struct UnkOwner_l32f678 {
    u8 pad_0x0[0x30];
    UnkActor_l32f678* actor;
};
struct UnkShinespark_l32f678 {
    u8 pad_0x0[0xc0];
    UnkOwner_l32f678* owner;
    u8 pad_0xc8[0x441 - 0xc8];
    bool in_air;
};

bool fn_71009e1310(void* self);

// The actor's facing: its mover's, or the one its transform gives while the mover is busy.
static inline f32 Facing(UnkActor_l32f678* actor) {
    if (UnkMover_l32f678* mover = actor->field_0x208) {
        if (mover->field_0x2325 || fn_71009e1310(mover))
            return fn_710002614c(&actor->transform).m[2];
        return mover->facing;
    }
    if (UnkFacing_l32f678* facing = actor->field_0x1f8)
        return facing->facing;
    return 0.0f;
}

u32 fn_710032f678(UnkShinespark_b56* shinespark, const UnkDirOut_b56* dir) {
    if (!(dir->z >= 0.2f))
        return 0;

    UnkShinespark_l32f678* self = reinterpret_cast<UnkShinespark_l32f678*>(shinespark);
    const UnkShinesparkTunable_l32f678* tunable = reinterpret_cast<const UnkShinesparkTunable_l32f678*>(&g_unk_71726b0378);
    bool in_air = self->in_air;
    f32 up = in_air ? tunable->air_up : tunable->ground_up;
    f32 high = in_air ? tunable->air_high : tunable->ground_high;
    f32 middle = in_air ? tunable->air_middle : tunable->ground_middle;
    f32 low = in_air ? tunable->air_low : tunable->ground_low;

    f32 angle = atan2f(dir->y, fabsf(dir->x));
    f32 side = dir->x * Facing(self->owner->actor);
    if (angle > up)
        return 1;
    if (angle > high)
        return side < 0.0f ? 3 : 2;
    if (angle > middle)
        return side < 0.0f ? 5 : 4;
    if (angle > low)
        return side < 0.0f ? 7 : 6;
    return 8;
}
