// CFlockingSwarmControllerComponent unit (unit 1158 of tools/tu_map.py): sends the swarm to the node nearest its
// target. When a route to that node is found, the route's points become the swarm's path, every member
// joins the current group's first node (a member changing group leaves its old one first) and is set
// facing the group's second node. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/SLogicPathNode.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkTU1161.h"
#include "game/UnkXformNode.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkPathPointInit {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    f32 field_0x18;
};

// A node of the swarm's node graph: its group and its index within it.
struct UnkSwarmNodeKey_l73f888 {
    s32 group;
    s32 index;
};

struct UnkSwarmNode_l73f888 {
    void** vtable;
    u8 pad_0x8[0xc];
    f32 field_0x14;

    const CVector3D* Pos() {
        return reinterpret_cast<const CVector3D* (*)(UnkSwarmNode_l73f888*)>(vtable[0x18 / 8])(this);
    }
};

struct __attribute__((packed, aligned(4))) UnkSwarmRoutePoint_l73f888 {
    void** vtable;
    u8 pad_0x8[0x118];

    const CVector3D* Pos() {
        return reinterpret_cast<const CVector3D* (*)(UnkSwarmRoutePoint_l73f888*)>(vtable[0x18 / 8])(this);
    }
};
static_assert(sizeof(UnkSwarmRoutePoint_l73f888) == 0x120, "route point size");

struct UnkSwarmRoute_l73f888 {
    void** vtable;
    u8 pad_0x8[4];
    UnkSwarmRoutePoint_l73f888 points[0x108];  // 0xc

    u32 Count() { return reinterpret_cast<u32 (*)(UnkSwarmRoute_l73f888*)>(vtable[0x28 / 8])(this); }
};

// The route search's scratch space (fn_71002acfe4 builds it, fn_71002ad0dc releases it).
struct UnkSwarmRouteScratch_l73f888 {
    u8 pad_0x0[0x10];
    UnkSwarmRoute_l73f888 route;  // 0x10
    u8 pad_0x12920[0x168];

    UnkSwarmRouteScratch_l73f888() { fn_71002acfe4(this); }
    ~UnkSwarmRouteScratch_l73f888() { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this)); }
};
static_assert(sizeof(UnkSwarmRouteScratch_l73f888) == 0x12a88, "scratch size");

struct UnkSwarmPathPoint_l73f888 {
    void** vtable;
    u8 pad_0x8[0x18];
    ~UnkSwarmPathPoint_l73f888() { reinterpret_cast<void (*)(UnkSwarmPathPoint_l73f888*)>(vtable[1])(this); }
};

struct UnkSwarmPath_l73f888 {
    u8 pad_0x0[0x8];
    base::global::CRntVector<UnkSwarmPathPoint_l73f888> points;  // 0x8
};

struct UnkSwarmMember_l73f888 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x17];
    s8 field_0x3c;
    u8 pad_0x3d[0xb];
    UnkSwarmNodeKey_l73f888 node;  // 0x48
    UnkSwarmNodeKey_l73f888 next_node;  // 0x50
    u8 pad_0x58[4];
    f32 lateral;  // 0x5c
    u8 pad_0x60[4];
    f32 distance;  // 0x64
    bool field_0x68;
    bool changing_group;  // 0x69
    u8 pad_0x6a[6];
    UnkVec3f velocity;  // 0x70
    u8 pad_0x7c[0xc];
    CVector3D heading;  // 0x88
    CVector3D side;  // 0x94
    u8 pad_0xa0[0xa8];
};
static_assert(sizeof(UnkSwarmMember_l73f888) == 0x148, "member size");

struct UnkSwarm_l73f888 {
    u8 pad_0x0[0x50];
    UnkSwarmMember_l73f888* members;  // 0x50
    u8 pad_0x58[0xb8];
    s32 count;  // 0x110
};

