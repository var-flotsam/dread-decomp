// CSideEnemyMovement unit (tools/tu_map.py unit 1471): before handing over to the CEnemyMovement
// handler (fn_71010c9a50), when there is a target and the first flag is set, checks whether the
// target lies behind the facing (vf_0x370; the direction flips unless the second flag is set) and
// if so turns around (vf_0x348). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkEnemyMovement_9a50;
void fn_71010c9a50(UnkEnemyMovement_9a50* self, void* unused, bool a, bool b, bool c);

struct UnkSideEnemyMovement_l9b5e58 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30

    UnkVec3f vf_0x370() {
        return reinterpret_cast<UnkVec3f (*)(UnkSideEnemyMovement_l9b5e58*)>(vtable[0x370 / 8])(this);
    }
    void vf_0x348() { reinterpret_cast<void (*)(UnkSideEnemyMovement_l9b5e58*)>(vtable[0x348 / 8])(this); }
};

struct UnkDir_l9b5e58 {
    f32 x;
    f32 y;
    f32 z;
};

static inline UnkDir_l9b5e58 operator-(const UnkDir_l9b5e58& a, const UnkDir_l9b5e58& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline UnkDir_l9b5e58 operator-(const UnkDir_l9b5e58& a) { return {-a.x, -a.y, -a.z}; }

static inline UnkDir_l9b5e58 Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

void fn_71009b5e58(UnkSideEnemyMovement_l9b5e58* self, UnkActor* target, bool a, bool b, bool c) {
    if (target && a) {
        UnkVec3f facing = self->vf_0x370();
        UnkDir_l9b5e58 toward = Pos(target) - Pos(self->owner);
        UnkDir_l9b5e58 d = b ? toward : -toward;
        if (d.x * facing.x + d.y * facing.y + d.z * facing.z < 0.0f)
            self->vf_0x348();
    }
    fn_71010c9a50(reinterpret_cast<UnkEnemyMovement_9a50*>(self), target, a, b, c);
}
