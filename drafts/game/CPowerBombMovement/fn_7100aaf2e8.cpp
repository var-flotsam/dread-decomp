// CPowerBombMovement unit (tools/tu_map.py, unit 1612): where the power bomb is placed and how it is turned.
// The ground's normal under the actor is found (along its velocity, from its ground state, or by a short
// horizontal then vertical cast), the bomb's base placement runs, the bomb is pushed back along the normal
// and tilted to it. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

// As src/game/CBombMovement/CBombMovement_Hits_ovb0078.cpp defines it.
struct UnkBombMovement_ovb0078;
struct UnkBombActor_ovb0078;
void fn_7100aabf4c(UnkBombMovement_ovb0078* self, UnkBombActor_ovb0078* actor, UnkVec3f* pos,
                   UnkVec3f* offset);

// A reference-counted object of the collision queries: released through its vf_0x10.
struct UnkRayRefTarget_laaf2e8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
};

struct UnkRayRef_laaf2e8 {
    UnkRayRefTarget_laaf2e8* ptr;

    UnkRayRef_laaf2e8() : ptr(nullptr) {}
    ~UnkRayRef_laaf2e8() {
        UnkRayRefTarget_laaf2e8* p = ptr;
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

// What a collision cast reports.
struct UnkRayHit_laaf2e8 {
    UnkRayRef_laaf2e8 collider;
    UnkRayRef_laaf2e8 shape;
    s32 index;            // 0x10
    f32 distance;         // 0x14
    CVector3D point;      // 0x18
    f32 field_0x24;
    f32 field_0x28;
    CVector3D normal;     // 0x2c
    u32 field_0x38;
    u8 flags;             // 0x3c: 0x10 when the normal is set
    u8 pad_0x3d[3];
    u32 field_0x40;

    UnkRayHit_laaf2e8()
        : index(-1), distance(0.0f), point{0.0f, 0.0f, 0.0f}, field_0x24(0.0f), field_0x28(0.0f),
          normal{0.0f, 0.0f, 0.0f}, field_0x38(0), flags(0), pad_0x3d{}, field_0x40(0) {}
};
static_assert(__builtin_offsetof(UnkRayHit_laaf2e8, flags) == 0x3c, "layout");

// Casts from `from` to `to` against the tracker's collision of layer mask `mask`.
bool fn_7100d5b268(void* tracker, u32 mask, const UnkVec3f* from, const UnkVec3f* to, UnkRayHit_laaf2e8* hit,
                   bool a, bool b);

struct UnkPowerBombTuning_laaf2e8 {
    u8 pad_0x0[0x30];
    f32 reach;            // 0x30
    u8 pad_0x34[0x2];
    bool push_back;       // 0x36
    bool tilt;            // 0x37
};
extern UnkPowerBombTuning_laaf2e8 g_unk_7172753dc0;

struct UnkGroundSource_laaf2e8 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkGround_laaf2e8 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
    u8 pad_0xfa[0x128 - 0xfa];
    UnkGroundSource_laaf2e8* field_0x128;
    u8 pad_0x130[0x154 - 0x130];
    CVector3D normal;  // 0x154
};

struct UnkMotion_laaf2e8 {
    u8 pad_0x0[0x9ec];
    f32 velocity_x;  // 0x9ec
    f32 velocity_y;  // 0x9f0
};

struct UnkPowerBombActor_laaf2e8 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1d8 - 0x94];
    UnkMotion_laaf2e8* motion;  // 0x1d8
    u8 pad_0x1e0[0x210 - 0x1e0];
    UnkGround_laaf2e8* ground;  // 0x210
    u8 pad_0x218[0x37c - 0x218];
    f32 radius;  // 0x37c
};
static_assert(__builtin_offsetof(UnkPowerBombActor_laaf2e8, radius) == 0x37c, "layout");

struct UnkPowerBombMovement_laaf2e8;

