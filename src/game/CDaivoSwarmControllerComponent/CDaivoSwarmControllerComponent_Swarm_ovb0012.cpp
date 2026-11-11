// CDaivoSwarmControllerComponent's unit (tools/tu_map.py unit 768): the state change on the flag at +0x554
// (fn_71004e25a0), a pass over every group's members (fn_71004e10dc), the move along the target's
// heading (fn_71004e2494), the members' colour fades (fn_71004e081c), the destructor (fn_71004e2630) and a
// navigation-mesh probe (fn_71004e05d8).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

using base::color::CColor4F;

extern "C" f32 sqrtf(f32);
extern "C" int rand();

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

// The object the swarm follows: its velocity at +0x70.
struct UnkSwarmTarget_b0012 {
    u8 pad_0x0[0x70];
    f32 velocity[3];  // 0x70
};

struct UnkSwarmMember_b0012;

// A group of members (0x148 bytes).
struct UnkSwarmGroup_b0012 {
    u8 pad_0x0[0x4c];
    u32 field_0x4c;
    u8 pad_0x50[0xb8 - 0x50];
    UnkSwarmMember_b0012** members;  // 0xb8
    u32 memberCount;  // 0xc0
    u8 pad_0xc4[0x148 - 0xc4];

    UnkSwarmMember_b0012** begin() { return members; }
    UnkSwarmMember_b0012** end() { return members + memberCount; }
};

struct UnkSwarmGroupList_b0012 {
    u8 pad_0x0[0x50];
    UnkSwarmGroup_b0012* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    // The group at `i`, or null past the end.
    UnkSwarmGroup_b0012* At(s64 i) {
        if (i >= count)
            return nullptr;
        return groups ? &groups[i] : nullptr;
    }
};

struct UnkSwarmData_b0012 {
    u8 pad_0x0[0x10];
    UnkSwarmGroupList_b0012* groupList;  // 0x10
};

struct UnkDaivoTuning_b0012 {
    u8 pad_0x0[0x458];
    f32 field_0x458;
    u8 pad_0x45c[0x484 - 0x45c];
    f32 field_0x484;
};

struct UnkSwarmPath_b0012 {
    u8 data[0x8];
};

struct UnkZeroVec_b0012 {
    f32 x, y, z;
    UnkZeroVec_b0012() : x(0.0f), y(0.0f), z(0.0f) {}
};

class UnkDaivoSwarm_b0012 {
public:
    // Slots 0x0-0x2b8 (88 entries).
    UNK_VF8(0) UNK_VF8(1) UNK_VF8(2) UNK_VF8(3) UNK_VF8(4) UNK_VF8(5) UNK_VF8(6) UNK_VF8(7)
    UNK_VF8(8) UNK_VF8(9) UNK_VF8(10)
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual void vf_0x2d0();
    virtual void vf_0x2d8();
    virtual void vf_0x2e0();
    virtual void vf_0x2e8(u8 state, bool arg);

    u8 pad_0x8[0xe8 - 0x8];
    UnkSwarmData_b0012* data;  // 0xe8
    u8 pad_0xf0[0x2c8 - 0xf0];
    UnkSwarmPath_b0012 path;  // 0x2c8
    u8 pad_0x2d0[0x395 - 0x2d0];
    bool field_0x395;
    u8 pad_0x396[0x554 - 0x396];
    bool field_0x554;
    u8 pad_0x555[0x570 - 0x555];
    UnkDaivoTuning_b0012* tuning;  // 0x570
    u8 pad_0x578[0x588 - 0x578];
    bool field_0x588;
};

#undef UNK_VF8

struct UnkParent_71007455dc;
struct UnkSwarmHold_b66a4;
struct UnkSwarmUpdate_b65a4;
// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_Hold_b66a4.cpp declares it.
// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_Update_b65a4.cpp declares them.
UnkVec3Result fn_7100745e24(UnkParent_71007455dc* parent);
void fn_710074dda0(UnkDaivoSwarm_b0012* self, UnkSwarmMember_b0012* member);
void fn_7100656f3c(UnkDaivoSwarm_b0012* self, UnkSwarmPath_b0012* path, const UnkVec3Result* dir, const f32* speed,
                   bool* out, const UnkZeroVec_b0012* offset, f32 radius, f32 limit);

