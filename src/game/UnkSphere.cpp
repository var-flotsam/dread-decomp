// A positioned sphere (include/game/UnkSphere.h), translation unit 308 of data/tu_map.csv. The
// unit's overlap test fn_7100216c2c is the frozen Phase 3 sample src/flag_id/fn_7100216c2c.cpp,
// which keeps its own view of the layout.
#include <math.h>

#include "game/UnkSphere.h"
#include "types.h"

// Stores the position (word by word, as the UnkVec3f copy does) and its quantised copy: each
// component times ten, truncated.
void fn_7100216978(UnkSphere* self, const UnkVec3f* position) {
    self->pos = *position;
    self->cell[0] = (s32)(self->pos.x * 10.0f);
    self->cell[1] = (s32)(self->pos.y * 10.0f);
    self->cell[2] = (s32)(self->pos.z * 10.0f);
}

// Grows the range [*lo, *hi] to take in `value` (an empty range becomes just `value`); returns the
// new upper bound.
static inline f32 WidenRange(f32* lo, f32* hi, f32 value) {
    f32* bound = nullptr;
    f32 top = *hi;
    if (!(*lo <= top)) {
        bound = hi;
        top = value;
        *lo = value;
    } else if (*lo > value) {
        bound = lo;
    } else if (top < value) {
        bound = hi;
        top = value;
    }
    if (bound)
        *bound = value;
    return top;
}

// NON_MATCHING(minor): registers only; a register copy and the first range store swap places
void fn_71002169e0(UnkSphere* self, const UnkVec3f* point) {
    f32 dx = point->x - self->pos.x;
    f32 dy = point->y - self->pos.y;
    f32 dz = point->z - self->pos.z;
    f32 beyond = sqrtf(dx * dx + dy * dy + dz * dz) - self->radius;
    beyond = !(beyond < 0.0f) ? beyond : 0.0f;
    f32 top = WidenRange(&self->field_0x24, &self->field_0x28, beyond);
    WidenRange(&self->field_0x2c, &self->base_radius, beyond);
    s64 fixed = (s64)(top * 10000.0f);
    self->radius_fixed = fixed;
    self->radius = top;
    self->field_0x4c = fixed == 0;
}

// NON_MATCHING(minor): registers only; the two x and y sums take their operands in the other order
bool fn_7100216ca4(UnkSphere* self, const UnkSphere* other) {
    f32 d = __builtin_fabsf(other->pos.x - self->pos.x) + other->radius;
    f32 lo = self->field_0x24;
    if (!(lo <= d))
        return false;
    f32 hi = self->field_0x28;
    if (!(hi >= d))
        return false;
    d = __builtin_fabsf(other->pos.y - self->pos.y) + other->radius;
    if (!(lo <= d && hi >= d))
        return false;
    d = other->radius + __builtin_fabsf(other->pos.z - self->pos.z);
    if (!(lo <= d))
        return false;
    return hi >= d;
}
