// CCentralUnitAIComponent's unit (tools/tu_map.py unit 614): moves the owner towards the point it
// follows (+0x14720), at most 800 units a second (100 once that point has reached the goal at
// +0x14714); a point that has run more than 50 units off the goal, or one the owner has reached,
// jumps to the goal. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkCentralUnitOwner_l4040bc {
    void** vtable;
    u8 pad_0x8[0x30];
    f32 transform[3][4];  // 0x38, rows; the position in the last column

    UnkVec3f GetPosition() const { return UnkVec3f(transform[0][3], transform[1][3], transform[2][3]); }
    void SetPosition(const UnkVec3f* position, bool flag) {
        reinterpret_cast<void (*)(UnkCentralUnitOwner_l4040bc*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(
            this, position, flag);
    }
};

struct UnkCentralUnitAI_l4040bc {
    u8 pad_0x0[0x30];
    UnkCentralUnitOwner_l4040bc* owner;  // 0x30
    u8 pad_0x38[0x14714 - 0x38];
    UnkVec3f goal;     // 0x14714
    UnkVec3f follow;  // 0x14720
};
static_assert(__builtin_offsetof(UnkCentralUnitAI_l4040bc, follow) == 0x14720, "layout");

static inline bool Near(f32 v, f32 ref) { return !(v > ref + 0.001f) && !(v < ref - 0.001f); }

static inline bool Near(const UnkVec3f& v, const UnkVec3f& ref) {
    return Near(v.x, ref.x) && Near(v.y, ref.y) && Near(v.z, ref.z);
}

// Whether v lies in [ref - 0.001, ref + 0.001].
static inline bool Within(f32 v, f32 ref) { return v <= ref + 0.001f && ref - 0.001f <= v; }

static inline bool Within(const UnkVec3f& v, const UnkVec3f& ref) {
    return Within(v.x, ref.x) && Within(v.y, ref.y) && Within(v.z, ref.z);
}

// `from` moved towards `to` by at most `step`.
static inline f32 Approach(f32 from, f32 to, f32 step) {
    f32 lo = from - step;
    f32 hi = step + from;
    f32 above = lo < to ? to : lo;
    f32 below = hi > to ? to : hi;
    return to > from ? below : above;
}

void fn_71004040bc(UnkCentralUnitAI_l4040bc* self, f32 dt) {
    UnkCentralUnitOwner_l4040bc* owner = self->owner;
    UnkVec3f position = owner->GetPosition();
    if (!Near(self->follow, position)) {
        f32 speed = Within(self->goal, self->follow) ? 100.0f : 800.0f;
        f32 step = speed * dt;
        UnkVec3f next;
        next.x = Approach(position.x, self->follow.x, step);
        next.y = Approach(position.y, self->follow.y, step);
        next.z = Approach(position.z, self->follow.z, step);
        position = next;
        f32 dx = position.x - self->goal.x;
        f32 dy = position.y - self->goal.y;
        f32 dz = position.z - self->goal.z;
        if (dx * dx + dy * dy + dz * dz <= 2500.0f)
            owner->SetPosition(&position, true);
        else
            self->follow = self->goal;
    } else if (!Near(self->goal, self->follow)) {
        self->follow = self->goal;
    }
}