static inline UnkVec3Result Position(UnkDaivoSwarm_b0012* self) {
    return fn_710064fef8(reinterpret_cast<UnkSwarmHold_b66a4*>(self));
}

static inline UnkSwarmTarget_b0012* Target(UnkDaivoSwarm_b0012* self) {
    return reinterpret_cast<UnkSwarmTarget_b0012*>(fn_7100739c64(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self)));
}

static inline UnkVec3Result TargetPosition(UnkSwarmTarget_b0012* target) {
    return fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(target));
}

// With fn_710065634c's go-ahead, the state change (vf_0x2e8) to 4 or 0 by the flag at +0x554.
void fn_71004e25a0(UnkDaivoSwarm_b0012* self) {
    if (fn_710065634c(self))
        self->vf_0x2e8(self->field_0x554 ? 4 : 0, false);
}

// With the flag at +0x588 set, every member of every group whose +0x4c is clear goes through
// fn_710074dda0.
void fn_71004e10dc(UnkDaivoSwarm_b0012* self) {
    if (!self->field_0x588)
        return;
    s32 count = self->data->groupList->count;
    for (s32 i = 0; i < count; i++) {
        UnkSwarmGroup_b0012* group = self->data->groupList->At(i);
        if (group->field_0x4c != 0)
            continue;
        for (UnkSwarmMember_b0012* member : *group)
            fn_710074dda0(self, member);
    }
}

// Sends the swarm (fn_7100656f3c) along the target's normalised velocity at the tuned speed (+0x484),
// within twice the tuned range (+0x458), and marks it moving (+0x395). The positions it asks for
// along the way are not used.
void fn_71004e2494(UnkDaivoSwarm_b0012* self) {
    UnkSwarmTarget_b0012* target = Target(self);
    const f32* v = target->velocity;
    f32 inv = 1.0f / (sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) + 0.0001f);
    f32 x = v[0] * inv;
    f32 y = v[1] * inv;
    f32 z = inv * v[2];
    TargetPosition(target);
    UnkVec3Result dir(x, y, z);
    f32 radius = self->tuning->field_0x458;
    f32 speed = self->tuning->field_0x484;
    f32 range = radius + radius;
    UnkVec3Result pos = Position(self);
    TargetPosition(target);
    UnkZeroVec_b0012 offset;
    fn_7100656f3c(self, &self->path, &dir, &speed, &self->field_0x554, &offset, range, radius);
    self->field_0x395 = true;
}

// ---- fn_71004e081c: the members' colour fades ----

// As src/game/UnkColorFade1159/UnkColorFade1159.cpp defines it.
struct UnkColorFade1159;
void fn_71007453a8(UnkColorFade1159* self, const base::color::CColor4F* from, const base::color::CColor4F* to,
                   bool mark_dirty, bool flag, f32 time);
// As src/game/CRinkaUnitComponent/CRinkaUnitComponent_b39.cpp declares it.
extern const base::math::CVector4D g_unk_7101647634;

struct UnkSwarmFade_b0012 {
    u8 pad_0x0[0x118];
    CColor4F color;  // 0x118
    u8 pad_0x128[0x154 - 0x128];
    f32 field_0x154;
};

struct UnkFadeGroup_b0012 {
    u8 pad_0x0[0xb8];
    UnkSwarmFade_b0012** members;  // 0xb8
    u32 memberCount;  // 0xc0

    UnkSwarmFade_b0012* At(u64 i) { return i < memberCount ? members[i] : nullptr; }
};

struct UnkFadeTuning_b0012 {
    u8 pad_0x0[0xf0];
    f32 maxTime;  // 0xf0
    f32 minTime;  // 0xf4
};

struct UnkDaivoFade_b0012 {
    u8 pad_0x0[0x570];
    UnkFadeTuning_b0012* tuning;  // 0x570
};

