// UnkTU2568 unit: moves a walker along the sides of the navigation mesh by `step`. A step shorter than the
// walker's distance is shortened in front of a sharp corner; a longer one walks on from side to side, as
// far as the corners allow, then moves on along the last side. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkNavMeshCell.h"
#include "types.h"

using base::math::CVector2D;

bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);

struct UnkSideWalker_lec2b34 {
    u8 pad_0x0[0x18];
    u8 reverse;  // walks the sides backwards when set
    u8 pad_0x19[0x7];
    u32 flags;
    u8 pad_0x24[0x4];
    CVector2D* pos;
    u8 pad_0x30[0x10];
    CVector2D widths;
    f32 distance;
    bool use_end_distance;
    u8 pad_0x4d[0x3];
    f32 end_distance;  // the distance when the walker stands at the side's end
    u8 pad_0x54[0x3c];
    u16* cell;
    u8* side;
};

// Whether `v` is within 0.1 of `c`.
inline bool Near_lec2b34(f32 v, f32 c) {
    return c + 0.1f >= v && !(c - 0.1f > v);
}

void fn_7100ec2b34(f32 step, UnkNavMesh* mesh, UnkSideWalker_lec2b34* self, u16 cell, u8 side, UnkSegment2D* edge,
                   const CVector2D* dir, const CVector2D* point) {
    u16 corner_cell;
    u8 corner_side;
    f32 distance = self->distance;
    if (self->use_end_distance) {
        UnkVec2Result n = fn_7100125e78(edge);
        if (Near_lec2b34(n.x, point->x) && Near_lec2b34(n.y, point->y))
            distance = self->end_distance;
    }

    const CVector2D* widths = &self->widths;
    f32 width = widths->x;
    if (step + 0.1f < distance || step - 0.1f > distance) {
        if (distance < step) {
            // Shorter than the step: stop short of a sharp corner on the next side.
            corner_cell = 0xffff;
            corner_side = 0xff;
            UnkSegment2D next;
            next.start.x = 0.0f;
            next.start.y = 0.0f;
            next.end.x = 0.0f;
            next.end.y = 0.0f;
            next.field_0x10 = -1.0f;
            if (fn_710027c460(mesh, cell, side, self->reverse == 0, widths, self->flags, &corner_cell, &corner_side)) {
                fn_71002afea8(fn_710027a328(mesh, corner_cell), &corner_side,
                              reinterpret_cast<UnkNavMeshSideSegment*>(&next));
                UnkVec2Result a = fn_7100125e78(edge);
                UnkVec2Result b = fn_7100125e78(&next);
                if (a.x * b.x + a.y * b.y < 0.95f) {
                    f32 limit = step - ceilf(width * 0.5f / 50.0f) * 50.0f;
                    distance = distance < limit ? distance : limit;
                    if (distance <= 0.001f)
                        distance = self->distance;
                }
            }
            self->pos->x = distance * dir->x + self->pos->x;
            self->pos->y = distance * dir->y + self->pos->y;
            return;
        }

        // Longer than the step: walk on along the following sides.
        u16 next_cell = 0xffff;
        u8 next_side = 0xff;
        CVector2D along = *dir;
        f32 remaining = distance;
        if (distance > 0.0f) {
            for (remaining = distance - step; remaining > 0.0f;) {
                fn_710027c460(mesh, cell, side, self->reverse == 0, widths, self->flags, &next_cell, &next_side);
                cell = next_cell;
                *self->cell = cell;
                side = next_side;
                *self->side = side;
                fn_71002afea8(fn_710027a328(mesh, next_cell), &next_side, reinterpret_cast<UnkNavMeshSideSegment*>(edge));
                step = fn_7100125fbc(edge);
                f32 ex = edge->end.x - edge->start.x;
                f32 ey = edge->end.y - edge->start.y;
                f32 length = sqrtf(ex * ex + ey * ey);
                if (length != 0.0f) {
                    f32 inv = 1.0f / length;
                    ex *= inv;
                    ey *= inv;
                }
                f32 sign = self->reverse == 0 ? 1.0f : -1.0f;
                CVector2D next_along = {ex * sign, ey * sign};
                const CVector2D* corner = self->reverse == 0 ? &edge->end : &edge->start;
                f32 left = next_along.x * along.x + next_along.y * along.y > 0.95f ? remaining : distance;
                *self->pos = *corner;
                along = next_along;
                remaining = left - step;
            }
        }
        self->pos->x = remaining * along.x + self->pos->x;
        self->pos->y = remaining * along.y + self->pos->y;
        return;
    }
    self->pos->x = dir->x * step + self->pos->x;
    self->pos->y = dir->y * step + self->pos->y;
}
