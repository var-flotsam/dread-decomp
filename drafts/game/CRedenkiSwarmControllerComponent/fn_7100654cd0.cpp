// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py, unit 977): sizes a group's slot list to its
// members, then places every active member on a circle around the group, aims its slot outwards, starts
// the billboard effect on it and picks the slot's state from how far it must turn. Names and layouts are
// placeholders.
#include <math.h>
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU2560.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::global::CRntVector;
using base::math::CVector3D;
using base::reflection::CClass;

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkStruct_71007455dc;
UnkVec3Result fn_71007455dc(UnkStruct_71007455dc* self);
u8 fn_71007452a4(UnkTarget* target);

extern char g_unk_710164a410[];
extern f32 g_unk_7101a5aed8;

// A slot's trailing settings, copied as a whole.
struct UnkRedenkiSlotState_l654cd0 {
    u8 field_0x18;
    u8 heading;  // 0x19: the angle the member faces, in 1/256 turns
    u8 pad_0x1a[2];
    f32 field_0x1c;
    u32 state;  // 0x20
    u32 field_0x24;
};

// A slot's point or direction, copied component by component.
struct UnkRedenkiVec_l654cd0 {
    f32 x;
    f32 y;
    f32 z;

    UnkRedenkiVec_l654cd0() = default;
    UnkRedenkiVec_l654cd0(const UnkRedenkiVec_l654cd0& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkRedenkiSlot_l654cd0 {
    UnkRedenkiVec_l654cd0 pos;
    CVector3D dir;  // 0xc
    UnkRedenkiSlotState_l654cd0 settings;  // 0x18

    UnkRedenkiSlot_l654cd0() = default;
    UnkRedenkiSlot_l654cd0(const UnkRedenkiSlot_l654cd0& o)
        : pos(o.pos), dir{o.dir.x, o.dir.y, o.dir.z}, settings(o.settings) {}
};
static_assert(sizeof(UnkRedenkiSlot_l654cd0) == 0x28, "UnkRedenkiSlot_l654cd0 layout");

UnkRedenkiSlot_l654cd0* fn_710065a7dc(CRntVector<UnkRedenkiSlot_l654cd0>* slots, u32 index);

struct UnkRedenkiMember_l654cd0 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
    f32 field_0x30;
    u8 pad_0x34[0x44 - 0x34];
    f32 radius;  // 0x44
    f32 angle;  // 0x48
    u8 pad_0x4c[0x164 - 0x4c];
    f32 field_0x164;
    u8 pad_0x168[0x170 - 0x168];
    u8 field_0x170;
    u8 pad_0x171[3];
    CVector3D home;  // 0x174
};

struct UnkRedenkiGroup_l654cd0 {
    u8 pad_0x0[0x20];
    u32 index;  // 0x20
    u8 pad_0x24[0xb8 - 0x24];
    UnkRedenkiMember_l654cd0** members;  // 0xb8
    u32 memberCount;  // 0xc0
};

struct UnkRedenkiKeyHolder_l654cd0 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkRedenkiBillboard_l654cd0 {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkRedenkiKeyHolder_l654cd0* field_0xd0;
};

struct UnkRedenkiChain2_l654cd0 {
    u8 pad_0x0[0x10];
    UnkRedenkiBillboard_l654cd0* field_0x10;
};

struct UnkRedenkiChain1_l654cd0 {
    u8 pad_0x0[0x10];
    UnkRedenkiChain2_l654cd0* field_0x10;
};

struct UnkRedenkiTuning_l654cd0 {
    u8 pad_0x0[0x458];
    f32 radiusScale;  // 0x458
};

struct UnkRedenkiController_l654cd0 {
    u8 pad_0x0[0xe8];
    UnkRedenkiChain1_l654cd0* field_0xe8;
    u8 pad_0xf0[0x488 - 0xf0];
    CRntVector<CRntVector<UnkRedenkiSlot_l654cd0>> slots;  // 0x488, one list per group
    u8 pad_0x4b0[0x560 - 0x4b0];
    UnkRedenkiTuning_l654cd0* tuning;  // 0x560
};
static_assert(__builtin_offsetof(UnkRedenkiController_l654cd0, tuning) == 0x560, "UnkRedenkiController_l654cd0 layout");

template <class V>
static inline f32 Length(const V& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// The angle between v and the x axis, both normalised with a small bias.
template <class V>
static inline f32 AngleToXAxis(const V& v) {
    f32 inv = 1.0f / (Length(v) + 0.0001f);
    f32 nx = v.x * inv;
    f32 ny = v.y * inv;
    f32 nz = v.z * inv;
    CVector3D axis = {1.0f, 0.0f, 0.0f};
    f32 axisInv = 1.0f / (Length(axis) + 0.0001f);
    f32 d = nx * (axis.x * axisInv) + ny * (axis.y * axisInv) + nz * (axis.z * axisInv);
    d = !(d >= 1.0f) ? d : 1.0f;
    d = !(d < -1.0f) ? d : -1.0f;
    return acosf(d);
}

// Grows the list to `n` slots (new ones zeroed) or shortens it.
static inline void ResizeSlots(CRntVector<UnkRedenkiSlot_l654cd0>& slots, u32 n) {
    if (slots.count < n) {
        slots.Reserve(n);
        for (u32 i = slots.count; i < n; i++)
            new (&slots.data[i]) UnkRedenkiSlot_l654cd0();
    } else if (slots.count <= n) {
        return;
    }
    slots.count = n;
}

void fn_7100654cd0(UnkRedenkiController_l654cd0* self, UnkRedenkiGroup_l654cd0* group, const CVector3D* facing) {
    CRntVector<UnkRedenkiSlot_l654cd0>* slots = &self->slots[group->index];
    ResizeSlots(*slots, group->memberCount);

    for (u64 i = 0; i < group->memberCount; i++) {
        UnkRedenkiMember_l654cd0* member = group->members[i];
        if (!fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member)))
            continue;
        UnkRedenkiSlot_l654cd0* slot = fn_710065a7dc(slots, i);
        UnkVec3Result home = fn_71007455dc(reinterpret_cast<UnkStruct_71007455dc*>(member));
        member->field_0x170 = fn_71007452a4(reinterpret_cast<UnkTarget*>(member));

        f32 radius = self->tuning->radiusScale * member->radius;
        f32 angle = member->angle;
        f32 baseX = member->field_0x2c;
        f32 baseY = member->field_0x30;
        f32 offsetX = radius * cosf(angle);
        f32 offsetY = radius * sinf(angle);
        {
            UnkVec3Result center = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(group));
            f32 x = offsetX + center.x;
            f32 y = offsetY + center.y;
            f32 z = 0.0f + center.z;
            slot->pos.x = x;
            slot->pos.y = y;
            slot->pos.z = z;
        }
        CVector3D dir = {offsetX - baseX, offsetY - baseY, 0.0f};
        slot->dir.x = dir.x;
        slot->dir.y = dir.y;
        slot->dir.z = dir.z;
        f32 distance = Length(dir);
        if (distance < 0.001f) {
            slot->dir = g_unk_710164779c;
        } else {
            slot->dir.x /= distance;
            slot->dir.y /= distance;
            slot->dir.z /= distance;
        }
        slot->settings.field_0x1c = 500.0f;

