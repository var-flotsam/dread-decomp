// CEnemyMovement's unit (tools/tu_map.py): two directions, the 2D one and the 3D one, from the
// object at +0x8e8 (fn_710096ee98, fn_71009704c8) while the owner's flag (+0x1c18, its +0x1b1) is
// clear and the state at +0x848 is 1; straight up otherwise. Both are returned in memory (their copy
// is written out). Names and layouts are placeholders.
#include "game/UnkTU1442.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkVec2f_A5 {
    f32 x, y;
    UnkVec2f_A5(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2f_A5(const UnkVec2f_A5& o) : x(o.x), y(o.y) {}
};

struct UnkEnemyOwner_A5 {
    u8 pad_0x0[0x1b1];
    bool flag;  // 0x1b1
};

struct UnkEnemyState_A5 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkEnemyMovement_A5 {
    u8 pad_0x0[0x848];
    UnkEnemyState_A5* state;  // 0x848
    u8 pad_0x850[0x98];
    u8 source[0x1330];  // 0x8e8
    UnkEnemyOwner_A5* owner;  // 0x1c18
};
static_assert(__builtin_offsetof(UnkEnemyMovement_A5, owner) == 0x1c18, "UnkEnemyMovement_A5 layout");

UnkVec3f fn_71009704c8(void* source);

static inline bool FromSource(UnkEnemyMovement_A5* self) {
    return !self->owner->flag && self->state && self->state->kind == 1;
}

UnkVec2f_A5 fn_71010c9a1c(UnkEnemyMovement_A5* self) {
    if (FromSource(self))
        return fn_710096ee98(self->source);
    return UnkVec2f_A5(0.0f, 1.0f);
}

UnkVec3f fn_71010c9ad0(UnkEnemyMovement_A5* self) {
    if (FromSource(self))
        return fn_71009704c8(self->source);
    return UnkVec3f(0.0f, 1.0f, 0.0f);
}
