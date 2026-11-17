// CSclawkAIComponent's unit (tools/tu_map.py unit 1005): looks along the mover's path, both ways, for
// the next corner within reach and says which side it turns to; 0 when the path's normal changed.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/CEnemyMovement.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSclawkAI2_ovb0125;

extern const CVector2D g_unk_7101647780;

struct UnkSclawkPathPoint_l67ca84 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkSclawkPath_l67ca84 {
    UnkSclawkPathPoint_l67ca84* points;
    u32 count;
    u8 pad_0xc[0x20];
    u8 closed;  // 0x2c

    u32 SegmentCount() const { return count - ((count != 0) & (closed == 0)); }
    UnkSegment2D Segment(u32 i) const {
        const UnkSclawkPathPoint_l67ca84& end = i == count - 1 ? points[0] : points[i + 1];
        return UnkSegment2D(points[i].pos, end.pos);
    }
};

bool fn_710035e53c(UnkSclawkPath_l67ca84* path);

struct UnkSclawkMover_l67ca84 {
    u8 pad_0x0[0x971];
    u8 field_0x971;
    u8 pad_0x972[0x2];
    CVector2D pos;  // 0x974
    u8 pad_0x97c[0x8];
    u32 segment;  // 0x984
    u8 pad_0x988[0x8];
    u8 field_0x990;
    u8 pad_0x991[0x4b];
    CVector2D dir;   // 0x9dc
    CVector2D side;  // 0x9e4
    u8 pad_0x9ec[0x20];
    u8 field_0xa0c;

    bool IsReversed() const { return (field_0xa0c != 0) != (field_0x971 != 0); }
};
static_assert(__builtin_offsetof(UnkSclawkMover_l67ca84, field_0xa0c) == 0xa0c, "layout");

struct UnkSclawkOwner_l67ca84 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;
    u8 pad_0x78[0x1a0];
    UnkSclawkMover_l67ca84* mover;  // 0x218
};

struct UnkSclawkTurn_l67ca84 {
    CVector2D normal;
    bool changed;
    bool pending;
};

struct UnkSclawkAI_l67ca84 {
    u8 pad_0x0[0x30];
    UnkSclawkOwner_l67ca84* owner;
    u8 pad_0x38[0x30];
    UnkSclawkPath_l67ca84 path;  // 0x68
    u8 pad_0x98[0x1464c];
    UnkSclawkTurn_l67ca84 turn;  // 0x146e4
};
static_assert(__builtin_offsetof(UnkSclawkAI_l67ca84, turn) == 0x146e4, "layout");

static inline bool IsNear(f32 a, f32 b, f32 eps) {
    return a + eps >= b && a - eps <= b;
}

template <class V>
static inline bool IsNear(const V& a, const CVector2D& b) {
    return IsNear(a.x, b.x, 0.1f) && IsNear(a.y, b.y, 0.1f);
}

static inline f32 Distance(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

u32 fn_710067ca84(UnkSclawkAI2_ovb0125* ai) {
    UnkSclawkAI_l67ca84* self = reinterpret_cast<UnkSclawkAI_l67ca84*>(ai);
    UnkSclawkMover_l67ca84* move = self->owner->mover;
    if (fn_71010cc3a4(move) != 1 || move->field_0x990)
        return 1;

    UnkSclawkPath_l67ca84& path = self->path;
    {
        u32 idx = move->segment;
        u32 next;
        if (move->IsReversed())
            next = idx != 0 ? idx - 1 : path.SegmentCount() - 1;
        else
            next = idx == path.SegmentCount() - 1 ? 0 : idx + 1;

        UnkSegment2D cur = path.Segment(idx);
        UnkSegment2D nxt = path.Segment(next);
        UnkVec2Result n0 = fn_7100125e78(&cur);
        UnkVec2Result n1 = fn_7100125e78(&nxt);
        f32 bend = n0.x * n1.x + n0.y * n1.y;
        const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
        CVector3D axis = {m.m[2], m.m[6], m.m[10]};
        CVector2D fwd;
        fn_710007b430(&fwd, &axis);
        if (IsNear(bend, 0.0f, 0.001f) && fwd.x * n1.x + fwd.y * n1.y < 0.0f) {
            const CVector2D& p = move->IsReversed() ? nxt.end : nxt.start;
            const UnkMatrix44& t = self->owner->transform;
            f32 dx = p.x - t.m[3];
            f32 dy = p.y - t.m[7];
            f32 dz = 0.0f - t.m[11];
            if (dx * dx + dy * dy + dz * dz < 10000.0f)
                return 1;
        }
    }

    for (s32 pass = 0; pass != 2; pass++) {
        // The result is not used in the original.
        fn_71010cc3a4(move);
        bool forward = pass == 1;
        f32 reach = forward == move->IsReversed() ? 300.0f : 50.0f;
        CVector2D dir = move->dir;
        CVector2D side = move->side;
        u32 i = move->segment;
        CVector2D pos = move->pos;
        s32 step = forward ? 1 : -1;
        do {
            UnkSegment2D seg = path.Segment(i);
            if (Distance(forward ? seg.end : seg.start, pos) >= reach)
                break;
            i += step;
            if (fn_710035e53c(&path)) {
                u32 n = path.SegmentCount();
                if (i >= n)
                    i = forward ? 0 : n - 1;
            }
            if (i >= path.SegmentCount())
                return 3;
            UnkSegment2D next = path.Segment(i);
            CVector2D q = forward ? next.end : next.start;
            f32 moved = Distance(q, pos);
            pos = q;
            f32 dx = next.end.x - next.start.x;
            f32 dy = next.end.y - next.start.y;
            f32 len = sqrtf(dx * dx + dy * dy);
            if (len != 0.0f) {
                f32 inv = 1.0f / len;
                dx *= inv;
                dy *= inv;
            }
            if (!(__builtin_fabsf(dir.x * dx + dir.y * dy) > 0.9f)) {
                f32 s = side.x * dx + side.y * dy;
                if (forward)
                    return s > 0.0f ? 2 : 1;
                return s < 0.0f ? 2 : 1;
            }
            reach -= moved;
        } while (reach > 0.0f);
    }

    UnkSclawkTurn_l67ca84& turn = self->turn;
    UnkSegment2D seg = path.Segment(move->segment);
    if (!IsNear(fn_7100125e78(&seg), turn.normal) || IsNear(g_unk_7101647780, turn.normal)) {
        UnkVec2Result n = fn_7100125e78(&seg);
        turn.normal.x = n.x;
        turn.normal.y = n.y;
        turn.changed = true;
        return 0;
    }
    if (turn.pending) {
        turn.pending = false;
        turn.changed = true;
    } else {
        turn.changed = false;
    }
    return 0;
}
