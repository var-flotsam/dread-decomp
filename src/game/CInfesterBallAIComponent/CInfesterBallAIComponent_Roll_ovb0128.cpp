// CInfesterBallAIComponent's unit (tools/tu_map.py, unit 898): starting to roll along the path (fn_71005cffd0)
// or towards a point (fn_71005d1cf4), and whether the point ahead of the ball lies in the central unit's shape
// (fn_71005d09a8). Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "../CFulmiteBellyMineAIComponent/UnkCentralShapeTest_A3_b70.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkInfesterPathVertex_ovb0128 {
    CVector2D pos;
    f32 field_0x8;
};

// The path, a 2D polygon with the layout of src/base/spatial/UnkPolygon2D.cpp.
struct UnkInfesterPath_ovb0128 {
    UnkInfesterPathVertex_ovb0128* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;  // 0x2c

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }
    // Builds edge `i` (from its vertex to the next one, wrapping to the first) in `seg`.
    void Edge(UnkSegment2D* seg, u32 i) const {
        new (seg) UnkSegment2D(vertices[i].pos, (count - 1 == i ? vertices : &vertices[i + 1])->pos);
    }
};

struct UnkInfesterMover_ovb0128 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool active;  // 0xf9
    u8 pad_0xfa[0x11c - 0xfa];
    f32 normal[3];  // 0x11c
    u8 pad_0x128[0x1d8 - 0x128];
    f32 velocity[3];  // 0x1d8

    void vf_0x260() { reinterpret_cast<void (*)(UnkInfesterMover_ovb0128*)>(vtable[0x260 / 8])(this); }
};

struct UnkInfesterSpeed_ovb0128 {
    void** vtable;
    u8 pad_0x8[0x534];
    f32 climb_factor;  // 0x53c

    f32 vf_0xb0(void* owner) {
        return reinterpret_cast<f32 (*)(UnkInfesterSpeed_ovb0128*, void*)>(vtable[0xb0 / 8])(this, owner);
    }
};

// The rolling state at +0x146c8.
struct UnkInfesterRoll_ovb0128 {
    UnkInfesterMover_ovb0128* mover;  // 0x0
    UnkInfesterSpeed_ovb0128* speed;  // 0x8
    u8 pad_0x10[0x30];
    u32 state;  // 0x40
    u8 pad_0x44[1];
    bool backwards;  // 0x45
    u8 pad_0x46[4];
    bool rolling;  // 0x4a
    u8 pad_0x4b[1];
    bool has_path;  // 0x4c
    u8 pad_0x4d[3];
    CVector2D target;  // 0x50
    u8 pad_0x58[0x44];
    f32 velocity[3];  // 0x9c
    u8 pad_0xa8[0x8];
    f32 direction;  // 0xb0
};

struct UnkInfesterBallAI_ovb0128 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkInfesterPath_ovb0128 path;  // 0x68
    u8 pad_0x98[0x146c8 - 0x98];
    UnkInfesterRoll_ovb0128 roll;  // 0x146c8
};
static_assert(__builtin_offsetof(UnkInfesterBallAI_ovb0128, roll) == 0x146c8, "layout");

// Starts rolling: towards the path's first point, or its last when going backwards; the mover is
// activated and its velocity taken, the x part set to the speed's magnitude signed by the direction.
void fn_71005cffd0(UnkInfesterBallAI_ovb0128* self) {
    UnkInfesterRoll_ovb0128& roll = self->roll;
    if (roll.state != 0)
        roll.state = 0;
    roll.has_path = false;
    u32 edges = self->path.EdgeCount();
    if (edges != 0) {
        roll.has_path = true;
        UnkSegment2D seg;
        if (roll.backwards) {
            self->path.Edge(&seg, edges - 1);
            roll.target.x = seg.end.x;
            roll.target.y = seg.end.y;
        } else {
            self->path.Edge(&seg, 0);
            roll.target.x = seg.start.x;
            roll.target.y = seg.start.y;
        }
    }
    UnkInfesterMover_ovb0128* mover = roll.mover;
    if (mover->active != true) {
        mover->active = true;
        mover->vf_0x260();
    }
    roll.velocity[0] = roll.mover->velocity[0];
    roll.velocity[1] = roll.mover->velocity[1];
    roll.velocity[2] = roll.mover->velocity[2];
    f32 speed = roll.speed->vf_0xb0(self->owner);
    f32 d = roll.direction;
    f32 x = __builtin_fabsf(speed) * (f32)((d > 0.0f) - (d < 0.0f));
    roll.rolling = true;
    roll.velocity[0] = x;
}