void fn_7100aaf2e8(UnkPowerBombMovement_laaf2e8* self, UnkPowerBombActor_laaf2e8* actor, UnkVec3f* pos,
                   UnkVec3f* rotation) {
    UnkVec3f origin = actor->xform.GetTranslation();
    *pos = origin;
    UnkGround_laaf2e8* ground = actor ? actor->ground : nullptr;
    UnkGameManagerSub_0x608* world = g_unk_7101d47310->field_0x608;
    void* tracker = world ? world->field_0x820 : nullptr;
    f32 reach = g_unk_7172753dc0.reach;
    UnkMotion_laaf2e8* motion = actor ? actor->motion : nullptr;

    CVector3D normal;
    UnkRayHit_laaf2e8 hit;
    bool found = false;
    // Along the actor's velocity first.
    if (motion && (motion->velocity_x != 0.0f || motion->velocity_y != 0.0f)) {
        CVector3D step = {motion->velocity_x, motion->velocity_y, 0.0f};
        CVector3D scaled = {reach * step.x, reach * step.y, reach * step.z};
        UnkVec3f to(scaled.x + origin.x, scaled.y + origin.y, scaled.z + origin.z);
        if (fn_7100d5b268(tracker, 9, pos, &to, &hit, true, true) && (hit.flags & 0x10)) {
            normal = hit.normal;
            found = true;
        }
    }
    if (!found) {
        if (ground && !ground->field_0xf9 && ground->field_0x128 && ground->field_0x128->field_0x10) {
            normal = ground->normal;
            found = true;
        } else {
            // A short cast across, then one along, the actor's position.
            static const CVector3D s_across asm("g_unk_7172753e48") = {reach, 0.0f, 0.0f};
            static const CVector3D s_along asm("g_unk_7172753e54") = {0.0f, reach, 0.0f};
            bool cast;
            {
                UnkVec3f from(pos->x - s_across.x, pos->y - s_across.y, pos->z - s_across.z);
                UnkVec3f to(pos->x + s_across.x, pos->y + s_across.y, pos->z + s_across.z);
                cast = fn_7100d5b268(tracker, 9, &from, &to, &hit, true, true);
            }
            if (!cast) {
                UnkVec3f from(pos->x - s_along.x, pos->y - s_along.y, pos->z - s_along.z);
                UnkVec3f to(pos->x + s_along.x, pos->y + s_along.y, pos->z + s_along.z);
                cast = fn_7100d5b268(tracker, 9, &from, &to, &hit, true, true);
            }
            if (cast && (hit.flags & 0x10)) {
                normal = hit.normal;
                found = true;
            }
        }
    }

    fn_7100aabf4c(reinterpret_cast<UnkBombMovement_ovb0078*>(self), reinterpret_cast<UnkBombActor_ovb0078*>(actor),
                  pos, rotation);
    if (!found) {
        normal = {0.0f, 1.0f, 0.0f};
    } else if (g_unk_7172753dc0.push_back) {
        f32 back = actor->radius * 0.33f;
        pos->x = pos->x - normal.x * back;
        pos->y = pos->y - normal.y * back;
        pos->z = pos->z - normal.z * back;
    }

    if (g_unk_7172753dc0.tilt) {
        // The angle between the normal and up, signed by the normal's side.
        f32 inv = 1.0f / (sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z) + 0.0001f);
        f32 nx = normal.x * inv;
        f32 ny = normal.y * inv;
        f32 nz = normal.z * inv;
        f32 cosine = nx * 0.0f + ny * 0.9999f;
        cosine = nz * 0.0f + cosine;
        cosine = !(cosine >= 1.0f) ? cosine : 1.0f;
        cosine = !(cosine < -1.0f) ? cosine : -1.0f;
        f32 angle = acosf(cosine);
        s32 side = (normal.x > 0.0f) - (normal.x < 0.0f);
        *rotation = UnkVec3f(0.0f, 0.0f, -(angle * side));
    } else {
        *rotation = UnkVec3f(0.0f, 0.0f, 0.0f);
    }
}