struct UnkSwarmHolder_l73f888 {
    u8 pad_0x0[0x10];
    UnkSwarm_l73f888* swarm;  // 0x10
};

struct UnkSwarmGroupSource_l73f888 {
    void** vtable;
    s32 Group() { return reinterpret_cast<s32 (*)(UnkSwarmGroupSource_l73f888*)>(vtable[0x28 / 8])(this); }
};

struct UnkSwarmTunables_l73f888 {
    u8 pad_0x0[0x458];
    u32 path_weight;  // 0x458
};

struct UnkFlockingSwarm_l73f888 {
    u8 pad_0x0[0x80];
    UnkSwarmGroupSource_l73f888* group_source;  // 0x80
    u8 pad_0x88[0x60];
    UnkSwarmHolder_l73f888* holder;  // 0xe8
    u8 pad_0xf0[0x50];
    base::global::CRntVector<base::global::CRntVector<UnkEntry*>> groups;  // 0x140
    u8 pad_0x168[0x160];
    UnkSwarmPath_l73f888 path;  // 0x2c8
    u8 pad_0x2f8[0x28];
    UnkSwarmNodeKey_l73f888 target;  // 0x320
    UnkSwarmTunables_l73f888* tunables;  // 0x328

    UnkSwarmMember_l73f888* Member(s32 index) {
        UnkSwarm_l73f888* swarm = holder->swarm;
        return index < swarm->count ? (swarm->members ? &swarm->members[index] : nullptr) : nullptr;
    }
};
static_assert(__builtin_offsetof(UnkFlockingSwarm_l73f888, path) == 0x2c8, "layout");
static_assert(__builtin_offsetof(UnkFlockingSwarm_l73f888, target) == 0x320, "layout");

struct UnkRedenkiSwarm_ovb0084;
struct UnkRedenkiScratch_ovb0084;
bool fn_710073cbe4(UnkRedenkiSwarm_ovb0084* self, CVector2D* from, CVector2D* to, UnkRedenkiScratch_ovb0084* scratch,
                   bool flag);
struct UnkSwarmController_A5_b68;
void fn_7100748370(UnkSwarmController_A5_b68* self, void* member, u32 index);
UnkSwarmNode_l73f888* fn_7100749e44(UnkFlockingSwarm_l73f888* self, UnkSwarmNodeKey_l73f888* key);
void fn_710073912c(UnkFlockingSwarm_l73f888* self, UnkSwarmMember_l73f888* member, s32 group);
void fn_7100745f10(UnkSwarmMember_l73f888* member, f32 value);
void fn_7101059318(UnkSwarmPath_l73f888* path, const UnkPathPointInit* point);

// Whether v is outside [-0.1, 0.1] (or not a number).
static inline bool NotNearZero(f32 v) {
    return !(v + 0.1f >= 0.0f) || !(v - 0.1f <= 0.0f);
}

