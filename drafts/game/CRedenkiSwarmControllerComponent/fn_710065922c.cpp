// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): one member's step in its cell of its group's
// list: by the cell's state it flies to the cell, turns, darts or waits, then turns its heading toward the result
// by at most a full turn per second. Names and layouts are placeholders.
#include <math.h>
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU2560.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::CClass;

// The layout of a base::global::CRntVector, freed in place.
struct UnkRedenkiSubList_ovb0084 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::IAllocator* allocator;

    void Free() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// An array of those lists; growing it is out of line.
struct UnkRedenkiSubLists_ovb0084 {
    UnkRedenkiSubList_ovb0084* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::IAllocator* allocator;
};

struct UnkRedenkiKeyHolder_ovb0084 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkRedenkiBillboard_ovb0084 {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkRedenkiKeyHolder_ovb0084* field_0xd0;
};

struct UnkRedenkiChain2_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiBillboard_ovb0084* field_0x10;
};

struct UnkRedenkiChain1_ovb0084 {
    u8 pad_0x0[0x10];
    UnkRedenkiChain2_ovb0084* field_0x10;
};

struct UnkTarget;
struct UnkOffsetObj_1159_l7452d0;
struct UnkStruct_71007455dc;
struct UnkRedenkiFxCtrl_ovb0084;
struct UnkRedenkiDart_ovb0084;
struct UnkRedenkiCell_ovb0084;
struct UnkRedenkiCellArray_ovb0084;

u8 fn_71007452a4(UnkTarget* target);
void fn_71007456fc(UnkTarget* target, u8 value);
UnkVec3f fn_71007452d0(const UnkOffsetObj_1159_l7452d0* self);
void fn_710074566c(UnkStruct_71007455dc* self, const f32* pos);
void fn_710065b1b0(UnkRedenkiSubLists_ovb0084* self, u32 n);
void fn_710065a37c(UnkRedenkiFxCtrl_ovb0084* self, UnkRedenkiDart_ovb0084* member);
UnkRedenkiCell_ovb0084* fn_710065a7dc(UnkRedenkiCellArray_ovb0084* self, u32 index);
extern char g_unk_710164a410[];

// A member's cell: where it flies to, its heading there (a full turn is 256), its speed and its state.
struct UnkRedenkiSwarmCell_l65922c {
    f32 x;
    f32 y;
    f32 z;
    u8 pad_0xc[0xd];
    u8 heading;  // 0x19
    u8 pad_0x1a[0x2];
    f32 speed;      // 0x1c
    u32 state;      // 0x20
    u32 prevState;  // 0x24
};

struct UnkRedenkiSlotRef_l65922c {
    u8 pad_0x0[0x20];
    u32 group;
};

struct UnkRedenkiMember_l65922c {
    u8 pad_0x0[0x10];
    UnkRedenkiSlotRef_l65922c* field_0x10;
    u8 pad_0x18[0x8];
    u32 slot;  // 0x20
    u8 pad_0x24[0x164 - 0x24];
    f32 acceleration;  // 0x164
    u8 pad_0x168[0x180 - 0x168];
    bool field_0x180;
};

struct UnkRedenkiSteerCtrl_l65922c {
    u8 pad_0x0[0xe8];
    UnkRedenkiChain1_ovb0084* field_0xe8;
    u8 pad_0xf0[0x468 - 0xf0];
    u32 field_0x468;
    u8 pad_0x46c[0x488 - 0x46c];
    UnkRedenkiSubLists_ovb0084 lists;  // 0x488
};

static const f32 kTwoPi = 6.2831855f;

