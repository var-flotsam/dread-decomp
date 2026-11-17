// CDaivoSwarmControllerComponent's unit (tools/tu_map.py unit 768): moves the path point at the step's end
// node back along the step by the step's margin, drops the points after it, sends the swarm along the step
// (fn_7100656f3c) and extends the path to `target` (fn_7100658310). Names and layouts are placeholders.
#include "base/math/CVector.h"
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

// As src/game/CDaivoSwarmControllerComponent/CDaivoSwarmControllerComponent_Swarm_ovb0012.cpp defines it.
struct UnkZeroVec_b0012 {
    f32 x, y, z;
    UnkZeroVec_b0012() : x(0.0f), y(0.0f), z(0.0f) {}
};

class UnkDaivoSwarm_b0012;
struct UnkSwarmPath_b0012;
struct UnkRedenkiSwarm_ovb0084;
struct UnkRedenkiPath_ovb0084;

// As src/game/CDaivoSwarmControllerComponent/CDaivoSwarmControllerComponent_Swarm_ovb0012.cpp declares it.
void fn_7100656f3c(UnkDaivoSwarm_b0012* self, UnkSwarmPath_b0012* path, const UnkVec3Result* dir, const f32* speed,
                   bool* out, const UnkZeroVec_b0012* offset, f32 radius, f32 limit);
// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_Swarm_ovb0084.cpp defines it.
bool fn_7100658310(UnkRedenkiSwarm_ovb0084* self, UnkRedenkiPath_ovb0084* path, const CVector3D* target, bool flag);

struct UnkPoint3_l4e1d14 {
    f32 x, y, z;
};

// A node of the swarm's graph; its position comes from vf_0x18.
struct UnkSwarmNode_l4e1d14 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkPoint3_l4e1d14& Position();
};

// A node's key: the index at +0x4 is also the node's point on the path.
struct UnkNodeKey_l4e1d14 {
    u32 id;
    u32 index;
};

// One step of a route: from one node to the next, ending `margin` short of the second.
struct UnkSwarmStep_l4e1d14 {
    u8 pad_0x0[0x48];
    UnkNodeKey_l4e1d14 from;  // 0x48
    UnkNodeKey_l4e1d14 to;  // 0x50
    u8 pad_0x58[0x64 - 0x58];
    f32 margin;  // 0x64
};

// A point of the path (0x20 bytes).
struct UnkPathPoint_l4e1d14 {
    void* vtable;
    UnkPoint3_l4e1d14 position;  // 0x8
    f32 field_0x14;
    u8 pad_0x18[0x20 - 0x18];
};

struct UnkSwarmPath_l4e1d14 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Count();  // 0x28

    UnkPathPoint_l4e1d14* points;  // 0x8
};

struct UnkDaivoTuning_l4e1d14 {
    u8 pad_0x0[0x458];
    f32 radius;  // 0x458
};

#define UNK_VF8(n) virtual void vf_##n##_0(); virtual void vf_##n##_1(); virtual void vf_##n##_2(); \
    virtual void vf_##n##_3(); virtual void vf_##n##_4(); virtual void vf_##n##_5(); \
    virtual void vf_##n##_6(); virtual void vf_##n##_7();

class UnkDaivoSwarm_l4e1d14 {
public:
    // Slots 0x0-0x2b8 (88 entries).
    UNK_VF8(0) UNK_VF8(1) UNK_VF8(2) UNK_VF8(3) UNK_VF8(4) UNK_VF8(5) UNK_VF8(6) UNK_VF8(7)
    UNK_VF8(8) UNK_VF8(9) UNK_VF8(10)
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual void vf_0x2d0();
    virtual f32 vf_0x2d8();

    u8 pad_0x8[0x395 - 0x8];
    bool moving;  // 0x395
    u8 pad_0x396[0x53c - 0x396];
    u32 lastPoint;  // 0x53c
    u8 pad_0x540[0x570 - 0x540];
    UnkDaivoTuning_l4e1d14* tuning;  // 0x570
};

#undef UNK_VF8

UnkSwarmNode_l4e1d14* fn_7100749e44(UnkDaivoSwarm_l4e1d14* self, const UnkNodeKey_l4e1d14* key);
void fn_71010590c0(UnkPathPoint_l4e1d14* point, const UnkVec3Result* position);
void fn_7101059a58(UnkSwarmPath_l4e1d14* path, u32 index);

bool fn_71004e1d14(UnkDaivoSwarm_l4e1d14* self, const CVector3D* target, const UnkSwarmStep_l4e1d14* step,
                   void* unused, UnkSwarmPath_l4e1d14* path, bool flag, f32 speed) {
    UnkNodeKey_l4e1d14 from = step->from;
    UnkNodeKey_l4e1d14 to = step->to;
    UnkSwarmNode_l4e1d14* fromNode = fn_7100749e44(self, &from);
    UnkSwarmNode_l4e1d14* toNode = fn_7100749e44(self, &to);
    if (!fromNode || !toNode)
        return false;

    UnkPoint3_l4e1d14 end = toNode->Position();
    const UnkPoint3_l4e1d14& start = fromNode->Position();
    f32 dx = end.x - start.x;
    f32 dy = end.y - start.y;
    f32 dz = end.z - start.z;
    f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    u32 index = to.index;
    UnkVec3Result dir(dx, dy, dz);
    dir.x *= inv;
    dir.y = inv * dir.y;
    dir.z = inv * dir.z;
    f32 margin = step->margin;
    UnkVec3Result stop(end.x - dir.x * margin, end.y - dir.y * margin, end.z - dir.z * margin);

    UnkPathPoint_l4e1d14* point = &path->points[index];
    fn_71010590c0(point, &stop);
    point->field_0x14 = self->vf_0x2d8();
    self->moving = true;

    // The points after the moved one go.
    u32 count = path->Count();
    u32 next = to.index + 1;
    for (u32 i = next; i < count; i++)
        fn_7101059a58(path, next);

    if (speed > 0.0f) {
        f32 radius = self->tuning->radius;
        UnkZeroVec_b0012 offset;
        bool arrived;  // unset in the original: fn_7100656f3c writes it
        fn_7100656f3c(reinterpret_cast<UnkDaivoSwarm_b0012*>(self), reinterpret_cast<UnkSwarmPath_b0012*>(path), &dir,
                      &speed, &arrived, &offset, radius + radius, radius);
    }

    if (!fn_7100658310(reinterpret_cast<UnkRedenkiSwarm_ovb0084*>(self), reinterpret_cast<UnkRedenkiPath_ovb0084*>(path),
                       target, flag))
        return false;
    self->lastPoint = path->Count() - 1;
    return true;
}
