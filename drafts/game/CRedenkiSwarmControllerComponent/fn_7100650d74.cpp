// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py, unit 977): while the swarm is larger than its
// limit (or its timer runs), drifts every group along and across the line between two points; otherwise
// gathers the members that ask for it and switches state. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU1161.h"
#include "types.h"

using base::math::CVector3D;

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkBillboardEntry_be;
void fn_7100745e74(UnkBillboardEntry_be* self, const CVector3D* target, bool keep_items);

struct UnkRedenkiController_b55;
struct UnkTarget;
void fn_7100655280(UnkRedenkiController_b55* self, UnkTarget* target);

extern f32 g_unk_7101a5aee0;
extern u32 g_unk_71726fc5ac;

struct UnkDriftVec_l650d74 {
    f32 x;
    f32 y;
    f32 z;
};

static inline UnkDriftVec_l650d74 operator-(const UnkDriftVec_l650d74& a, const UnkDriftVec_l650d74& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
static inline UnkDriftVec_l650d74 operator+(const UnkDriftVec_l650d74& a, const UnkDriftVec_l650d74& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
static inline UnkDriftVec_l650d74 operator*(const UnkDriftVec_l650d74& v, f32 s) {
    return {v.x * s, v.y * s, v.z * s};
}
static inline UnkDriftVec_l650d74 operator*(f32 s, const UnkDriftVec_l650d74& v) {
    return {s * v.x, s * v.y, s * v.z};
}
static inline f32 Dot(const UnkDriftVec_l650d74& a, const UnkDriftVec_l650d74& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct UnkRedenkiMember_l650d74 {
    u8 pad_0x0[0x180];
    u8 field_0x180;  // wants to be gathered
};

struct UnkRedenkiGroup_l650d74 {
    u8 pad_0x0[0xb8];
    UnkRedenkiMember_l650d74** members;  // 0xb8
    u32 memberCount;  // 0xc0
    u8 pad_0xc4[0xe0 - 0xc4];
    s32 size;  // 0xe0
    u8 pad_0xe4[0x148 - 0xe4];
};

struct UnkRedenkiGroupList_l650d74 {
    u8 pad_0x0[0x50];
    UnkRedenkiGroup_l650d74* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    UnkRedenkiGroup_l650d74* At(s64 i) {
        if (i >= count)
            return nullptr;
        return groups ? &groups[i] : nullptr;
    }
};

struct UnkRedenkiData_l650d74 {
    u8 pad_0x0[0x10];
    UnkRedenkiGroupList_l650d74* list;  // 0x10
};

struct UnkRedenkiTuning_l650d74 {
    u8 pad_0x0[0x4a4];
    bool drift;  // 0x4a4
    u8 pad_0x4a5[0x4ac - 0x4a5];
    f32 alongScale;  // 0x4ac
    f32 acrossScale;  // 0x4b0
};

struct UnkRedenkiController_l650d74 {
    void** vtable;
    u8 pad_0x8[0xe8 - 0x8];
    UnkRedenkiData_l650d74* data;  // 0xe8
    u8 pad_0xf0[0x338 - 0xf0];
    u32 field_0x338;
    u8 pad_0x33c[0x394 - 0x33c];
    u8 field_0x394;
    u8 pad_0x395[0x468 - 0x395];
    s32 limit;  // 0x468
    u8 pad_0x46c[0x4b0 - 0x46c];
    UnkDriftVec_l650d74 direction;  // 0x4b0
    UnkDriftVec_l650d74 from;  // 0x4bc
    UnkDriftVec_l650d74 to;  // 0x4c8
    u8 pad_0x4d4[0x55c - 0x4d4];
    f32 timer;  // 0x55c
    UnkRedenkiTuning_l650d74* tuning;  // 0x560

    void SetState(u32 a, u32 b) {
        reinterpret_cast<void (*)(UnkRedenkiController_l650d74*, u32, u32)>(vtable[0x2e8 / 8])(this, a, b);
    }
};
static_assert(__builtin_offsetof(UnkRedenkiController_l650d74, tuning) == 0x560, "UnkRedenkiController_l650d74 layout");

void fn_7100650d74(UnkRedenkiController_l650d74* self, f32 dt) {
    UnkRedenkiGroupList_l650d74* list = self->data->list;
    s32 n = list->count;
    s32 total = 0;
    for (s32 i = 0; i < n; i++)
        total += list->groups[i].size;

    s32 limit = self->limit;
    if (limit < total || (self->tuning->drift && !(self->timer <= 0.0f))) {
        if (total > limit * 2 || !self->tuning->drift)
            return;
        self->timer -= dt;
        for (s32 i = 0; i < n; i++) {
            UnkRedenkiGroup_l650d74* group = self->data->list->At(i);
            UnkVec3Result p = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(group));
            UnkDriftVec_l650d74 from = self->from;
            UnkDriftVec_l650d74 to = self->to;
            UnkDriftVec_l650d74 velocity;
            if (!(__builtin_fabsf(to.x - from.x) + __builtin_fabsf(to.y - from.y) + __builtin_fabsf(to.z - from.z) <= 0.1f)) {
                UnkDriftVec_l650d74 axis = to - from;
                UnkDriftVec_l650d74 rel = {p.x - from.x, p.y - from.y, p.z - from.z};
                UnkDriftVec_l650d74 side = {-axis.y, axis.x, 0.0f};
                f32 along = Dot(axis, rel) / Dot(axis, axis);
                f32 across = Dot(side, rel) / Dot(side, side);
                UnkRedenkiTuning_l650d74* tuning = self->tuning;
                UnkDriftVec_l650d74 alongMove = axis * along * (tuning->alongScale - 1.0f);
                UnkDriftVec_l650d74 acrossMove = side * across * (tuning->acrossScale - 1.0f);
                velocity = acrossMove + alongMove;
            } else {
                velocity = g_unk_7101a5aee0 * self->direction * (self->tuning->alongScale - 1.0f) * -3.0f;
            }
            UnkDriftVec_l650d74 step = velocity * dt;
            CVector3D target;
            target.x = step.x + p.x;
            target.y = step.y + p.y;
            target.z = step.z + p.z;
            fn_7100745e74(reinterpret_cast<UnkBillboardEntry_be*>(group), &target, true);
        }
    } else {
        for (s32 i = 0; i < n; i++) {
            UnkRedenkiGroup_l650d74* group = self->data->list->At(i);
            for (u64 j = 0; j < group->memberCount; j++) {
                UnkRedenkiMember_l650d74* member = group->members[j];
                if (fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member)) && member->field_0x180)
                    fn_7100655280(reinterpret_cast<UnkRedenkiController_b55*>(self), reinterpret_cast<UnkTarget*>(member));
            }
        }
        self->SetState(2, 0);
        self->field_0x338 = g_unk_71726fc5ac;
        self->field_0x394 = 1;
    }
}