// Sends the event with the given hash (in this unit).
void fn_71005d1be4(UnkInfesterBallAI_ovb0128* self, u64 hash);

struct UnkInfesterPoint_ovb0128 {
    u8 pad_0x0[0x14];
    base::math::CVector3D pos;  // 0x14
};

// Starts rolling towards `point` (projected to 2D): the speed along x signed by the direction, the
// climbing speed along y, both pushed by half the mover's normalised normal (y flipped); then the mover is
// activated and the event 0x94c9351292223983 sent.
void fn_71005d1cf4(UnkInfesterBallAI_ovb0128* self, UnkInfesterPoint_ovb0128* point) {
    UnkInfesterRoll_ovb0128& roll = self->roll;
    roll.has_path = true;
    CVector2D target;
    fn_710007b430(&target, &point->pos);
    roll.target.x = target.x;
    roll.target.y = target.y;
    UnkInfesterMover_ovb0128* mover = roll.mover;
    f32 x = mover->normal[0];
    f32 y = mover->normal[1];
    f32 z = mover->normal[2];
    f32 inv = 1.0f / (sqrtf(x * x + y * y + z * z) + 1e-4f);
    f32 ux = x * inv;
    f32 uy = y * inv;
    f32 uz = z * inv;
    f32 nx = ux * 0.5f;
    f32 ny = uy * -0.5f;
    f32 nz = uz * 0.5f;
    f32 speed = roll.speed->vf_0xb0(self->owner);
    f32 d = roll.direction;
    roll.velocity[0] = __builtin_fabsf(speed) * (f32)((d > 0.0f) - (d < 0.0f));
    f32 climb = roll.speed->vf_0xb0(self->owner);
    f32 vy = __builtin_fabsf(climb) * roll.speed->climb_factor;
    f32 vx = roll.velocity[0];
    f32 mx = nx * vx;
    f32 my = ny * vy;
    f32 vz = roll.velocity[2];
    f32 mz = nz * vz;
    roll.velocity[0] = vx + mx;
    roll.velocity[1] = vy + my;
    roll.velocity[2] = vz + mz;
    mover = roll.mover;
    if (mover->active != true) {
        mover->active = true;
        mover->vf_0x260();
    }
    roll.rolling = true;
    fn_71005d1be4(self, 0x94c9351292223983);
}

// Whether the point ahead of the owner (its position plus the direction at +0x14778 times twice the
// owner's +0x37c) lies in the shape of the central unit's tracked component.
struct UnkInfesterBallOwner_ovb0128 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x37c - 0x68];
    f32 radius;  // 0x37c
};

struct UnkInfesterBallShapeAI_ovb0128 {
    u8 pad_0x0[0x30];
    UnkInfesterBallOwner_ovb0128* owner;  // 0x30
    u8 pad_0x38[0x14778 - 0x38];
    base::math::CVector2D direction;  // 0x14778
};

bool fn_71005d09a8(UnkInfesterBallShapeAI_ovb0128* self) {
    return CentralShapeTest_A3_b70([self] {
        UnkInfesterBallOwner_ovb0128* owner = self->owner;
        f32 px = owner->x;
        f32 py = owner->y;
        f32 pz = owner->z;
        f32 dx = self->direction.x;
        f32 dy = self->direction.y;
        f32 dz = 0.0f;
        f32 k = owner->radius + owner->radius;
        f32 ox = dx * k;
        f32 oy = dy * k;
        f32 oz = dz * k;
        base::math::CVector3D r;
        r.x = px + ox;
        r.y = py + oy;
        r.z = pz + oz;
        return r;
    });
}
