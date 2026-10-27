// From unit 244 of tools/tu_map.py (0x71001b76ec-0x71001bb60c, no reflected class): a view
// frustum's six planes (at +0x178 of the view object) tested against a bounding sphere. The sphere
// remembers the plane that last rejected it and the test starts there, walking the planes in the
// order a next-plane table gives. Names and layouts are placeholders.
// Holds fn_71001bab30.
#include "types.h"

// A plane: its normal and its distance term.
struct UnkPlane {
    f32 x;
    f32 y;
    f32 z;
    f32 d;
};

struct UnkFrustumView {
    u8 pad_0x0[0x178];
    UnkPlane planes[6];
};

// A plane's index (0..5). An enum type: its stores don't alias the sphere's floats.
enum class UnkPlaneIndex : u8 {};

struct UnkBoundSphere {
    u8 pad_0x0[0x8];
    f32 radius;
    u8 pad_0xc[0xc];
    f32 center_x;
    f32 center_y;
    f32 center_z;
    u8 pad_0x24[0x29];
    UnkPlaneIndex plane;  // the plane the test starts with
};
static_assert(__builtin_offsetof(UnkBoundSphere, plane) == 0x4d, "UnkBoundSphere placeholder layout");

// The plane tested after each plane.
extern const u8 g_unk_71015e9322[6] __attribute__((visibility("hidden")));

// 1 when the sphere is outside a plane, -1 when it is inside all six (or too small to test),
// 0 when it crosses one. The sphere's start plane moves on with every plane tested.
// NON_MATCHING(minor): the original computes the fifth plane's inside test (cset) first; the same
// six compares and ands, scheduled in another order.
s32 fn_71001bab30(const UnkFrustumView* self, UnkBoundSphere* sphere) {
    f32 r = sphere->radius;
    if (r < 0.0001f)
        return -1;
    u32 index = (u32)sphere->plane;
    bool inside = true;
    for (u32 i = 0; i < 6; ++i) {
        const UnkPlane& p = self->planes[index];
        f32 dist = p.x * sphere->center_x + p.y * sphere->center_y + p.z * sphere->center_z + p.d;
        if (dist > sphere->radius)
            return 1;
        if (dist > -r)
            inside = false;
        index = g_unk_71015e9322[index];
        sphere->plane = (UnkPlaneIndex)index;
    }
    return inside ? -1 : 0;
}
