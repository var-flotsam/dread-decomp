// CFlockingSwarmControllerComponent unit (tools/tu_map.py unit 1158): fn_710073fea8, the nearest free swarm
// point to `pos`: the groups sorted by how far `pos` lies outside their bounds, then in each the member
// nearest `pos` (farther than `range` when it is not negative), the first one the controller accepts.
// Names and layouts are placeholders.
// This unit has the vector's element accessor inlined.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "types.h"

using base::math::CVector3D;

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkSwarmPoint_b65a4 {
    u64 value;
};

struct UnkSwarmBounds_l73fea8 {
    CVector3D min;
    CVector3D max;
};

struct UnkSwarmMember_l73fea8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* GetPosition();  // 0x18
};

struct UnkSwarmGroup_l73fea8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Count();                                    // 0x28
    virtual UnkSwarmMember_l73fea8* Get(u32 index);         // 0x30
    virtual const UnkSwarmBounds_l73fea8* GetBounds();      // 0x38
};

struct UnkSwarmGroupList_l73fea8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Count();                            // 0x28
    virtual UnkSwarmGroup_l73fea8* Get(u32 index);  // 0x30
};

struct UnkFlockingSwarm_l73fea8 {
    void** vtable;
    u8 pad_0x8[0x78];
    UnkSwarmGroupList_l73fea8* groups;  // 0x80
    u8 pad_0x88[0x2c8 - 0x88];
    u8 field_0x2c8[8];

    // vf_0x2c0: whether the point at `pos` can be taken.
    bool Accepts(const CVector3D* pos) {
        return reinterpret_cast<bool (*)(UnkFlockingSwarm_l73fea8*, const CVector3D*, void*, bool)>(
            vtable[0x2c0 / 8])(this, pos, field_0x2c8, false);
    }
};

// A group's distance from the point and its index, sorted by distance (fn_7100744000).
struct UnkSwarmDist_l73fea8 {
    f32 dist;
    u32 index;

    UnkSwarmDist_l73fea8() = default;
    UnkSwarmDist_l73fea8(f32 d, u32 i) : dist(d), index(i) {}
    ~UnkSwarmDist_l73fea8() {}
};

struct UnkSwarmDistLess_l73fea8 {};

void fn_7100744000(UnkSwarmDist_l73fea8* first, UnkSwarmDist_l73fea8* last, UnkSwarmDistLess_l73fea8& less);

// How far v lies outside [lo, hi].
static inline f32 Outside(f32 v, f32 lo, f32 hi) {
    if (v > hi)
        return v - hi;
    if (v < lo)
        return lo - v;
    return 0.0f;
}

static inline f32 DistanceSq(const CVector3D* a, const UnkVec3Result* b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    return dx * dx + dy * dy + dz * dz;
}

bool fn_710073fea8(UnkSwarmUpdate_b65a4* update, const UnkVec3Result* pos, UnkSwarmPoint_b65a4* out, f32 range) {
    UnkFlockingSwarm_l73fea8* self = reinterpret_cast<UnkFlockingSwarm_l73fea8*>(update);
    u32 n = self->groups->Count();
    base::global::CRntVector<UnkSwarmDist_l73fea8> order(0xf);
    for (u32 i = 0; i < n; ++i) {
        const UnkSwarmBounds_l73fea8* bounds = self->groups->Get(i)->GetBounds();
        f32 dx = Outside(pos->x, bounds->min.x, bounds->max.x);
        f32 dy = Outside(pos->y, bounds->min.y, bounds->max.y);
        f32 dz = Outside(pos->z, bounds->min.z, bounds->max.z);
        order.EmplaceBack(UnkSwarmDist_l73fea8(dx * dx + dy * dy + dz * dz, i));
    }
    UnkSwarmDistLess_l73fea8 less;
    fn_7100744000(order.begin(), order.end(), less);

    f32 min_sq = range < 0.0f ? -1.0f : range * range;
    bool found = false;
    for (u32 j = 0; j < n; ++j) {
        u32 group_index = order.GrowAt(j).index;
        UnkSwarmGroup_l73fea8* group = self->groups->Get(group_index);
        u32 count = group->Count();
        UnkSwarmMember_l73fea8* best = group->Get(0);
        f32 best_dist = DistanceSq(best->GetPosition(), pos);
        u32 best_index = 0;
        for (u32 k = 1; k < count; ++k) {
            UnkSwarmMember_l73fea8* member = group->Get(k);
            f32 dist = DistanceSq(member->GetPosition(), pos);
            // A negative minimum means none.
            if (dist < best_dist && (min_sq < 0.0f || dist > min_sq)) {
                best_dist = dist;
                best_index = k;
                best = member;
            }
        }
        if (self->Accepts(best->GetPosition())) {
            out->value = group_index | (u64)best_index << 32;
            found = true;
            break;
        }
    }
    order.Free();
    return found;
}
