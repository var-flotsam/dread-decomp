// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): extends the swarm's path from its last
// point along `dir`, sweeping a sphere (fn_71006580e0) and sliding along what it hits, until the distance is
// used up, the path is full, the sweep is stuck or the hit faces back. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/SLogicPathNode.h"
#include "types.h"

using base::math::CVector3D;

extern "C" f32 sqrtf(f32);

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_Swarm_ovb0084.cpp defines them.
struct UnkRedenkiPathPoint_ovb0084 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* Position();  // 0x18
};

struct UnkRedenkiPath_ovb0084 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Count();  // 0x28
    virtual UnkRedenkiPathPoint_ovb0084* Point(u32 i);  // 0x30
};

// What a sweep hits: the point, the surface normal and the sweep's own normal there.
struct UnkSweepHit_l656f3c {
    CVector3D position;  // 0x0
    CVector3D normal;    // 0xc
    CVector3D facing;    // 0x18
    f32 field_0x24;
    f32 field_0x28;
};

class UnkDaivoSwarm_b0012;
struct UnkSwarmPath_b0012;
struct UnkZeroVec_b0012;
struct UnkFlockingSwarm_fsc;
struct UnkSwarmList_fsc;

// Sweeps a sphere of `radius` from `from` to `to`; false when nothing is hit.
bool fn_71006580e0(UnkDaivoSwarm_b0012* self, const CVector3D* from, const CVector3D* to, UnkSweepHit_l656f3c* hit,
                   f32 radius, f32 limit);
// As src/game/CFlockingSwarmControllerComponent/CFlockingSwarmControllerComponent_b39.cpp defines it.
void CFlockingSwarmControllerComponent_ComputePathPointValueIfUnset(UnkFlockingSwarm_fsc* self, u32 index, UnkSwarmList_fsc* list);

static inline f32 Dot(const CVector3D& a, const CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline void AddPoint(UnkDaivoSwarm_b0012* self, UnkRedenkiPath_ovb0084* path, const CVector3D* point) {
    fn_71010592b0(reinterpret_cast<UnkMinimapPath*>(path), point);
    CFlockingSwarmControllerComponent_ComputePathPointValueIfUnset(reinterpret_cast<UnkFlockingSwarm_fsc*>(self), path->Count() - 1,
                  reinterpret_cast<UnkSwarmList_fsc*>(path));
}

// `speed` is the distance left (consumed) and `offset` receives the sweep's offset; the declaration
// in CDaivoSwarmControllerComponent_Swarm_ovb0012.cpp marks both const.
void fn_7100656f3c(UnkDaivoSwarm_b0012* self, UnkSwarmPath_b0012* swarm_path, const UnkVec3Result* dir_in,
                   const f32* speed, bool* out, const UnkZeroVec_b0012* offset, f32 radius, f32 limit) {
    auto* path = reinterpret_cast<UnkRedenkiPath_ovb0084*>(swarm_path);
    f32* remaining = const_cast<f32*>(speed);
    CVector3D* sweep_offset = reinterpret_cast<CVector3D*>(const_cast<UnkZeroVec_b0012*>(offset));

    CVector3D from = *path->Point(path->Count() - 1)->Position();
    f32 length = sqrtf(dir_in->x * dir_in->x + dir_in->y * dir_in->y + dir_in->z * dir_in->z);
    if (!(*remaining > 0.0f))
        return;

    f32 scale = 1.0f / (length + 0.0001f);
    CVector3D dir = {dir_in->x * scale, dir_in->y * scale, dir_in->z * scale};
    CVector3D current = from;
    s32 stuck = 0;
    bool can_retry = true;
    while (*remaining > 0.0f) {
        CVector3D target = {current.x + dir.x * *remaining, current.y + dir.y * *remaining,
                            current.z + dir.z * *remaining};
        UnkSweepHit_l656f3c hit = {};
        if (!fn_71006580e0(self, &from, &target, &hit, radius, limit)) {
            AddPoint(self, path, &target);
            *remaining = 0.0f;
            *out = false;
            return;
        }
        *sweep_offset = {dir.x * radius, dir.y * radius, dir.z * radius};
        target = {hit.position.x - sweep_offset->x, hit.position.y - sweep_offset->y,
                  hit.position.z - sweep_offset->z};
        CVector3D step = {target.x - current.x, target.y - current.y, target.z - current.z};
        f32 distance = sqrtf(Dot(step, step));
        if (distance - 0.001f <= 0.0f && distance + 0.001f >= 0.0f)
            ++stuck;
        else
            stuck = 0;
        if (Dot(dir, step) >= 0.0f) {
            AddPoint(self, path, &target);
            *remaining -= distance;
            from = target;
            current = from;
            can_retry = true;
        } else {
            // Pushed back: allowed once in a row.
            if (!can_retry) {
                *out = true;
                return;
            }
            can_retry = false;
        }
        // Facing almost straight back into the surface: give up.
        if (Dot(hit.facing, hit.normal) < -0.9659258f) {
            *out = true;
            return;
        }
        // Slide along the surface, sideways to the facing in the horizontal plane.
        CVector3D side = {hit.facing.y, -hit.facing.x, 0.0f};
        if (Dot(hit.normal, side) < 0.0f)
            dir = {-side.x, -side.y, -side.z};
        else
            dir = side;
        if (stuck > 10 || path->Count() > 50)
            return;
    }
}
