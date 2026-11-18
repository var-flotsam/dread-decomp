// CSideEnemyMovement unit. fn_71009b699c: the movement's direction. Outside state 1 it is the stored one
// (+0x58); in state 1 it is the fixed direction (+0x9dc) or the one between two points (+0x9a0 to +0x9a8,
// turned round when the animation's flag 2 is set), normalised. Names and layouts are placeholders.
#include <math.h>

#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSideEnemyMovement_l9b699c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x20];
    UnkVec3f direction;  // 0x58
    u8 pad_0x64[0x990 - 0x64];
    u8 useFixedDirection;  // 0x990
    u8 pad_0x991[0xf];
    f32 fromX;  // 0x9a0
    f32 fromY;
    f32 toX;  // 0x9a8
    f32 toY;
    u8 pad_0x9b0[0x2c];
    f32 fixedX;  // 0x9dc
    f32 fixedY;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkVec3f fn_71009b699c(UnkSideEnemyMovement_l9b699c* self) {
    UnkVec3f dir;
    if (fn_71010cc3a4(self) == 1) {
        dir = UnkVec3f(0.0f, 0.0f, 0.0f);
        void* animation = self->owner->field_0x220;
        if (animation) {
            if (self->useFixedDirection) {
                dir = UnkVec3f(self->fixedX, self->fixedY, 0.0f);
            } else {
                dir = UnkVec3f(self->toX - self->fromX, self->toY - self->fromY, 0.0f);
                if (fn_7101178524(static_cast<UnkAnimFlagsHolder_b47a3*>(animation), 2)) {
                    dir.x = -dir.x;
                    dir.y = -dir.y;
                    dir.z = -dir.z;
                }
            }
        }
        f32 inv = 1.0f / (sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) + 0.0001f);
        dir = UnkVec3f(dir.x * inv, inv * dir.y, inv * dir.z);
    } else {
        dir = self->direction;
    }
    return dir;
}
