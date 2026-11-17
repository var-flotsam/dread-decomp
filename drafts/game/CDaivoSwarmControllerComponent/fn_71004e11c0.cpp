// CDaivoSwarmControllerComponent's unit (tools/tu_map.py unit 768): the update that moves each group of
// the swarm from node to node along its path, then picks the controller's next state once it stops.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU1161.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern "C" f32 sqrtf(f32);
extern "C" f32 acosf(f32);
extern "C" f32 sinf(f32);

#define UNK_VF8(n) virtual void vf_##n##_0(); virtual void vf_##n##_1(); virtual void vf_##n##_2(); \
    virtual void vf_##n##_3(); virtual void vf_##n##_4(); virtual void vf_##n##_5(); \
    virtual void vf_##n##_6(); virtual void vf_##n##_7();

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

// A place on a path: which path, and which node along it.
struct UnkPathKey_l4e11c0 {
    u32 path;
    u32 index;

    bool operator==(const UnkPathKey_l4e11c0& o) const { return index == o.index && path == o.path; }
};

class UnkPathNode_l4e11c0 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkVec3Result* Position();  // 0x18

    u8 pad_0x8[0x14 - 0x8];
    f32 field_0x14;
};

class UnkPath_l4e11c0 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Count();  // 0x28
};

struct UnkSwarmMember_l4e11c0 {
    u8 pad_0x0[0x25];
    u8 marked;  // 0x25
};

// A group of members (0x148 bytes) following a path.
struct UnkPathGroup_l4e11c0 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x44 - 0x25];
    f32 field_0x44;
    UnkPathKey_l4e11c0 field_0x48;
    UnkPathKey_l4e11c0 key;  // 0x50
    u8 pad_0x58[0x5c - 0x58];
    f32 offset;  // 0x5c
    f32 field_0x60;
    f32 distance;  // 0x64
    u8 pad_0x68;
    bool changedPath;  // 0x69
    u8 pad_0x6a[0x88 - 0x6a];
    CVector3D dir;  // 0x88
    CVector3D side;  // 0x94
    u8 pad_0xa0[0xb8 - 0xa0];
    UnkSwarmMember_l4e11c0** members;  // 0xb8
    u32 memberCount;  // 0xc0
    u8 pad_0xc4[0x148 - 0xc4];

    UnkSwarmMember_l4e11c0** begin() { return members; }
    UnkSwarmMember_l4e11c0** end() { return members + memberCount; }
};
static_assert(sizeof(UnkPathGroup_l4e11c0) == 0x148, "UnkPathGroup_l4e11c0 layout");

struct UnkPathGroupList_l4e11c0 {
    u8 pad_0x0[0x50];
    UnkPathGroup_l4e11c0* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    // The group at `i`, or null past the end.
    UnkPathGroup_l4e11c0* At(s64 i) {
        if (i >= count)
            return nullptr;
        return groups ? &groups[i] : nullptr;
    }
};

struct UnkPathData_l4e11c0 {
    u8 pad_0x0[0x10];
    UnkPathGroupList_l4e11c0* groupList;  // 0x10
};

class UnkDaivoSwarm_l4e11c0 {
public:
    UNK_VF8(0) UNK_VF8(1) UNK_VF8(2) UNK_VF8(3) UNK_VF8(4) UNK_VF8(5)
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual UnkPath_l4e11c0* vf_0x1a8(u32 path);
    UNK_VF8(6) UNK_VF8(7) UNK_VF8(8) UNK_VF8(9)
    virtual void vf_0x2b0();
    virtual void vf_0x2b8();
    virtual void vf_0x2c0();
    virtual void vf_0x2c8(UnkPathGroup_l4e11c0* group);
    virtual void vf_0x2d0();
    virtual void vf_0x2d8();
    virtual void vf_0x2e0();
    virtual void vf_0x2e8(u8 state, bool arg);
    virtual void vf_0x2f0();
    virtual void vf_0x2f8();
    virtual bool vf_0x300();

    u8 pad_0x8[0xe8 - 0x8];
    UnkPathData_l4e11c0* data;  // 0xe8
    u8 pad_0xf0[0x338 - 0xf0];
    u32 flags;  // 0x338
    u8 pad_0x33c[0x554 - 0x33c];
    bool field_0x554;
    u8 pad_0x555[0x56c - 0x555];
    f32 field_0x56c;
    u8 pad_0x570[0x588 - 0x570];
    bool moving;  // 0x588
};

