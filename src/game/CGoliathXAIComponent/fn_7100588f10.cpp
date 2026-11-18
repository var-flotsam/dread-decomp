// CGoliathXAIComponent's unit (tools/tu_map.py unit 838). fn_7100588f10: the owner's bounds: the base
// bounds, grown sideways toward the target: past the arena's near edge (50 units out), scaled by how
// far the target is from the arena's centre. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkVec3 {
    f32 x, y, z;
};

struct UnkAabb {
    UnkVec3 min;
    UnkVec3 max;
};

// The arena's horizontal extent.
struct UnkGoliathArena_l588f10 {
    f32 left;
    f32 field_0x4;
    f32 right;
};

struct UnkGoliathXAI_l588f10 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14c38];
    UnkGoliathArena_l588f10 arena;  // 0x14c70
};
static_assert(__builtin_offsetof(UnkGoliathXAI_l588f10, arena) == 0x14c70, "layout");

static inline f32 Min(f32 a, f32 b) { return a < b ? a : b; }
static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }
static inline f32 Clamp01(f32 v) { return v <= 0.0f ? 0.0f : (v >= 1.0f ? 1.0f : v); }

// Grows the box to take in the point.
static inline void Grow(UnkAabb* box, const UnkVec3& p) {
    box->min = UnkVec3{Min(box->min.x, p.x), Min(box->min.y, p.y), Min(box->min.z, p.z)};
    box->max = UnkVec3{Max(box->max.x, p.x), Max(box->max.y, p.y), Max(box->max.z, p.z)};
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100588f10(UnkGoliathXAI_l588f10* self, UnkAabb* out) {
    fn_71011061d0(reinterpret_cast<UnkBoundsComponent*>(self), out);
    UnkGoliathArena_l588f10& arena = self->arena;
    f32 centre = arena.left + (arena.right - arena.left) * 0.5f;
    f32 target_x = fn_7100d7c990(self->owner)->field_0x44;
    f32 weight = Clamp01(fabsf(target_x - centre) / 500.0f);
    // The box's corner on the target's side, moved out to the edge.
    if (target_x - centre < 0.0f) {
        f32 edge = arena.left - 50.0f;
        UnkVec3 p = out->min;
        if (!(weight >= 1.0f))
            edge = weight * edge + (1.0f - weight) * p.x;
        p.x = edge;
        Grow(out, p);
    } else {
        f32 edge = arena.right + 50.0f;
        UnkVec3 p = out->max;
        if (!(weight >= 1.0f))
            edge = weight * edge + (1.0f - weight) * p.x;
        p.x = edge;
        Grow(out, p);
    }
}