// The list at `index`, the array grown to it first when it may grow.
static inline UnkRedenkiSubList_ovb0084* ListAt(UnkRedenkiSubLists_ovb0084* lists, u32 index) {
    if (lists->growable && lists->count <= index) {
        if (lists->capacity <= index && lists->step != 0) {
            u32 grown = (index / lists->step + 1) * lists->step;
            if (lists->capacity < grown) {
                UnkRedenkiSubList_ovb0084* p = static_cast<UnkRedenkiSubList_ovb0084*>(lists->allocator->vf_0x40(
                    (u64)grown * sizeof(UnkRedenkiSubList_ovb0084), 8, lists->flags));
                if (lists->data) {
                    UnkRedenkiSubList_ovb0084* src = lists->data;
                    UnkRedenkiSubList_ovb0084* dst = p;
                    for (u32 i = 0; i < lists->count; ++i, ++dst, ++src) {
                        dst->data = src->data;
                        dst->count = src->count;
                        dst->capacity = src->capacity;
                        dst->step = src->step;
                        dst->growable = src->growable;
                        dst->flags = src->flags;
                        dst->allocator = src->allocator;
                        // The moved-from list is cleared as raw bytes (its array, count and capacity), as
                        // fn_710065b1b0 does (base::refl_ResizeSubVecs).
                        __builtin_memset(src, 0, 16);
                    }
                    lists->allocator->vf_0x48(lists->data);
                }
                lists->data = p;
                lists->capacity = grown;
            }
        }
        fn_710065b1b0(lists, index + 1);
    }
    return &lists->data[index];
}

static inline UnkTarget* AsTarget(UnkRedenkiMember_l65922c* member) {
    return reinterpret_cast<UnkTarget*>(member);
}

static inline f32 Length(f32 x, f32 y, f32 z) {
    return sqrtf(x * x + y * y + z * z);
}

static inline f32 Clamp(f32 v, f32 lo, f32 hi) {
    v = !(v >= hi) ? v : hi;
    return !(v < lo) ? v : lo;
}

// The heading of the direction (x, y, z) from the x axis, a full turn being 256.
static inline f32 Heading(f32 x, f32 y, f32 z) {
    f32 inv = 1.0f / (Length(x, y, z) + 0.0001f);
    f32 axisInv = 1.0f / (Length(1.0f, 0.0f, 0.0f) + 0.0001f);
    f32 cosine = x * inv * (1.0f * axisInv) + y * inv * (0.0f * axisInv) + z * inv * (0.0f * axisInv);
    f32 heading = acosf(Clamp(cosine, -1.0f, 1.0f)) * 256.0f / kTwoPi;
    if (y < 0.0f)
        heading = 256.0f - heading;
    return heading;
}

static inline f32 ToRadians(u8 heading) {
    return heading * kTwoPi / 256.0f;
}

// A heading difference brought into [-128, 128].
static inline f32 WrapDelta(f32 delta) {
    if (delta > 128.0f)
        delta -= 256.0f;
    else if (delta < -128.0f)
        delta += 256.0f;
    return delta;
}