#undef UNK_VF8

UnkPathNode_l4e11c0* fn_7100749e44(UnkDaivoSwarm_l4e11c0* self, const UnkPathKey_l4e11c0* key);
void fn_71007390d0(UnkDaivoSwarm_l4e11c0* self, UnkPathGroup_l4e11c0* group, const UnkPathKey_l4e11c0* from);
void fn_710065034c(UnkDaivoSwarm_l4e11c0* self, f32 dt);
bool fn_71006502f4(UnkDaivoSwarm_l4e11c0* self, const UnkVec3Result* pos);

static inline f32 Length(const CVector3D& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline f32 Dot(const CVector3D& a, const CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline bool IsZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

// The angle between two directions, in radians.
static inline f32 AngleBetween(const CVector3D& a, const CVector3D& b) {
    f32 c = Dot(a, b) / (Length(a) * Length(b));
    if (c >= 1.0f)
        return 0.0f;
    if (c <= -1.0f)
        return 3.14159265f;
    return acosf(c);
}

static inline f32 Lerp(f32 a, f32 b, f32 t) {
    if (t >= 1.0f)
        return b;
    if (t < 0.0f)
        return a;
    return a * (1.0f - t) + b * t;
}

// Past the path's first node, the members fn_7100744e1c accepts are marked.
static inline void MarkMembers(UnkPathGroup_l4e11c0* group, UnkPathKey_l4e11c0 key) {
    if (key.index == 0)
        return;
    for (u64 m = 0; m < group->memberCount; m++) {
        UnkSwarmMember_l4e11c0* member = group->members[m];
        if (fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member)))
            member->marked = 1;
    }
}

static inline UnkPathGroupList_l4e11c0* GroupList(UnkDaivoSwarm_l4e11c0* self) {
    return self->data ? self->data->groupList : nullptr;
}

