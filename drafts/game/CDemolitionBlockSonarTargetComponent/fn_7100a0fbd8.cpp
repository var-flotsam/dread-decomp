// CDemolitionBlockSonarTargetComponent's unit: whether a point lies within `tolerance` of the block's edge on
// both axes, the edge being the side of the block's bounds (fn_71008653c0 on its +0x2a0 object) nearer the
// point. False while the component is inactive (fn_710024fbc8) or its part does not hold (fn_71008c5898).
// Names and layouts are placeholders.
#include <math.h>

#include "game/CActorComponent.h"
#include "types.h"

// A 2D box, returned through memory (its copy constructor is not trivial).
struct UnkSonarBounds_la0fbd8 {
    f32 min_x, min_y;
    f32 max_x, max_y;

    UnkSonarBounds_la0fbd8(const UnkSonarBounds_la0fbd8& o)
        : min_x(o.min_x), min_y(o.min_y), max_x(o.max_x), max_y(o.max_y) {}
};

struct UnkSonarPoint_la0fbd8 {
    f32 x, y;
};

struct UnkSonarPart_la0fbd8;

bool fn_71008c5898(UnkSonarPart_la0fbd8* part);
UnkSonarBounds_la0fbd8 fn_71008653c0(void* object, s32 mask, u32 flags);

struct UnkSonarActor_la0fbd8 {
    u8 pad_0x0[0x2a0];
    void* field_0x2a0;
};

struct UnkSonarPart_la0fbd8 {
    u8 pad_0x0[0x30];
    UnkSonarActor_la0fbd8* actor;  // 0x30
};

struct UnkSonarHolder_la0fbd8 {
    u8 pad_0x0[0x10];
    UnkSonarPart_la0fbd8* part;  // 0x10
};

struct UnkSonarTarget_la0fbd8 {
    u8 pad_0x0[0x40];
    UnkSonarHolder_la0fbd8* holder;  // 0x40
};

// Whether `v` is within `tolerance` of the end of [lo, hi] it is nearer to.
static inline bool NearEnd(f32 v, f32 lo, f32 hi, f32 tolerance) {
    f32 center = (lo + hi) * 0.5f;
    f32 half = (hi - lo) * 0.5f;
    return !(fabsf(v - (center + (v <= center ? -half : half))) > tolerance);
}

bool fn_7100a0fbd8(UnkSonarTarget_la0fbd8* self, const UnkSonarPoint_la0fbd8* point, f32 tolerance) {
    if (!fn_710024fbc8(self) || !self->holder)
        return false;
    UnkSonarPart_la0fbd8* part = self->holder->part;
    if (!part || !fn_71008c5898(part))
        return false;
    UnkSonarBounds_la0fbd8 bounds = fn_71008653c0(self->holder->part->actor->field_0x2a0, 0x7fffffff, 0);
    return NearEnd(point->x, bounds.min_x, bounds.max_x, tolerance) &
           NearEnd(point->y, bounds.min_y, bounds.max_y, tolerance);
}