void fn_710073f888(UnkFlockingSwarm_l73f888* self) {
    UnkSwarmUpdate_b65a4* update = reinterpret_cast<UnkSwarmUpdate_b65a4*>(self);
    UnkVec3Result pos = fn_7100745e24(fn_7100739c64(update));
    UnkSwarmNodeKey_l73f888 target = {};
    if (!fn_710073fea8(update, &pos, reinterpret_cast<UnkSwarmPoint_b65a4*>(&target), 50.0f))
        return;
    UnkSwarmNode_l73f888* goal = fn_7100749e44(self, &target);
    if (goal == nullptr)
        return;

    UnkSwarmRouteScratch_l73f888 scratch;
    {
        CVector2D from;
        fn_710007b430(&from, reinterpret_cast<const CVector3D*>(&pos));
        CVector2D to;
        fn_710007b430(&to, goal->Pos());
        if (!fn_710073cbe4(reinterpret_cast<UnkRedenkiSwarm_ovb0084*>(self), &from, &to,
                           reinterpret_cast<UnkRedenkiScratch_ovb0084*>(&scratch), true))
            return;
    }

    self->target = target;
    self->path.points.Clear();
    u32 n = scratch.route.Count();
    for (u32 i = 0; i < n; ++i) {
        const u32* p = reinterpret_cast<const u32*>(scratch.route.points[i].Pos());
        UnkPathPointInit point;
        point.vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
        point.field_0x8 = p[0];
        point.field_0xc = p[1];
        point.field_0x10 = p[2];
        point.field_0x14 = self->tunables->path_weight;
        point.field_0x18 = 1.0f;
        fn_7101059318(&self->path, &point);
    }

    for (base::global::CRntVector<UnkEntry*>& group : self->groups)
        for (UnkEntry* entry : group)
            fn_7100d35a98(entry, false);

    s32 group = self->group_source->Group();
    UnkSwarmNodeKey_l73f888 first = {group, 0};
    CVector3D start = *fn_7100749e44(self, &first)->Pos();
    UnkSwarmNodeKey_l73f888 second = {group, 1};
    UnkSwarmNode_l73f888* next = fn_7100749e44(self, &second);
    CVector3D end = *next->Pos();
    f32 dx = end.x - start.x;
    f32 dy = end.y - start.y;
    f32 dz = end.z - start.z;
    f32 len_sq = dx * dx + dy * dy + dz * dz;
    f32 inv = 1.0f / (sqrtf(len_sq) + 0.0001f);
    CVector3D dir;
    dir.z = dz * inv;
    dir.x = dx * inv;
    dir.y = dy * inv;

    s32 count = self->holder->swarm->count;
    for (s32 i = 0; i < count; ++i) {
        UnkSwarmMember_l73f888* member = self->Member(i);
        if (!member->active)
            continue;
        s32 old_group = member->node.group;
        if (first.group != old_group) {
            fn_7100748370(reinterpret_cast<UnkSwarmController_A5_b68*>(self), member, old_group);
            fn_710073912c(self, member, first.group);
        }
        member->node = first;
        member->next_node = second;
        member->changing_group = second.group != first.group;

        UnkVec3Result at = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(member));
        f32 tx = end.x - at.x;
        f32 ty = end.y - at.y;
        f32 tz = end.z - at.z;
        f32 distance = sqrtf(tx * tx + ty * ty + tz * tz);
        f32 scale = 1.0f / distance;
        member->heading.x = tx * scale;
        member->heading.y = ty * scale;
        member->heading.z = tz * scale;
        member->distance = distance;

        // How far the member is from the line through the group's first two nodes.
        f32 ox = at.x - start.x;
        f32 oy = at.y - start.y;
        f32 oz = at.z - start.z;
        f32 lateral = 0.0f;
        if (NotNearZero(ox) || NotNearZero(oy) || NotNearZero(oz)) {
            f32 dot = dx * ox + dy * oy + dz * oz;
            f32 off_sq = ox * ox + oy * oy + oz * oz;
            f32 cos_angle = dot / (sqrtf(off_sq) * sqrtf(len_sq));
            f32 angle;
            if (cos_angle >= 1.0f)
                angle = 0.0f;
            else if (cos_angle <= -1.0f)
                angle = 3.14159265f;
            else
                angle = acosf(cos_angle);
            if (angle != angle)  // not a number
                angle = 0.0f;
            lateral = fabsf(sinf(angle) * sqrtf(off_sq));
        }
        member->lateral = lateral;
        member->side = {-dir.y, dir.x, 0.0f};
        fn_7100745f10(member, next->field_0x14);
        member->velocity = reinterpret_cast<const UnkVec3f&>(g_unk_710164779c);

        CVector2D dir2;
        fn_710007b430(&dir2, &dir);
        f32 turn = acosf(-dir2.x) / 6.28318531f;
        f32 facing = dir2.y > 0.0f ? 1.0f - turn : turn;
        member->field_0x68 = false;
        member->field_0x3c = -static_cast<s32>(facing);
    }
}