// Each active group measures its way to its node; once past it, the group steps to the next nodes of its
// path (marking the members fn_7100744e1c accepts), turning (vf_0x2c8) when the path bends back. Its
// distance to the node ahead (+0x48) sets +0x44.
static inline void FollowPaths(UnkDaivoSwarm_l4e11c0* self) {
    s32 count = self->data->groupList->count;
    for (s32 i = 0; i < count; i++) {
        UnkPathGroup_l4e11c0* group = &self->data->groupList->groups[i];
        if (!group->active)
            continue;
        self->flags |= 1;
        UnkVec3Result target = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(group));
        UnkPathKey_l4e11c0 key = group->key;
        UnkPathNode_l4e11c0* node = fn_7100749e44(self, &key);
        if (!node)
            continue;
        UnkVec3Result pos = *node->Position();
        // The heading the side vector stands square to.
        CVector3D heading = {group->side.y, -group->side.x, 0.0f};
        CVector3D toNode = {pos.x - target.x, pos.y - target.y, pos.z - target.z};
        f32 dist = Length(toNode);
        bool passed = IsZero(dist);
        if (!passed) {
            toNode.x /= dist;
            toNode.y /= dist;
            toNode.z /= dist;
            passed = Dot(heading, toNode) < 0.0f;
        }
        if (passed) {
            UnkPathKey_l4e11c0 from = group->key;
            for (;;) {
                MarkMembers(group, group->key);
                UnkPath_l4e11c0* path = self->vf_0x1a8(key.path);
                bool advanced = false;
                if (path) {
                    u32 index = key.index;
                    if (index >= 4 && !self->field_0x554)
                        self->moving = false;
                    if (index < path->Count() - 1) {
                        key.index++;
                        advanced = true;
                    }
                }
                if (!advanced)
                    self->moving = false;
                UnkPathNode_l4e11c0* fromNode = fn_7100749e44(self, &from);
                UnkPathNode_l4e11c0* toNodeNext = fn_7100749e44(self, &key);
                if (!fromNode || !toNodeNext)
                    break;
                // Skip nodes at the same place as the one left, seen from above.
                bool samePlace = from == key;
                if (!samePlace) {
                    CVector2D fromPlace;
                    fn_710007b430(&fromPlace, reinterpret_cast<const CVector3D*>(fromNode->Position()));
                    CVector2D toPlace;
                    fn_710007b430(&toPlace, reinterpret_cast<const CVector3D*>(toNodeNext->Position()));
                    samePlace = __builtin_fabsf(fromPlace.x - toPlace.x) + __builtin_fabsf(fromPlace.y - toPlace.y) <
                                0.001f;
                }
                if (advanced & samePlace)
                    continue;
                if (advanced) {
                    fn_71007390d0(self, group, &from);
                    group->key = key;
                    group->changedPath = group->field_0x48.path != group->key.path;
                    UnkPathNode_l4e11c0* next = fn_7100749e44(self, &key);
                    const UnkVec3Result& np = *next->Position();
                    if (IsZero(__builtin_fabsf(np.x - pos.x) + __builtin_fabsf(np.y - pos.y))) {
                        // No move: keep the direction.
                        toNode = group->dir;
                        dist = 0.0f;
                    } else {
                        CVector3D move = {np.x - pos.x, np.y - pos.y, np.z - pos.z};
                        f32 inv = 1.0f / (Length(move) + 0.0001f);
                        pos = *next->Position();
                        CVector3D to = {pos.x - target.x, pos.y - target.y, pos.z - target.z};
                        if (IsZero(__builtin_fabsf(pos.x - target.x) + __builtin_fabsf(pos.y - target.y) +
                                   __builtin_fabsf(pos.z - target.z))) {
                            const UnkVec3Result& prev = *fn_7100749e44(self, &from)->Position();
                            to.x = pos.x - prev.x;
                            to.y = pos.y - prev.y;
                            to.z = pos.z - prev.z;
                        }
                        CVector3D dir = {move.x * inv, move.y * inv, move.z * inv};
                        dist = Length(to);
                        group->field_0x60 = next->field_0x14;
                        // A turn of more than 5/8 of a half-turn.
                        if (AngleBetween(dir, heading) > 1.9634954f)
                            self->vf_0x2c8(group);
                        toNode.x = to.x / dist;
                        toNode.y = to.y / dist;
                        group->side.x = -dir.y;
                        group->side.y = dir.x;
                        group->side.z = 0.0f;
                        toNode.z = to.z / dist;
                    }
                    node = next;
                }
                break;
            }
        }
        group->dir = toNode;
        group->distance = dist;
        UnkPathKey_l4e11c0 aheadKey = group->field_0x48;
        UnkPathNode_l4e11c0* ahead = fn_7100749e44(self, &aheadKey);
        if (!ahead)
            continue;
        const UnkVec3Result& ap = *ahead->Position();
        CVector3D toAhead = {pos.x - ap.x, pos.y - ap.y, pos.z - ap.z};
        f32 offset = group->offset;
        if (!IsZero(toAhead.x + toAhead.y)) {
            offset = __builtin_fabsf(dist * sinf(AngleBetween(toNode, toAhead)));
            group->offset = offset;
        }
        const CVector3D& zero = g_unk_710164779c;
        f32 value;
        if (__builtin_fabsf(toAhead.x - zero.x) < 0.01f && __builtin_fabsf(toAhead.y - zero.y) < 0.01f &&
            __builtin_fabsf(toAhead.z - zero.z) < 0.01f)
            value = ahead->field_0x14;
        else
            value = Lerp(ahead->field_0x14, node->field_0x14, 1.0f - dist / Length(toAhead));
        f32 gap = value - offset;
        group->field_0x44 = !(gap < 10.0f) ? gap : 10.0f;
    }
}

// Once the swarm stops: every member of an active group is marked and the controller picks its next state.
static inline void Stop(UnkDaivoSwarm_l4e11c0* self) {
    s32 count = self->data->groupList->count;
    for (s32 i = 0; i < count; i++) {
        UnkPathGroup_l4e11c0* group = GroupList(self)->At(i);
        if (!group || !group->active)
            continue;
        for (UnkSwarmMember_l4e11c0* member : *group)
            member->marked = 1;
    }
    if (self->field_0x554) {
        fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 2);
        self->vf_0x2e8(4, false);
        return;
    }
    if (self->field_0x56c > 0.1f && self->vf_0x300()) {
        fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 2);
        return;
    }
    UnkParent_71007455dc* target = fn_7100739c64(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self));
    if (!target)
        return;
    UnkVec3Result pos = fn_7100745e24(target);
    if (fn_71006502f4(self, &pos))
        fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 1);
}

// While moving (+0x588) the groups follow their paths, and the swarm stops once one of them ends it;
// otherwise fn_710065034c.
void fn_71004e11c0(UnkDaivoSwarm_l4e11c0* self, f32 dt) {
    if (self->moving) {
        FollowPaths(self);
        if (!self->moving)
            Stop(self);
    } else {
        fn_710065034c(self, dt);
    }
}
