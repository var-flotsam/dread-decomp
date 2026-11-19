// CChozoRobotSoldierAIComponent unit: the direction the robot soldier should turn to face, from the first
// path point off its own x (or else its target's direction); false when it already faces that way.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkPathPoint_l474760 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const f32* vf_0x18();  // the point's position
};

struct UnkPathPoints_l474760 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 vf_0x28();  // the number of points
};

struct UnkPath_l474760 {
    u8 pad_0x0[0x19c0];
    bool active;  // 0x19c0
    u8 pad_0x19c1[0x17];
    u8 points[0xc];  // 0x19d8, an UnkPathPoints_l474760
    u8 slots[1][0x120];  // 0x19e4, one UnkPathPoint_l474760 each

    UnkPathPoints_l474760* Points() { return reinterpret_cast<UnkPathPoints_l474760*>(points); }
    UnkPathPoint_l474760* Slot(u64 i) { return reinterpret_cast<UnkPathPoint_l474760*>(slots[i]); }
};
static_assert(__builtin_offsetof(UnkPath_l474760, slots) == 0x19e4, "layout");

struct UnkTargetInfo_l474760 {
    u8 pad_0x0[0x70];
    CVector3D direction;  // 0x70
};

struct UnkActor_l474760 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x178];
    UnkTargetInfo_l474760* target;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkPath_l474760* path;  // 0x240
};
static_assert(__builtin_offsetof(UnkActor_l474760, path) == 0x240, "layout");

struct UnkSoldierAI_l474760 {
    u8 pad_0x0[0x30];
    UnkActor_l474760* actor;  // 0x30
};

__attribute__((visibility("hidden"))) extern CVector3D g_unk_7101649820;
__attribute__((visibility("hidden"))) extern CVector3D g_unk_710164982c;

static inline f32 Sign_l474760(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
bool fn_7100474760(UnkSoldierAI_l474760* self, CVector3D* out) {
    const UnkMatrix44& world = fn_710002614c(&self->actor->transform);
    f32 x = world.m[2];
    f32 y = world.m[6];
    f32 z = world.m[10];
    out->x = x;
    out->y = y;
    out->z = z;

    UnkPath_l474760* path = self->actor->path;
    if (path->active) {
        for (u64 i = 0; i < path->Points()->vf_0x28(); i++) {
            f32 point_x = *path->Slot(i)->vf_0x18();
            f32 own_x = self->actor->transform.m[3];
            f32 low = point_x - 0.001f;
            f32 high = point_x + 0.001f;
            if (!(high >= own_x && low <= own_x)) {
                f32 side = Sign_l474760(point_x - own_x);
                if (side == Sign_l474760(fn_710002614c(&self->actor->transform).m[2]))
                    return false;
                *out = side > 0.0f ? g_unk_7101649820 : g_unk_710164982c;
                return true;
            }
        }
    }

    if (Sign_l474760(self->actor->target->direction.x) ==
        Sign_l474760(fn_710002614c(&self->actor->transform).m[2]))
        return false;
    const CVector3D& direction = self->actor->target->direction;
    out->x = direction.x;
    out->y = direction.y;
    out->z = direction.z;
    return true;
}
