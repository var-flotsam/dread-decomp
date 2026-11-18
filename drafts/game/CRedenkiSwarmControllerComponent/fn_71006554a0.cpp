// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): the sub-area change. Whether `area` is among
// the sub-areas at the swarm's position (fn_7100ee2bd8) is written into every member's +0x25; when it is, the swarm
// goes for its target (state 1) or else spreads its groups along the path between two nodes (fn_7100749e44), each
// group taking its direction, distance and side offset (state 0); otherwise mode 2 becomes state 3. Names and
// layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU2577.h"
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

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_Update_b65a4.cpp defines them.
struct UnkSwarmPoint_b65a4 {
    u64 value;
};

struct UnkSwarmRecord_b65a4 {
    bool active;
};

struct UnkObjMgrB_b189;
struct UnkRecB_b189;
struct UnkObjB_b189;
bool fn_7100ee2bd8(UnkObjMgrB_b189* self, base::global::CRntVector<UnkRecB_b189*>* out, const base::math::CVector2D* pos,
                   u32 mask, UnkObjB_b189* skip);

// A sub-area record as the swarm compares it: by the id it starts with.
struct UnkSubAreaRecord_l6554a0 {
    u64 id;
};

// The sub-area the swarm is told about.
struct UnkSubAreaRef_l6554a0 {
    u8 pad_0x0[0x10];
    u64 id;  // 0x10
};

struct UnkSubAreaManager_l6554a0;

static inline UnkSubAreaManager_l6554a0* SubAreaManager_l6554a0() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return static_cast<UnkSubAreaManager_l6554a0*>(manager);
}

// Collects the sub-area records at a position.
static inline bool RecordsAt(UnkSubAreaManager_l6554a0* manager, base::global::CRntVector<UnkRecB_b189*>* records,
                             const CVector3D& position) {
    CVector2D position2;
    fn_710007b430(&position2, &position);
    return fn_7100ee2bd8(reinterpret_cast<UnkObjMgrB_b189*>(manager), records, &position2, 4, nullptr);
}

// A node of the path: its area and index on it.
struct UnkPathKey_l6554a0 {
    u32 area;
    u32 index;
};

struct UnkPathNode_l6554a0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* GetPos();
};

struct UnkSpreadMember_l6554a0 {
    u8 pad_0x0[0x25];
    bool inArea;  // 0x25
};

// One group (0x148 bytes).
struct UnkSpreadGroup_l6554a0 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x48 - 0x25];
    u32 area;  // 0x48
    u8 pad_0x4c[0x4];
    UnkPathKey_l6554a0 key;  // 0x50
    u8 pad_0x58[0x5c - 0x58];
    f32 sideDistance;  // 0x5c
    u8 pad_0x60[0x64 - 0x60];
    f32 distance;  // 0x64
    u8 pad_0x68;
    bool otherArea;  // 0x69
    u8 pad_0x6a[0x88 - 0x6a];
    CVector3D dir;  // 0x88
    CVector3D side;  // 0x94
    u8 pad_0xa0[0xb8 - 0xa0];
    UnkSpreadMember_l6554a0** members;  // 0xb8
    u32 count;  // 0xc0
    u8 pad_0xc4[0x148 - 0xc4];
};

struct UnkSpreadGroups_l6554a0 {
    u8 pad_0x0[0x50];
    UnkSpreadGroup_l6554a0* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    UnkSpreadGroup_l6554a0* Group(s64 index) {
        if (index >= count)
            return nullptr;
        return groups ? &groups[index] : nullptr;
    }
};

struct UnkSpreadChain_l6554a0 {
    u8 pad_0x0[0x10];
    UnkSpreadGroups_l6554a0* field_0x10;
};

struct UnkSpreadTransform_l6554a0 {
    u8 pad_0x0[0x38];
    f32 matrix[3][4];  // 0x38, the translation in the last column
};

static inline CVector3D Translation(const UnkSpreadTransform_l6554a0* transform) {
    return {transform->matrix[0][3], transform->matrix[1][3], transform->matrix[2][3]};
}

struct UnkSpreadSwarm_l6554a0 {
    u8 pad_0x0[0x30];
    UnkSpreadTransform_l6554a0* transform;  // 0x30
    u8 pad_0x38[0xe8 - 0x38];
    UnkSpreadChain_l6554a0* field_0xe8;
    u8 pad_0xf0[0x31c - 0xf0];
    UnkSwarmRecord_b65a4 record;  // 0x31c
    u8 pad_0x31d[0x390 - 0x31d];
    u32 mode;  // 0x390

    UnkSpreadGroups_l6554a0* Groups() { return field_0xe8 ? field_0xe8->field_0x10 : nullptr; }
};

