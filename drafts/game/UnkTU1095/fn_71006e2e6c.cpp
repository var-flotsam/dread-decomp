// behaviortree::CXParasiteGoToPointAction (unit 1095): its update. Steers the X parasite owner toward the
// current target point with a speed-limited, turn-limited velocity kept in the AI component, and returns
// 2 (running), 5 (arrived) or 6 (failed). Names and layouts are placeholders.
#include <math.h>

#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU83.h"
#include "game/UnkVec3_cs.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

UnkVec3f RotateVec3AboutZ(const UnkVec3f& v, f32 angle);
bool fn_71006e2b9c(void* self);

struct UnkXParasiteAI_l6e2e6c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x146f0 - 0x8];
    UnkVec3f velocity;  // 0x146f0
};

struct UnkGoToPoint_l6e2e6c {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x20];
    bool field_0x40;
    u8 pad_0x41[3];
    UnkVec3f target;  // 0x44
    UnkVec3f dir;     // 0x50
    f32 maxSpeed;     // 0x5c
    f32 accel;        // 0x60
    f32 accelNear;    // 0x64
    f32 radius;       // 0x68
    f32 accelBack;    // 0x6c
    bool turning;     // 0x70
    u8 pad_0x71[3];
    f32 maxTurn;      // 0x74 (degrees)
    UnkVec3_cs* path; // 0x78
    u32 pathCount;    // 0x80
};

static inline UnkVec3f Pos(UnkActor* a) {
    return UnkVec3f(a->field_0x44, a->field_0x54, a->field_0x64);
}

static inline f32 Len(f32 x, f32 y, f32 z) {
    return sqrtf(x * x + y * y + z * z);
}

u32 fn_71006e2e6c(UnkGoToPoint_l6e2e6c* self, f32 dt) {
    UnkXParasiteAI_l6e2e6c* ai = static_cast<UnkXParasiteAI_l6e2e6c*>(
        static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240));
    if (!ai)
        return 6;
    if (!fn_710009d50c(ai->vf_0x0(), fn_71006e5f28()))
        return 6;
    if (self->pathCount == 0 && !fn_71006e2b9c(self))
        return 6;

    UnkActor* owner = self->owner;
    UnkVec3f diff = self->target - Pos(owner);
    f32 dx = diff.x, dy = diff.y, dz = diff.z;
    f32 dist = Len(dx, dy, dz);
    f32 ratio = dist / self->radius;
    f32 t = !(ratio <= 0.0f) ? (!(ratio >= 1.0f) ? ratio : 1.0f) : 0.0f;
    f32 floor = self->pathCount == 0 ? 0.1f : 1.0f;
    f32 blend = 1.0f;
    if (!(t >= 1.0f)) {
        if (t < 0.0f)
            blend = floor;
        else
            blend = t + floor * (1.0f - t);
    }

    f32 inv = 1.0f / (Len(dx, dy, dz) + 0.0001f);
    f32 sx = dx * inv, sy = dy * inv, sz = dz * inv;
    f32 speed = self->maxSpeed;
    f32 wy = blend * (speed * sy);
    f32 wx = blend * (speed * sx);
    f32 wz = blend * (speed * sz);

    UnkVec3f* vel = &ai->velocity;
    f32 cx = vel->x, cy = vel->y, cz = vel->z;
    f32 ex = wx - cx, ey = wy - cy, ez = wz - cz;
    f32 nx = 0, ny = 0, nz = 0;
    f32 e2 = ex * ex + ey * ey + ez * ez;
    if (!(e2 < 100.0f)) {
        f32 k = 1.0f / (sqrtf(e2) + 0.0001f);
        nx = ex * k;
        ny = ey * k;
        nz = ez * k;
    }
    f32 ik = 1.0f / (Len(cx, cy, cz) + 0.0001f);
    f32 ux = cx * ik, uy = cy * ik, uz = cz * ik;

    f32 accel = self->accel;
    bool skipTurn = false;
    if (self->field_0x40 && blend < 1.0f && self->pathCount == 0) {
        accel = self->accelNear;
        self->turning = false;
        skipTurn = true;
    } else {
        if (!(wx * ux + wy * uy + wz * uz > 0.0f))
            accel = self->accelBack;
        skipTurn = !self->turning;
    }
    if (!skipTurn) {
        f32 d = nx * ux + ny * uy + nz * uz;
        f32 c = !(d >= 1.0f) ? d : 1.0f;
        f32 angle = acosf(d > -1.0f ? c : -1.0f);
        f32 limit = (f32)((f64)self->maxTurn * 3.14159265358979323846 / 180.0);
        if (angle > limit) {
            UnkVec3f from(ux, uy, uz);
            f32 cxp = -uy - uz * -0.0f;
            f32 cyp = uz * -0.0f + ux;
            f32 czp = uy * 0.0f - ux * 0.0f;
            f32 side = nx * cxp + ny * cyp + nz * czp;
            UnkVec3f r = RotateVec3AboutZ(from, side < 0.0f ? -limit : limit);
            nx = r.x;
            ny = r.y;
            nz = 0.0f;
        }
    }

    f32 vx = cx + accel * nx * dt;
    f32 vy = cy + accel * ny * dt;
    f32 vz = cz + accel * nz * dt;
    f32 v2 = vx * vx + vy * vy + vz * vz;
    if (sqrtf(v2) > self->maxSpeed) {
        f32 k = 1.0f / (sqrtf(v2) + 0.0001f);
        vx = self->maxSpeed * (vx * k);
        vy = self->maxSpeed * (vy * k);
        vz = self->maxSpeed * (vz * k);
    }
    vel->x = vx;
    vel->y = vy;
    vel->z = vz;

    UnkActor* actor = self->owner;
    UnkVec3f pos(vx * dt + actor->field_0x44, vy * dt + actor->field_0x54, vz * dt + actor->field_0x64);
    actor->vf_0xd0(&pos, true);

    actor = self->owner;
    UnkVec3f rem = self->target - Pos(actor);
    f32 rx = rem.x, ry = rem.y, rz = rem.z;
    if (rx * rx + ry * ry + rz * rz > 400.0f && rx * self->dir.x + ry * self->dir.y + rz * self->dir.z >= 0.0f)
        return 2;

    if (self->pathCount != 0) {
        self->target = *reinterpret_cast<UnkVec3f*>(fn_71000cc374(&self->path, 0));
        if (self->pathCount != 1) {
            for (u32 i = 0; i < self->pathCount - 1; ++i)
                self->path[i] = self->path[i + 1];
        }
        self->pathCount = self->pathCount - 1;
        actor = self->owner;
        self->dir = self->target - Pos(actor);
        return 2;
    }
    if (self->field_0x40) {
        vel->x = 0;
        vel->y = 0;
        vel->z = 0;
        actor = self->owner;
        actor->field_0x44 = self->target.x;
        actor->field_0x54 = self->target.y;
        actor->field_0x64 = self->target.z;
        *reinterpret_cast<u8*>(reinterpret_cast<u8*>(actor) + 0x3b0) = 1;
    }
    return 5;
}