void fn_710064f42c(UnkDaivoFade_b0012* self, UnkFadeGroup_b0012* group);

static inline f32 Random01() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

// After fn_710064f42c, every member of the group fades from its colour (+0x118) to the colour
// g_unk_7101647634 over a random time between the tuned bounds, kept at +0x154.
void fn_71004e081c(UnkDaivoFade_b0012* self, UnkFadeGroup_b0012* group) {
    fn_710064f42c(self, group);
    u32 count = group->memberCount;
    if (count == 0)
        return;
    f32 maxTime = self->tuning->maxTime;
    f32 minTime = self->tuning->minTime;
    for (u64 i = 0; i < count; i++) {
        f32 time = minTime + (maxTime - minTime) * Random01();
        UnkSwarmFade_b0012* member = group->At(i);
        CColor4F from = member->color;
        CColor4F to = reinterpret_cast<const CColor4F&>(g_unk_7101647634);
        member->field_0x154 = time;
        fn_71007453a8(reinterpret_cast<UnkColorFade1159*>(member), &from, &to, false, false, time);
    }
}

// ---- fn_71004e2630: the destructor ----

// An array that frees itself when destroyed.
template <class T>
struct UnkOwnedVector_b0012 : base::UnkVector<T> {
    ~UnkOwnedVector_b0012() { this->Free(); }
};

struct UnkDaivoLists_b0012 {
    u8 pad_0x0[0x488];
    base::UnkVector<UnkOwnedVector_b0012<u32>> field_0x488;
    u8 pad_0x4b0[0x28];
    base::UnkVector<u32> field_0x4d8;
    base::UnkVector<UnkOwnedVector_b0012<u32>> field_0x500;
};
static_assert(__builtin_offsetof(UnkDaivoLists_b0012, field_0x500) == 0x500, "UnkDaivoLists_b0012 layout");

void fn_7100737f34(UnkDaivoLists_b0012* self);

// The vtable, the three arrays (last first, the nested ones freeing their elements' arrays), then the
// base's destructor.
void fn_71004e2630(UnkDaivoLists_b0012* self) {
    *reinterpret_cast<void**>(self) = g_unk_7101a5af00 + 0x10;
    self->field_0x500.Free();
    self->field_0x4d8.Free();
    self->field_0x488.Free();
    fn_7100737f34(self);
}

// ---- fn_71004e05d8: the navigation-mesh probe ----

// As src/game_hi/fn_7100ebfa7c.cpp defines it (its point and result types are local there).
struct Vec2;
struct Result;
bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b);

struct UnkNavPoint_b0012 {
    f32 x;
    f32 y;
    UnkNavPoint_b0012(f32 x_, f32 y_) : x(x_), y(y_) {}
};

// A segment with its direction (end - start).
struct UnkNavSegment_b0012 {
    UnkNavPoint_b0012 start;
    UnkNavPoint_b0012 end;
    UnkNavPoint_b0012 delta;
    UnkNavSegment_b0012(const UnkNavPoint_b0012& s, const UnkNavPoint_b0012& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

// fn_7100ebfa7c's result: no cell (0xffff), no side (0xff), points cleared.
struct UnkNavResult_b0012 {
    u16 cell;
    u8 side;
    UnkNavPoint_b0012 points[5];
    f32 distance;
    UnkNavResult_b0012()
        : cell(0xffff), side(0xff),
          points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, distance(0.0f) {}
};

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// Whether the navigation mesh finds nothing (fn_7100ebfa7c) along the 300-unit segment from `pos`
// in direction `dir`.
bool fn_71004e05d8(const UnkNavPoint_b0012* pos, const UnkNavPoint_b0012* dir) {
    f32 x = dir->x * 300.0f;
    f32 y = dir->y * 300.0f;
    UnkNavPoint_b0012 start(pos->x, pos->y);
    UnkNavPoint_b0012 end(x + start.x, y + start.y);
    UnkNavSegment_b0012 segment(start, end);
    UnkNavResult_b0012 result;
    return !fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&result),
                          40.0f, 0, -1.0f);
}
