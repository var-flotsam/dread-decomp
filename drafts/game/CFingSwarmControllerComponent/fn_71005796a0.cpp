// CFingSwarmControllerComponent's unit (tools/tu_map.py unit 821): one group's flocking step. Each live
// member is steered back towards the centre or away from its close neighbours, its speed is capped and it
// moves; the group then gets its centre, its radius and its shape's radius. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU2560.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "types.h"

using base::reflection::CClass;

extern "C" f32 sqrtf(f32);

// As src/game/CDaivoSwarmControllerComponent/CDaivoSwarmControllerComponent_Swarm_ovb0012.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkFingKey {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkFingBillboard {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkFingKey* field_0xd0;
};

// One member of a group.
struct UnkFingMember {
    u8 pad_0x0[0x28];
    u32 field_0x28;
    f32 position[3];  // 0x2c
    f32 velocity[3];  // 0x38
    u8 pad_0x44[0x64 - 0x44];
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    u8 pad_0x70[0x180 - 0x70];
    u8 field_0x180;
};

class UnkFingShape {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
};

struct UnkFingShapeHolder {
    u8 pad_0x0[0x158];
    UnkFingShape* shape;  // 0x158
};

// A group of members (0x148 bytes).
struct UnkFingGroup {
    u8 pad_0x0[0x44];
    f32 radius;  // 0x44
    u8 pad_0x48[0xa0 - 0x48];
    f32 centre[3];  // 0xa0
    u8 pad_0xac[0xb0 - 0xac];
    UnkFingShapeHolder* field_0xb0;
    UnkFingMember** members;  // 0xb8
    u32 memberCount;  // 0xc0
    u8 pad_0xc4[0x148 - 0xc4];
};

struct UnkFingGroupList {
    u8 pad_0x0[0x10];
    UnkFingBillboard* field_0x10;
    u8 pad_0x18[0x50 - 0x18];
    UnkFingGroup* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    // The group at `i`, or null past the end.
    UnkFingGroup* At(s32 i) {
        if (i < count && groups != nullptr)
            return &groups[i];
        return nullptr;
    }
};

struct UnkFingData {
    u8 pad_0x0[0x10];
    UnkFingGroupList* groupList;  // 0x10
};

struct UnkFingSwarm {
    u8 pad_0x0[0xe8];
    UnkFingData* data;  // 0xe8
};

struct UnkParent_71007455dc;
struct UnkSphereShape_ovb0135;
void fn_7100745f10(UnkFingGroup* group, f32 radius);
extern u8 g_unk_7101649f58[];

static inline bool IsNear(f32 a, f32 b, f32 epsilon) {
    return a - epsilon <= b && a + epsilon >= b;
}

// Whether (x, y, z) is within 0.001 of `ref` on every axis.
static inline bool IsNear(f32 x, f32 y, f32 z, const base::math::CVector3D& ref) {
    return x + 0.001f >= ref.x && x - 0.001f <= ref.x && y + 0.001f >= ref.y && y - 0.001f <= ref.y &&
           z + 0.001f >= ref.z && z - 0.001f <= ref.z;
}

static inline f32 Sign(f32 value) {
    return (f32)((value > 0.0f) - (value < 0.0f));
}

static inline bool IsAlive(UnkFingMember* member) {
    return fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member));
}

