// Unit 442 (the path an AI component follows): the distance along the path from a point on one edge
// to a point on another. A closed path gives the shorter of the two ways round.
// Names and layouts are placeholders.
#include <float.h>

#include "types.h"

struct UnkAIPath {
    void* vertices;
    s32 count;
    u8 unk_0xc[0x20];
    u8 closed;
    u8 unk_0x2d[0x1b];
    f32* lengths;  // one per edge
};

static inline u32 EdgeCount(const UnkAIPath* self) {
    return self->count - (!self->closed & (self->count != 0));
}

// An edge's length (-1 past the last edge).
static inline f32 EdgeLength(const UnkAIPath* self, u32 i) {
    return i < EdgeCount(self) ? self->lengths[i] : -1.0f;
}

// NON_MATCHING(minor): the two wrap-around steps add their operands the other way round (same instructions otherwise).
f32 fn_710035e5f8(const UnkAIPath* self, u32 edge_from, u32 edge_to, f32 t_from, f32 t_to) {
    if (edge_from == edge_to) {
        return (t_to - t_from) * EdgeLength(self, edge_from);
    }

    if (self->closed) {
        // Try both ways round and keep the shorter.
        f32 best = FLT_MAX;
        for (s32 step = -1; step <= 1; step += 2) {
            f32 distance = 0.0f;
            if (step < 0) {
                distance += EdgeLength(self, edge_from) * t_from;
                distance += (1.0f - t_to) * EdgeLength(self, edge_from);
            } else {
                distance += (1.0f - t_from) * EdgeLength(self, edge_from);
                distance += EdgeLength(self, edge_to) * t_to;
            }

            // Walk the edges in between, wrapping at the ends.
            const s32 count = self->count;
            const s32 wrap = count + step;
            for (s32 i = (edge_from + wrap) % count; i != edge_to; i = (wrap + i) % count) {
                distance += EdgeLength(self, i);
            }

            if (distance < best) {
                best = distance;
            }
        }
        return best;
    }

    // An open path: one way only, signed by the direction.
    f32 distance = 0.0f;
    s32 step;
    if (edge_from < edge_to) {
        distance += (1.0f - t_from) * EdgeLength(self, edge_from);
        distance += EdgeLength(self, edge_to) * t_to;
        step = 1;
    } else if (edge_to < edge_from) {
        distance -= EdgeLength(self, edge_from) * t_from;
        distance -= (1.0f - t_to) * EdgeLength(self, edge_from);
        step = -1;
    } else {
        step = 0;
    }
    for (u32 i = edge_from + step; i != edge_to; i += step) {
        distance += EdgeLength(self, i) * static_cast<f32>(step);
    }
    return distance;
}
