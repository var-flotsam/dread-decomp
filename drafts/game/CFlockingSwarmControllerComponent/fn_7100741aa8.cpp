// CFlockingSwarmControllerComponent's unit (tools/tu_map.py 1158): the update step. Counts down the
// lifetimes of the recorded positions, dropping each one that runs out, advances the clocks, then
// refreshes every member's position and target before steering them (fn_710074124c).
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::math::CVector3D;

template <class T>
struct UnkFrameVector;

f32* fn_71000d4a50(UnkFrameVector<f32>* v, u32 i);

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkVec3_cs {
    f32 x, y, z;
};

struct UnkSwarmMember_l741aa8 {
    u8 pad_0x0[0x148];
};

struct UnkSwarmClassData_l741aa8 {
    u8 pad_0x0[0x50];
    UnkSwarmMember_l741aa8* members;  // 0x50
    u8 pad_0x58[0xb8];
    s32 member_count;  // 0x110
};

struct UnkSwarmClassHolder_l741aa8 {
    u8 pad_0x0[0x10];
    UnkSwarmClassData_l741aa8* data;  // 0x10
};

struct UnkSwarmCtrl_l741aa8 {
    void** vtable;
    u8 pad_0x8[0x88];
    base::global::CRntVector<CVector3D> positions;  // 0x90
    base::global::CRntVector<f32> lifetimes;        // 0xb8
    u8 pad_0xe0[0x8];
    UnkSwarmClassHolder_l741aa8* klass;  // 0xe8
    u8 pad_0xf0[0x1bc];
    f32 time;  // 0x2ac
    u8 pad_0x2b0[0x84];
    f32 cooldown;  // 0x334
    u8 pad_0x338[0x8];
    base::global::CRntVector<UnkVec3_cs> member_positions;  // 0x340
    base::global::CRntVector<UnkVec3_cs> member_targets;    // 0x368

    void Step(f32 dt) { reinterpret_cast<void (*)(UnkSwarmCtrl_l741aa8*, f32)>(vtable[0x250 / 8])(this, dt); }
};
static_assert(__builtin_offsetof(UnkSwarmCtrl_l741aa8, time) == 0x2ac, "layout");
static_assert(__builtin_offsetof(UnkSwarmCtrl_l741aa8, member_targets) == 0x368, "layout");

void fn_710074124c(UnkSwarmCtrl_l741aa8* self, base::global::CRntVector<UnkVec3_cs>* positions,
                   base::global::CRntVector<UnkVec3_cs>* targets);

// Moves the later positions down one over the one at `index`.
static inline void ErasePosition(base::global::CRntVector<CVector3D>* v, u32 index) {
    if (index < v->count) {
        for (u64 i = index; i < v->count - 1; ++i)
            v->data[i] = v->data[i + 1];
        v->count = v->count - 1;
    }
}

static inline f32& LifetimeAt(UnkSwarmCtrl_l741aa8* self, u32 i) {
    return *fn_71000d4a50(reinterpret_cast<UnkFrameVector<f32>*>(&self->lifetimes), i);
}

void fn_7100741aa8(UnkSwarmCtrl_l741aa8* self, f32 dt) {
    s32 member_count = self->klass->data->member_count;

    // From the last recorded position down to the first.
    u32 i = self->positions.count;
    while ((s32)--i >= 0) {
        LifetimeAt(self, i) -= dt;
        if (!(LifetimeAt(self, i) <= 0.0f))
            continue;
        ErasePosition(&self->positions, i);
        self->lifetimes.EraseAt(i);
    }

    self->time += dt;
    self->Step(dt);
    if (self->cooldown > 0.0f)
        self->cooldown -= dt;

    for (s32 i = 0; i < member_count; ++i) {
        UnkSwarmClassData_l741aa8* data = self->klass->data;
        if (i < data->member_count && data->members) {
            UnkVec3Result position = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(&data->members[i]));
            UnkVec3_cs* p = fn_71000cc374(&self->member_positions, i);
            p->x = position.x;
            p->y = position.y;
            p->z = position.z;
            UnkVec3_cs* t = fn_71000cc374(&self->member_targets, i);
            t->x = g_unk_710164779c.x;
            t->y = g_unk_710164779c.y;
            t->z = g_unk_710164779c.z;
        }
    }

    fn_710074124c(self, &self->member_positions, &self->member_targets);
}