void fn_71005796a0(UnkFingSwarm* self, s32 index, f32 dt) {
    UnkFingGroupList* list = self->data ? self->data->groupList : nullptr;
    UnkFingGroup* group = list->At(index);
    if (group->memberCount != 0) {
        f32 sumX = 0.0f;
        f32 sumY = 0.0f;
        f32 sumZ = 0.0f;
        f32 farthest = 10.0f;
        u32 alive = 0;
        for (u64 i = 0; i < group->memberCount; i++) {
            UnkFingMember* member = group->members[i];
            if (!member || !IsAlive(member) || member->field_0x28 != 0)
                continue;

            f32 px = member->position[0];
            f32 py = member->position[1];
            f32 pz = member->position[2];
            f32 distSq = px * px + py * py + pz * pz;
            farthest = distSq > farthest ? distSq : farthest;
            f32 limit = group->radius * group->radius * 0.75f * 0.75f;

            f32 dx, dy, dz, accel;
            if (distSq > limit) {
                // Too far out: back towards the centre.
                f32 inv = 1.0f / (sqrtf(distSq) + 0.0001f);
                dx = px * inv;
                dy = py * inv;
                dz = pz * inv;
                accel = -5000.0f;
            } else {
                // Away from the neighbours closer than 50.
                f32 awayX = 0.0f;
                f32 awayY = 0.0f;
                f32 awayZ = 0.0f;
                for (u64 j = 0; j < group->memberCount; j++) {
                    UnkFingMember* other = group->members[j];
                    if (!other || !IsAlive(other) || i == j)
                        continue;
                    f32 ox = px - other->position[0];
                    f32 oy = py - other->position[1];
                    f32 oz = pz - other->position[2];
                    if (ox * ox + oy * oy + oz * oz < 2500.0f) {
                        awayX += ox;
                        awayY += oy;
                        awayZ += oz;
                    }
                }
                if (IsNear(awayX, awayY, awayZ, g_unk_710164779c)) {
                    f32 inv = 1.0f / (sqrtf(distSq) + 0.0001f);
                    dx = px * inv;
                    dy = py * inv;
                    dz = pz * inv;
                    accel = -100.0f;
                } else {
                    f32 inv = 1.0f / (sqrtf(awayX * awayX + awayY * awayY + awayZ * awayZ) + 0.0001f);
                    dx = awayX * inv;
                    dy = awayY * inv;
                    dz = awayZ * inv;
                    accel = 5000.0f;
                }
            }

            f32 vx = member->velocity[0] + accel * dx * dt;
            f32 vy = member->velocity[1] + accel * dy * dt;
            f32 vz = member->velocity[2] + accel * dz * dt;
            f32 speedSq = vx * vx + vy * vy + vz * vz;
            if (speedSq > 62500.0f) {
                f32 inv = 1.0f / (sqrtf(speedSq) + 0.0001f);
                vx = vx * inv * 250.0f;
                vy = vy * inv * 250.0f;
                vz = vz * inv * 250.0f;
            }

            if (!member->field_0x180 && IsNear(Sign(member->field_0x6c), Sign(vx), 1e-06f)) {
                UnkFingBillboard* obj = self->data->groupList->field_0x10;
                UnkFingBillboard* billboard =
                    obj && fn_710009d50c(reinterpret_cast<const CClass* (*)(UnkFingBillboard*)>(obj->vtable[0])(obj),
                                         fn_710072be34())
                        ? obj
                        : nullptr;
                void* manager = g_unk_71726da048;
                if (!manager) {
                    manager = fn_71000839a0(0xe0, 0x3f);
                    ConstructBillboardManager(manager);
                    g_unk_71726da048 = manager;
                }
                u64 key = billboard->field_0xd0->field_0x30;
                fn_7100745440(reinterpret_cast<UnkTarget*>(member), fn_7100e1cb50(manager, &key, g_unk_7101649f58), 0);
                member->field_0x64 = 1.0f;
                member->field_0x180 = 1;
            }

            member->velocity[0] = vx;
            member->velocity[1] = vy;
            member->velocity[2] = vz;
            px += vx * dt;
            py += vy * dt;
            pz += vz * dt;
            member->position[0] = px;
            member->position[1] = py;
            member->position[2] = pz;
            sumX += px;
            sumY += py;
            sumZ += pz;
            alive++;
        }

        if (alive != 0) {
            f32 count = (f32)alive;
            f32 avgX = sumX / count;
            f32 avgY = sumY / count;
            f32 avgZ = sumZ / count;
            f32 spread = 10.0f;
            for (u64 i = 0; i < group->memberCount; i++) {
                UnkFingMember* member = group->members[i];
                if (!IsAlive(member))
                    continue;
                f32 ox = member->position[0] - avgX;
                f32 oy = member->position[1] - avgY;
                f32 oz = member->position[2] - avgZ;
                f32 distSq = ox * ox + oy * oy + oz * oz;
                spread = spread > distSq ? spread : distSq;
            }

            fn_7100745f10(group, sqrtf(farthest));
            UnkVec3Result centre = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(group));
            f32 cx = avgX + centre.x;
            f32 cy = avgY + centre.y;
            f32 cz = avgZ + centre.z;
            group->centre[0] = cx;
            group->centre[1] = cy;
            group->centre[2] = cz;
            UnkFingShape* shape = group->field_0xb0->shape;
            if (shape && shape->vf_0x18() == 2)
                fn_7100d618f8(reinterpret_cast<UnkSphereShape_ovb0135*>(shape), sqrtf(spread));
            return;
        }
    }
    fn_7100745f10(group, 10.0f);
}
