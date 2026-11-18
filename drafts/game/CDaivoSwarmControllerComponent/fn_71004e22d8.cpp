// CDaivoSwarmControllerComponent's unit (tools/tu_map.py unit 768): the arrival check. When the swarm's goal
// (fn_710064fef8) is within 200 of the path's last point, the goal is kept at +0x478, the swarm is sent along
// the path towards fn_7100739c64's target (vf_0x2f8) and the state becomes 2. Names and layouts are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "types.h"

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkRedenkiSwarmController;
bool fn_710065244c(UnkRedenkiSwarmController* self);

extern bool g_unk_71726fc5b0;
extern f32 g_unk_7101a5aef8;

struct UnkDaivoPos_l4e22d8 {
    f32 x, y, z;
};

// A point of the path: polymorphic, 0x20 bytes; slot 3 gives its position.
struct UnkDaivoPathPoint_l4e22d8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkDaivoPos_l4e22d8* GetPos();
    u8 pad_0x8[0x18];
};

struct UnkDaivoPath_l4e22d8 {
    void* vtable;
    UnkDaivoPathPoint_l4e22d8* points;  // 0x8
    u32 count;  // 0x10
};

struct UnkDaivoSwarm_l4e22d8 {
    void** vtable;
    u8 pad_0x8[0x2c8 - 0x8];
    UnkDaivoPath_l4e22d8 path;  // 0x2c8
    u8 pad_0x2e0[0x478 - 0x2e0];
    f32 goal[3];  // 0x478

    void vf_0x2e8(u32 state, bool arg) {
        reinterpret_cast<void (*)(UnkDaivoSwarm_l4e22d8*, u32, bool)>(vtable[0x2e8 / 8])(this, state, arg);
    }
    void vf_0x2f8(const UnkVec3Result* goal, UnkParent_71007455dc* target, bool a, UnkDaivoPath_l4e22d8* path,
                  bool b, f32 speed) {
        reinterpret_cast<void (*)(UnkDaivoSwarm_l4e22d8*, const UnkVec3Result*, UnkParent_71007455dc*, bool,
                                  UnkDaivoPath_l4e22d8*, bool, f32)>(vtable[0x2f8 / 8])(this, goal, target, a, path,
                                                                                         b, speed);
    }
};

void fn_71004e22d8(UnkDaivoSwarm_l4e22d8* self) {
    UnkVec3Result goal = fn_710064fef8(reinterpret_cast<UnkSwarmHold_b66a4*>(self));
    const UnkDaivoPos_l4e22d8* end = self->path.points[self->path.count - 1].GetPos();
    f32 dx = goal.x - end->x;
    f32 dy = goal.y - end->y;
    f32 dz = goal.z - end->z;
    if (dx * dx + dy * dy + dz * dz > 40000.0f)
        return;
    self->goal[0] = goal.x;
    self->goal[1] = goal.y;
    self->goal[2] = goal.z;
    UnkParent_71007455dc* target = fn_7100739c64(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self));
    f32 speed = g_unk_71726fc5b0 ? g_unk_7101a5aef8 : -1.0f;
    bool flag = fn_710065244c(reinterpret_cast<UnkRedenkiSwarmController*>(self));
    self->vf_0x2f8(&goal, target, false, &self->path, flag, speed);
    self->vf_0x2e8(2, false);
}