UnkPathNode_l6554a0* fn_7100749e44(UnkSpreadSwarm_l6554a0* self, const UnkPathKey_l6554a0* key);
void fn_71007390d0(UnkSpreadSwarm_l6554a0* self, UnkSpreadGroup_l6554a0* group, const UnkPathKey_l6554a0* key);

static inline f32 Length(const CVector3D& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }
static inline f32 Dot(const CVector3D& a, const CVector3D& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

static inline f32 SafeAcos(f32 c) {
    if (c >= 1.0f)
        return 0.0f;
    if (c <= -1.0f)
        return 3.14159274f;
    return acosf(c);
}

// Sends the swarm for a point near the target, when there is one.
static inline bool StartChase(UnkSpreadSwarm_l6554a0* self, UnkParent_71007455dc* target) {
    if (!target)
        return false;
    UnkVec3Result pos = fn_7100745e24(target);
    UnkSwarmPoint_b65a4 point;
    point.value = 0;
    if (!fn_710073fea8(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self), &pos, &point, 50.0f))
        return false;
    fn_7100737d4c(&self->record, &point);
    self->record.active = true;
    return true;
}

bool fn_71006554a0(UnkSpreadSwarm_l6554a0* self, UnkSubAreaRef_l6554a0* ref) {
    base::global::CRntVector<UnkRecB_b189*> records(0xf);
    UnkSubAreaManager_l6554a0* manager = SubAreaManager_l6554a0();
    if (RecordsAt(manager, &records, Translation(self->transform))) {
        bool found = false;
        if (records.count != 0) {
            u32 i = 0;
            do {
                found = reinterpret_cast<UnkSubAreaRecord_l6554a0*>(records.GrowAt(i))->id == ref->id;
            } while (!found && ++i < records.count);
        }

        s32 n = self->field_0xe8->field_0x10->count;
        for (s32 g = 0; g < n; ++g) {
            UnkSpreadGroup_l6554a0* group = self->Groups()->Group(g);
            for (u32 j = 0; j < group->count; ++j) {
                UnkSpreadMember_l6554a0* member = group->members[j];
                if (fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member)))
                    member->inArea = found;
            }
        }

        UnkSwarmUpdate_b65a4* update = reinterpret_cast<UnkSwarmUpdate_b65a4*>(self);
        UnkRedenkiSwarmController* controller = reinterpret_cast<UnkRedenkiSwarmController*>(self);
        if (found) {
            UnkParent_71007455dc* target = fn_7100739c64(update);
            if (StartChase(self, target)) {
                fn_710073f7bc(controller, 1);
            } else {
                UnkSwarmPoint_b65a4 point;
                point.value = 0;
                UnkVec3Result pos = fn_7100745e24(target);
                u32 area = fn_710073fea8(update, &pos, &point, 50.0f) ? (u32)point.value : 0;
                UnkPathKey_l6554a0 start = {area, 0};
                UnkPathKey_l6554a0 end = {area, 1};
                UnkPathNode_l6554a0* startNode = fn_7100749e44(self, &start);
                UnkPathNode_l6554a0* endNode = fn_7100749e44(self, &end);
                if (startNode && endNode) {
                    const CVector3D* from = startNode->GetPos();
                    const CVector3D* to = endNode->GetPos();
                    CVector3D dir = {to->x - from->x, to->y - from->y, to->z - from->z};
                    f32 length = Length(dir);
                    for (s32 g = 0; g < self->field_0xe8->field_0x10->count; ++g) {
                        UnkSpreadGroup_l6554a0* group = self->Groups()->Group(g);
                        if (!group->active)
                            continue;
                        UnkVec3Result groupPos = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(group));
                        CVector3D toEnd = {to->x - groupPos.x, to->y - groupPos.y, to->z - groupPos.z};
                        f32 distance = Length(toEnd);
                        f32 inv = 1.0f / distance;
                        group->dir.x = toEnd.x * inv;
                        group->dir.y = toEnd.y * inv;
                        group->dir.z = toEnd.z * inv;
                        group->distance = distance;
                        CVector3D fromStart = {groupPos.x - from->x, groupPos.y - from->y, groupPos.z - from->z};
                        f32 angle = SafeAcos(Dot(dir, fromStart) / (Length(fromStart) * Length(dir)));
                        if (isnan(angle))
                            angle = 0.0f;
                        group->sideDistance = fabsf(sinf(angle) * Length(fromStart));
                        f32 sideScale = 1.0f / (length + 0.0001f);
                        group->side.x = -(dir.y * sideScale);
                        group->side.y = dir.x * sideScale;
                        group->side.z = 0.0f;
                        fn_71007390d0(self, group, &start);
                        group->key = end;
                        group->otherArea = group->area != end.area;
                    }
                    fn_710073f7bc(controller, 0);
                }
            }
        } else if (self->mode == 2) {
            fn_710073f7bc(controller, 3);
        }
    }
    records.Free();
    return false;
}
