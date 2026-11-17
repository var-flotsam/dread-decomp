// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): every active group follows its path: when its
// node is behind it, it moves on to the next node at another place, turning when the path bends; then its
// distance, direction, side offset and speed are updated. True when a group ran out of path. Names and layouts
// are placeholders.
#include <math.h>
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "game/UnkTU1161.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkParent_71007455dc;

// A place on the swarm's path: an area and a node index in it.
struct UnkRedenkiPathKey_l65634c {
    u32 area;
    u32 index;
};

// A path node: its position (vf_0x18) and its speed (+0x14).
struct UnkRedenkiPathNode_l65634c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* Position();
    u8 pad_0x8[0xc];
    f32 speed;  // 0x14
};

struct UnkRedenkiPathArea_l65634c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Count();
};

struct UnkRedenkiFollowGroup_l65634c {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x44 - 0x25];
    f32 speed;  // 0x44
    UnkRedenkiPathKey_l65634c prevPos;  // 0x48
    UnkRedenkiPathKey_l65634c pathPos;  // 0x50
    u8 pad_0x58[0x4];
    f32 sideOffset;  // 0x5c
    f32 nodeSpeed;   // 0x60
    f32 distance;    // 0x64
    u8 pad_0x68[0x1];
    bool areaChanged;  // 0x69
    u8 pad_0x6a[0x88 - 0x6a];
    CVector3D direction;  // 0x88
    CVector3D normal;     // 0x94
    u8 pad_0xa0[0x148 - 0xa0];
};
static_assert(sizeof(UnkRedenkiFollowGroup_l65634c) == 0x148, "UnkRedenkiFollowGroup_l65634c layout");

struct UnkRedenkiFollowGroups_l65634c {
    u8 pad_0x0[0x50];
    UnkRedenkiFollowGroup_l65634c* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110
};

struct UnkRedenkiFollowOwner_l65634c {
    u8 pad_0x0[0x10];
    UnkRedenkiFollowGroups_l65634c* field_0x10;
};

struct UnkRedenkiFollow_l65634c {
    void** vtable;
    u8 pad_0x8[0xe8 - 0x8];
    UnkRedenkiFollowOwner_l65634c* field_0xe8;
    u8 pad_0xf0[0x338 - 0xf0];
    u32 field_0x338;

    UnkRedenkiPathArea_l65634c* Area(u32 area) {
        return reinterpret_cast<UnkRedenkiPathArea_l65634c* (*)(UnkRedenkiFollow_l65634c*, u32)>(
            vtable[0x1a8 / 8])(this, area);
    }
    void OnSharpTurn(UnkRedenkiFollowGroup_l65634c* group) {
        reinterpret_cast<void (*)(UnkRedenkiFollow_l65634c*, UnkRedenkiFollowGroup_l65634c*)>(vtable[0x2c8 / 8])(
            this, group);
    }
};

UnkRedenkiPathNode_l65634c* fn_7100749e44(UnkRedenkiFollow_l65634c* self, const UnkRedenkiPathKey_l65634c* key);
void fn_71007390d0(UnkRedenkiFollow_l65634c* self, UnkRedenkiFollowGroup_l65634c* group,
                   const UnkRedenkiPathKey_l65634c* from);

static const f32 kPi = 3.14159274f;

static inline bool IsNearZero(f32 v) {
    return v + 0.001f >= 0.0f && v - 0.001f <= 0.0f;
}

static inline f32 Length(f32 x, f32 y, f32 z) {
    return sqrtf(x * x + y * y + z * z);
}