        f32 heading = AngleToXAxis(*facing);
        if (facing->y < 0.0f)
            heading = 6.2831855f - heading;
        slot->settings.heading = heading * 256.0f / 6.2831855f;
        member->field_0x164 = -100.0f;
        member->home.x = home.x;
        member->home.y = home.y;
        member->home.z = home.z;

        UnkRedenkiBillboard_l654cd0* obj = self->field_0xe8->field_0x10->field_0x10;
        UnkRedenkiBillboard_l654cd0* billboard =
            obj && fn_710009d50c(
                       reinterpret_cast<const CClass* (*)(UnkRedenkiBillboard_l654cd0*)>(obj->vtable[0])(obj),
                       fn_710072be34())
                ? obj
                : nullptr;
        void* manager = g_unk_71726da048;
        if (!manager) {
            manager = fn_71000839a0(0xe0, 0x3f);
            fn_7100e1bca4(manager);
            g_unk_71726da048 = manager;
        }
        u64 key = billboard->field_0xd0->field_0x30;
        fn_7100745440(reinterpret_cast<UnkTarget*>(member), fn_7100e1cb50(manager, &key, g_unk_710164a410), 0);
        slot->settings.state = 0;
        slot->settings.field_0x24 = 6;

        f32 start = fn_71007452a4(reinterpret_cast<UnkTarget*>(member)) * 6.2831855f / 256.0f;
        f32 turn = fn_710007a1d0(start - AngleToXAxis(slot->dir));
        if (fabsf(turn) > 1.5707964f)
            slot->settings.state = distance < g_unk_7101a5aed8 ? 1 : 2;
    }
}
