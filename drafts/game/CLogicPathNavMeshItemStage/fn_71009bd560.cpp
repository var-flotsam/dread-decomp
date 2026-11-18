// CLogicPathNavMeshItemStage's unit (tools/tu_map.py unit 1481): the stage's direction, from the first point
// of the owner's path (fn_7100382150) to the last point of its last segment, set through fn_710007b444; zero
// (g_unk_7101647780) without a path or points. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU55.h"
#include "game/behaviortree/CGoToWallFronPathAction.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern const base::math::CVector2D g_unk_7101647780;

struct UnkNavPathPoint_l9bd560 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* vf_0x18();  // the point's position
};

struct UnkNavPathSegment_l9bd560 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 vf_0x28();  // the number of points
    virtual UnkNavPathPoint_l9bd560* vf_0x30(u32 index);
    u8 pad_0x8[0x48];
};
static_assert(sizeof(UnkNavPathSegment_l9bd560) == 0x50, "layout");

// A member object of the path, so its slot is called through its vtable (a virtual call on a member
// object would be devirtualized).
struct UnkNavPathSegments_l9bd560 {
    void** vtable;
    u64 field_0x8;
    UnkNavPathSegment_l9bd560* data;  // 0x10

    // Slot 0x28: the number of segments.
    u32 Count() { return reinterpret_cast<u32 (*)(UnkNavPathSegments_l9bd560*)>(vtable[0x28 / 8])(this); }
};

struct UnkNavPath_l9bd560 {
    u8 pad_0x0[0x30];
    UnkNavPathSegments_l9bd560 segments;  // 0x30
};
static_assert(__builtin_offsetof(UnkNavPath_l9bd560, segments.data) == 0x40, "layout");

struct UnkNavItem_l9bd560 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkNavStage_l9bd560 {
    void* vtable;
    UnkNavItem_l9bd560* item;  // 0x8
};

// The direction as returned in memory.
struct UnkNavDirection_l9bd560 {
    f32 x;
    f32 y;

    UnkNavDirection_l9bd560(const CVector2D& value) : x(value.x), y(value.y) {}
    UnkNavDirection_l9bd560(const UnkNavDirection_l9bd560& other) : x(other.x), y(other.y) {}
};

UnkNavDirection_l9bd560 fn_71009bd560(UnkNavStage_l9bd560* self) {
    CVector2D dir;
    dir.x = g_unk_7101647780.x;
    dir.y = g_unk_7101647780.y;
    bool found = false;
    if (self->item && self->item->actor) {
        if (auto* path = reinterpret_cast<UnkNavPath_l9bd560*>(fn_7100382150(self->item->actor))) {
            if (u32 count = path->segments.Count()) {
                u32 last = count - 1;
                if (u32 points = path->segments.data[last].vf_0x28()) {
                    const CVector3D* first = path->segments.data[0].vf_0x30(0)->vf_0x18();
                    f32 x = first->x;
                    f32 y = first->y;
                    f32 z = first->z;
                    const CVector3D* end = path->segments.data[last].vf_0x30(points - 1)->vf_0x18();
                    CVector3D d;
                    d.x = end->x - x;
                    d.y = end->y - y;
                    d.z = end->z - z;
                    fn_710007b444(&dir, reinterpret_cast<const UnkVec3*>(&d));
                    found = true;
                }
            }
        }
    }
    return found ? dir : g_unk_7101647780;
}