static inline f32 Dot(const CVector3D& a, const CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// The angle between two directions given their dot product over their lengths.
static inline f32 AngleFromCos(f32 cosine) {
    if (cosine >= 1.0f)
        return 0.0f;
    if (cosine <= -1.0f)
        return kPi;
    return acosf(cosine);
}

static inline f32 Lerp(f32 a, f32 b, f32 t) {
    if (t >= 1.0f)
        return b;
    if (t < 0.0f)
        return a;
    return a * (1.0f - t) + b * t;
}

bool fn_710065634c(UnkDaivoSwarm_b0012* swarm) {
    UnkRedenkiFollow_l65634c* self = reinterpret_cast<UnkRedenkiFollow_l65634c*>(swarm);
    s32 count = self->field_0xe8->field_0x10->count;
    bool ended = false;
    for (s32 i = 0; i < count; ++i) {
        UnkRedenkiFollowGroup_l65634c* group = &self->field_0xe8->field_0x10->groups[i];
        if (!group->active)
            continue;
        self->field_0x338 |= 1;
        UnkVec3Result pos = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(group));
        UnkRedenkiPathKey_l65634c pathPos = group->pathPos;
        UnkRedenkiPathNode_l65634c* node = fn_7100749e44(self, &pathPos);
        if (!node)
            continue;

        CVector3D target = *node->Position();
        CVector3D forward = {group->normal.y, -group->normal.x, 0.0f};
        CVector3D dir = {target.x - pos.x, target.y - pos.y, target.z - pos.z};
        f32 len = Length(dir.x, dir.y, dir.z);
        bool behind = true;
        if (!IsNearZero(len)) {
            dir.x /= len;
            dir.y /= len;
            dir.z /= len;
            behind = Dot(forward, dir) < 0.0f;
        }

        if (behind) {
            // Move on to the next node of the area that is at another place.
            UnkRedenkiPathKey_l65634c start = group->pathPos;
            bool found = false;
            for (;;) {
                bool advanced;
                UnkRedenkiPathArea_l65634c* area = self->Area(pathPos.area);
                if (area->Count() >= 2 && area != nullptr && pathPos.index < area->Count() - 1) {
                    pathPos.index++;
                    advanced = true;
                } else {
                    advanced = false;
                    ended = true;
                }
                UnkRedenkiPathNode_l65634c* a = fn_7100749e44(self, &start);
                UnkRedenkiPathNode_l65634c* b = fn_7100749e44(self, &pathPos);
                if (!a || !b)
                    break;
                bool samePlace = true;
                if (start.index != pathPos.index || start.area != pathPos.area) {
                    CVector2D pa, pb;
                    fn_710007b430(&pa, a->Position());
                    fn_710007b430(&pb, b->Position());
                    samePlace = fabsf(pa.x - pb.x) + fabsf(pa.y - pb.y) < 0.001f;
                }
                if (!(advanced & samePlace)) {
                    found = advanced;
                    break;
                }
            }

            if (found) {
                fn_71007390d0(self, group, &start);
                group->pathPos = pathPos;
                group->areaChanged = group->prevPos.area != pathPos.area;
                node = fn_7100749e44(self, &pathPos);
                const CVector3D* next = node->Position();
                if (IsNearZero(fabsf(next->x - target.x) + fabsf(next->y - target.y))) {
                    len = 0.0f;
                    dir = group->direction;
                } else {
                    f32 ex = next->x - target.x;
                    f32 ey = next->y - target.y;
                    f32 ez = next->z - target.z;
                    f32 inv = 1.0f / (Length(ex, ey, ez) + 0.0001f);
                    target = *node->Position();
                    CVector3D d = {target.x - pos.x, target.y - pos.y, target.z - pos.z};
                    if (IsNearZero(fabsf(d.x) + fabsf(d.y) + fabsf(d.z))) {
                        const CVector3D* from = fn_7100749e44(self, &start)->Position();
                        d.x = target.x - from->x;
                        d.y = target.y - from->y;
                        d.z = target.z - from->z;
                    }
                    len = Length(d.x, d.y, d.z);
                    CVector3D edge = {ex * inv, ey * inv, ez * inv};
                    f32 cosine = Dot(forward, edge) / (Length(edge.x, edge.y, edge.z) *
                                                       Length(forward.x, forward.y, forward.z));
                    group->nodeSpeed = node->speed;
                    if (AngleFromCos(cosine) > kPi * 5.0f / 8.0f)
                        self->OnSharpTurn(group);
                    dir.x = d.x / len;
                    dir.y = d.y / len;
                    group->normal = {-edge.y, edge.x, 0.0f};
                    dir.z = d.z / len;
                }
            }
        }

        group->distance = len;
        group->direction = dir;
        UnkRedenkiPathKey_l65634c prevPos = group->prevPos;
        UnkRedenkiPathNode_l65634c* prev = fn_7100749e44(self, &prevPos);
        if (!prev)
            continue;
        const CVector3D* prevPosition = prev->Position();
        CVector3D toTarget = {target.x - prevPosition->x, target.y - prevPosition->y, target.z - prevPosition->z};
        f32 offset = group->sideOffset;
        if (!IsNearZero(toTarget.x + toTarget.y)) {
            f32 cosine = Dot(dir, toTarget) / (Length(dir.x, dir.y, dir.z) *
                                               Length(toTarget.x, toTarget.y, toTarget.z));
            offset = fabsf(len * sinf(AngleFromCos(cosine)));
            group->sideOffset = offset;
        }
        const CVector3D& zero = g_unk_710164779c;
        f32 speed;
        if (fabsf(toTarget.x - zero.x) < 0.01f && fabsf(toTarget.y - zero.y) < 0.01f &&
            fabsf(toTarget.z - zero.z) < 0.01f) {
            speed = prev->speed;
        } else {
            f32 t = 1.0f - len / Length(toTarget.x, toTarget.y, toTarget.z);
            speed = Lerp(prev->speed, node->speed, t);
        }
        speed -= offset;
        group->speed = !(speed < 10.0f) ? speed : 10.0f;
    }
    return ended;
}
