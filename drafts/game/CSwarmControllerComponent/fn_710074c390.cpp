// CSwarmControllerComponent's unit (tools/tu_map.py unit 1163): ages the swarm's members by `dt`, dropping
// the ones whose time ran out; a member early in its time that is still near the line between its two path
// points speeds up and moves along its direction. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1159.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkStruct_71007455dc;
UnkVec3Result GetPositionFromParentAndOffset(UnkStruct_71007455dc* self);
void fn_710074566c(UnkStruct_71007455dc* self, const f32* pos);

struct UnkEntryList_b0202;
UnkUnit2560Element** fn_7100745c5c(UnkEntryList_b0202* list, u32 index);

struct UnkEntryList_b0202 {
    void* data;  // 0x0
    u32 count;   // 0x8
};

// Removes the entry at index, closing the gap.
static inline void RemoveAt(UnkEntryList_b0202* list, u32 index) {
    if (index < list->count) {
        void** data = static_cast<void**>(list->data);
        __builtin_memcpy(&data[index], &data[index] + 1, (u64)(list->count - index - 1) * sizeof(void*));
        list->count = list->count - 1;
    }
}

struct UnkSegment2D_71001262e0;  // a segment's two points (src/game/malloc/fn_71001262e0.cpp)
f32 fn_71001262e0(const UnkSegment2D_71001262e0* seg, const CVector2D* p, CVector2D* closest, f32* t);

// A point of a path: its position (vf_0x18).
struct UnkPathPoint_74c390 {
    void** vtable;
    const CVector3D* Position() {
        return reinterpret_cast<const CVector3D* (*)(UnkPathPoint_74c390*)>(vtable[0x18 / 8])(this);
    }
};

// A path: its point count (vf_0x28) and its points (vf_0x30).
struct UnkPath_74c390 {
    void** vtable;
    u32 Count() { return reinterpret_cast<u32 (*)(UnkPath_74c390*)>(vtable[0x28 / 8])(this); }
    UnkPathPoint_74c390* Point(u32 index) {
        return reinterpret_cast<UnkPathPoint_74c390* (*)(UnkPath_74c390*, u32)>(vtable[0x30 / 8])(this, index);
    }
};

// Where a member goes: the segment between two path points, and how close it must stay.
struct UnkMemberRoute_74c390 {
    u8 pad_0x0[0x44];
    f32 radius;        // 0x44
    u32 from_path;     // 0x48
    u32 from_point;    // 0x4c
    u32 to_path;       // 0x50
    u32 to_point;      // 0x54
};

struct UnkSwarmMember_74c390 {
    u8 pad_0x0[0x10];
    UnkMemberRoute_74c390* route;  // 0x10
    u8 pad_0x18[0x28 - 0x18];
    s32 field_0x28;
    u8 pad_0x2c[0x38 - 0x2c];
    CVector3D direction;  // 0x38
    u8 pad_0x44[0x158 - 0x44];
    f32 duration;      // 0x158
    f32 time_left;     // 0x15c
    f32 speed;         // 0x160
    f32 acceleration;  // 0x164
};

struct UnkBillboard_74c390 {
    void** vtable;
    u8 pad_0x8[0x60 - 0x8];
    f32 width;  // 0x60
    const base::reflection::CClass* GetClass() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkBillboard_74c390*)>(vtable[0])(this);
    }
};

struct UnkBillboardHolder_74c390 {
    u8 pad_0x0[0x10];
    UnkBillboard_74c390* billboard;
};

struct UnkActorLink_74c390 {
    u8 pad_0x0[0x10];
    UnkBillboardHolder_74c390* holder;
};

struct UnkSwarmController_74c390 {
    void** vtable;
    u8 pad_0x8[0xe8 - 0x8];
    UnkActorLink_74c390* actor;  // 0xe8
    u8 pad_0xf0[0x208 - 0xf0];
    UnkEntryList_b0202 members;  // 0x208

    UnkPath_74c390* GetPath(u32 index) {
        return reinterpret_cast<UnkPath_74c390* (*)(UnkSwarmController_74c390*, u32)>(vtable[0x1a8 / 8])(this,
                                                                                                         index);
    }
    UnkPathPoint_74c390* GetPoint(u32 path_index, u32 point_index) {
        UnkPath_74c390* path = GetPath(path_index);
        if (path && point_index < path->Count())
            return path->Point(point_index);
        return nullptr;
    }
};

void fn_710074c390(UnkSwarmController_74c390* self, f32 dt) {
    // Backwards, so that removing a member keeps the ones still to visit in place.
    for (u32 i = self->members.count - 1; (s32)i >= 0; --i) {
        UnkSwarmMember_74c390* member = reinterpret_cast<UnkSwarmMember_74c390*>(*fn_7100745c5c(&self->members, i));
        member->time_left -= dt;
        if (member->time_left < 0.0f) {
            RemoveAt(&self->members, i);
            member->field_0x28 = 0;
            continue;
        }
        if (1.0f - member->time_left / member->duration < 0.66f) {
            UnkMemberRoute_74c390* route = member->route;
            u32 from_path = route->from_path;
            u32 from_point = route->from_point;
            u32 to_path = route->to_path;
            u32 to_point = route->to_point;
            UnkVec3Result pos = GetPositionFromParentAndOffset(reinterpret_cast<UnkStruct_71007455dc*>(member));
            UnkPathPoint_74c390* from = self->GetPoint(from_path, from_point);
            UnkPathPoint_74c390* to = self->GetPoint(to_path, to_point);
            if (from && to) {
                CVector2D a;
                fn_710007b430(&a, from->Position());
                CVector2D b;
                fn_710007b430(&b, to->Position());
                UnkSegment2D segment(a, b);
                CVector2D p;
                fn_710007b430(&p, reinterpret_cast<const CVector3D*>(&pos));
                f32 distance_sq = fn_71001262e0(reinterpret_cast<const UnkSegment2D_71001262e0*>(&segment), &p,
                                                nullptr, nullptr);
                f32 radius = route->radius;
                UnkBillboard_74c390* obj = self->actor->holder->billboard;
                UnkBillboard_74c390* billboard = fn_710009d50c(obj->GetClass(), fn_710072be34()) ? obj : nullptr;
                f32 reach = radius + billboard->width * -0.5f;
                if (reach * reach > distance_sq) {
                    f32 speed = member->acceleration * dt + member->speed;
                    speed = speed > 0.0f ? speed : 0.0f;
                    member->speed = speed;
                    f32 step = speed * dt;
                    f32 dx = member->direction.x * step;
                    f32 dy = member->direction.y * step;
                    f32 dz = member->direction.z * step;
                    pos.x += dx;
                    pos.y += dy;
                    pos.z += dz;
                    fn_710074566c(reinterpret_cast<UnkStruct_71007455dc*>(member), &pos.x);
                }
            }
        }
    }
}