void fn_710065922c(UnkRedenkiSteerCtrl_l65922c* self, UnkRedenkiMember_l65922c* member, f32 dt) {
    UnkRedenkiSlotRef_l65922c* ref = member->field_0x10;
    u8 heading = fn_71007452a4(AsTarget(member));
    UnkRedenkiSubList_ovb0084* list = ListAt(&self->lists, ref->group);
    f32 angle = heading;
    UnkRedenkiSwarmCell_l65922c* cell = reinterpret_cast<UnkRedenkiSwarmCell_l65922c*>(
        fn_710065a7dc(reinterpret_cast<UnkRedenkiCellArray_ovb0084*>(list), member->slot));

    bool entered = false;
    if (cell->prevState != cell->state) {
        entered = true;
        cell->prevState = cell->state;
    }

    switch (cell->state) {
    case 0: {
        UnkVec3f pos = fn_71007452d0(reinterpret_cast<const UnkOffsetObj_1159_l7452d0*>(member));
        f32 dx = cell->x - pos.x;
        f32 dy = cell->y - pos.y;
        f32 dz = cell->z - pos.z;
        f32 speed = cell->speed + member->acceleration * dt;
        cell->speed = !(speed < 300.0f) ? speed : 300.0f;
        f32 ratio = cell->speed * dt / Length(dx, dy, dz);
        f32 t = !(ratio >= 1.0f) ? ratio : 1.0f;
        pos.x += dx * t;
        pos.y += dy * t;
        pos.z += dz * t;
        fn_710074566c(reinterpret_cast<UnkStruct_71007455dc*>(member), &pos.x);
        angle = Heading(dx, dy, dz);
        if (ratio >= 1.0f) {
            f32 turn = fn_710007a1d0(ToRadians(fn_71007452a4(AsTarget(member))) - ToRadians(cell->heading));
            cell->state = fabsf(turn) > kTwoPi / 4.0f ? 4 : 3;
        }
        break;
    }
    case 1: {
        UnkVec3f pos = fn_71007452d0(reinterpret_cast<const UnkOffsetObj_1159_l7452d0*>(member));
        f32 dx = cell->x - pos.x;
        f32 dy = cell->y - pos.y;
        f32 dz = cell->z - pos.z;
        f32 speed = cell->speed + member->acceleration * dt;
        cell->speed = speed;
        f32 ratio = speed * dt / Length(dx, dy, dz);
        f32 t = ratio < 1.0f ? ratio : 1.0f;
        pos.x += dx * t;
        pos.y += dy * t;
        pos.z += dz * t;
        fn_710074566c(reinterpret_cast<UnkStruct_71007455dc*>(member), &pos.x);
        if (ratio >= 1.0f)
            cell->state = 3;
        angle = cell->heading;
        break;
    }
    case 2: {
        if (entered)
            fn_710065a37c(reinterpret_cast<UnkRedenkiFxCtrl_ovb0084*>(self),
                          reinterpret_cast<UnkRedenkiDart_ovb0084*>(member));
        UnkVec3f pos = fn_71007452d0(reinterpret_cast<const UnkOffsetObj_1159_l7452d0*>(member));
        f32 dx = cell->x - pos.x;
        f32 dy = cell->y - pos.y;
        f32 dz = cell->z - pos.z;
        f32 speed = cell->speed + member->acceleration * dt;
        cell->speed = speed;
        f32 ratio = speed * dt / Length(dx, dy, dz);
        f32 t = ratio < 1.0f ? ratio : 1.0f;
        bool arrived = ratio >= 1.0f;
        pos.x += dx * t;
        pos.y += dy * t;
        pos.z += dz * t;
        fn_710074566c(reinterpret_cast<UnkStruct_71007455dc*>(member), &pos.x);
        f32 away = Heading(dx, dy, dz);
        if (!member->field_0x180 || arrived)
            cell->state = 0;
        angle = away + 128.0f;
        break;
    }
    case 3: {
        angle = cell->heading;
        f32 delta = WrapDelta((f32)fn_71007452a4(AsTarget(member)) - angle);
        if (fabsf(delta) < 256.0f / 36.0f) {
            if (member->field_0x180) {
                UnkRedenkiBillboard_ovb0084* obj = self->field_0xe8->field_0x10->field_0x10;
                UnkRedenkiBillboard_ovb0084* billboard =
                    obj && fn_710009d50c(
                               reinterpret_cast<const CClass* (*)(UnkRedenkiBillboard_ovb0084*)>(obj->vtable[0])(obj),
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
                void* effect = fn_7100e1cb50(manager, &key, g_unk_710164a410);
                fn_7100745440(AsTarget(member), effect, 0);
            }
            cell->state = 5;
        }
        break;
    }
    case 4:
        if (entered)
            fn_710065a37c(reinterpret_cast<UnkRedenkiFxCtrl_ovb0084*>(self),
                          reinterpret_cast<UnkRedenkiDart_ovb0084*>(member));
        angle = cell->heading + 128.0f;
        if (!member->field_0x180)
            cell->state = 3;
        break;
    case 5:
        if (entered)
            self->field_0x468++;
        break;
    }

    f32 current = fn_71007452a4(AsTarget(member));
    f32 delta = WrapDelta(angle - current);
    f32 maxTurn = fabsf(dt * 256.0f);
    if (!(fabsf(delta) < maxTurn)) {
        f32 sign = (delta > 0.0f) - (delta < 0.0f);
        angle = maxTurn * sign + current;
        if (angle < 0.0f)
            angle += 256.0f;
        if (angle > 256.0f)
            angle -= 256.0f;
    }
    fn_71007456fc(AsTarget(member), angle);
}
