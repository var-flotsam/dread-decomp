// UnkTU356 (tools/tu_map.py unit 356): pulls a path taut through a list of portals (the funnel
// algorithm), marking the portals whose point the path turns at. Names and layouts are placeholders.
#include <new>

#include "base/math/CVector.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU376.h"
#include "types.h"

using base::math::CVector2D;

// The portal's payload, built by its out-of-line constructor.
struct UnkPortalData_l285a58 {
    u32 data[0x40];

    UnkPortalData_l285a58() { fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(this)); }
};

// One portal: its two ends, and the point the path takes through it.
struct UnkPortal_l285a58 {
    u32 type;  // 2: a single point the path must pass
    u16 field_0x4;
    u8 field_0x6;
    CVector2D left;   // 0x8
    CVector2D right;  // 0x10
    UnkPortalData_l285a58 data;  // 0x18
    CVector2D point;  // 0x118
    bool on_path;     // 0x120
    u8 pad_0x121[0x7];

    UnkPortal_l285a58()
        : type(0), field_0x4(0xffff), field_0x6(0xff), left(), right(), data(), point(), on_path(false) {}
};
static_assert(sizeof(UnkPortal_l285a58) == 0x128, "UnkPortal_l285a58 layout");

struct UnkPortalList_l285a58 {
    u32 count;
    UnkPortal_l285a58 portals[1];

    // The portal at `index`, growing the list to cover it first.
    UnkPortal_l285a58& At(u32 index) {
        if (index >= count) {
            for (u32 i = count; i <= index; i++)
                new (&portals[i]) UnkPortal_l285a58();
            count = index + 1;
        }
        return portals[index];
    }
};

// Twice the signed area of the triangle (a, b, c).
static inline f32 TriArea(const CVector2D& a, const CVector2D& b, const CVector2D& c) {
    f32 abx = b.x - a.x;
    f32 aby = b.y - a.y;
    f32 acx = c.x - a.x;
    f32 acy = c.y - a.y;
    return aby * acx - abx * acy;
}

static inline bool Near(f32 a, f32 b) {
    return !(a + 0.001f < b) && !(b < a - 0.001f);
}

static inline bool Same(const CVector2D& a, const CVector2D& b) {
    return Near(a.x, b.x) && Near(a.y, b.y);
}

void fn_7100285a58(void* self, UnkPortalList_l285a58* list) {
    UnkPortal_l285a58& first = list->At(0);
    first.on_path = true;
    first.point = first.left;
    CVector2D apex = first.left;
    CVector2D left = apex;
    CVector2D right = first.right;
    u32 left_index = 0;
    u32 right_index = 0;
    for (u32 i = 1; i < list->count; i++) {
        if (list->At(i).type == 2) {
            CVector2D point = list->At(i).left;
            list->At(i).on_path = true;
            list->At(i).point = point;
            apex = point;
            left = point;
            right = point;
            continue;
        }
        CVector2D next_right = list->At(i).right;
        if (TriArea(apex, right, next_right) <= 0.0f) {
            if (Same(apex, right) || !(TriArea(apex, left, next_right) < 0.0f)) {
                right = next_right;
                right_index = i;
            } else {
                // The right end crossed the left one: the left end is the next turn.
                i = left_index;
                UnkPortal_l285a58& turn = list->At(i);
                turn.on_path = true;
                turn.point = left;
                apex = left;
                right = left;
                right_index = left_index;
                continue;
            }
        }
        CVector2D next_left = list->At(i).left;
        if (TriArea(apex, left, next_left) < 0.0f)
            continue;
        if (Same(apex, left) || TriArea(apex, right, next_left) <= 0.0f) {
            left = next_left;
            left_index = i;
        } else {
            // The left end crossed the right one: the right end is the next turn.
            i = right_index;
            UnkPortal_l285a58& turn = list->At(i);
            turn.on_path = true;
            turn.point = right;
            apex = right;
            left = right;
            left_index = right_index;
        }
    }
    UnkPortal_l285a58& last = list->portals[list->count - 1];
    last.point = last.left;
    last.on_path = true;
}
