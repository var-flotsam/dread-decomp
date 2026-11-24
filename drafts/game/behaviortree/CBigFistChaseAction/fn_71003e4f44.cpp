// behaviortree::CBigFistChaseAction's unit (tools/tu_map.py 590): fn_71003e4f44 runs the chase. The target and
// the owner are both moved onto the current segment of the AI's path; the point the chase distance short of the
// target, towards the owner, is stored (+0x34) and handed to the owner's navigation as a point target, facing
// the way the target lies. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkBigFistChase_ovb0114;
f32 CBigFistChaseAction_ComputeDesiredChaseDistance(UnkBigFistChase_ovb0114* self);

struct UnkUnit110Vec2 {
    f32 x;
    f32 y;
    UnkUnit110Vec2() {}
    UnkUnit110Vec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkUnit110Vec2(const UnkUnit110Vec2& o) : x(o.x), y(o.y) {}
};
struct UnkUnit110Segment;
UnkUnit110Vec2 fn_71001261e8(const UnkUnit110Segment* seg, const UnkUnit110Vec2* p);

// The two facings: towards +x and towards -x.
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_7101649338;
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_7101649344;

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];  // CAIGotoPointNavigationState::SPointTarget's vtable

struct UnkBigFistPathPoint_l3e4f44 {
    CVector2D pos;
    f32 field_0x8;
};

// The AI component as the chase reads its path.
struct UnkBigFistPath_l3e4f44 {
    u8 pad_0x0[0x4c];
    s32 current;  // 0x4c
    u8 pad_0x50[0x18];
    UnkBigFistPathPoint_l3e4f44* points;  // 0x68
    u32 count;  // 0x70
};

struct UnkBigFistChase_l3e4f44 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0xc];
    u32 field_0x2c;
    u8 pad_0x30[0x4];
    CVector3D target;  // 0x34
};

// The navigation's point target.
struct UnkPointTarget_l3e4f44 {
    void** vtable;
    CVector3D pos;
};

// The parameter block of the vtable-0x7101a25b38 kind.
struct UnkChaseParams_l3e4f44 {
    void** vtable;
    CVector3D facing;  // 0x8
    bool has_facing;   // 0x14
    u8 pad_0x15[0x3];
    f32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u32 field_0x38;
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    bool field_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue value;  // 0x70
    bool field_0x88;

    UnkChaseParams_l3e4f44() {
        field_0x1c = false;
        vtable = g_unk_7101a25b38 + 2;
        __builtin_memset(&facing, 0, 13);
        field_0x3c = false;
        value = base::reflection::refl_EmptyValue();
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        field_0x88 = true;
        field_0x34 = 1;
        field_0x30 = false;
        __builtin_memset(field_0x48, 0, 16);
        __builtin_memset(field_0x20, 0, 16);
    }
    ~UnkChaseParams_l3e4f44() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }
};
static_assert(__builtin_offsetof(UnkChaseParams_l3e4f44, value) == 0x70, "layout");

static inline CVector3D Position(UnkActor* actor) {
    f32 x = actor->field_0x44;
    f32 y = actor->field_0x54;
    f32 z = actor->field_0x64;
    CVector3D pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    return pos;
}

// Moves the point onto the segment (in the plane, at height zero).
static inline void SnapToSegment(const UnkSegment2D& segment, CVector3D& point) {
    CVector2D flat;
    fn_710007b430(&flat, &point);
    UnkUnit110Vec2 on = fn_71001261e8(reinterpret_cast<const UnkUnit110Segment*>(&segment),
                                      reinterpret_cast<const UnkUnit110Vec2*>(&flat));
    point.x = on.x;
    point.y = on.y;
    point.z = 0.0f;
}

// Stores the object of the given type in the value: in place when the value holds a compatible type,
// else by rebuilding it.
static inline void SetValue(UnkReflValue* value, const CType* type, const void* object) {
    const CType* current = reinterpret_cast<const CType*>(value->type);
    if (current && value->field_0x14 == 0 &&
        (current == type ? type->m_copyAssign != nullptr : fn_710009d52c(type, current))) {
        fn_71000a1cc8(value, type, object);
    } else {
        fn_71000a1c60(value);
        fn_71000a18d4(value, type, object, false);
    }
}

s32 fn_71003e4f44(UnkBigFistChase_l3e4f44* self) {
    if (!self->owner->field_0x240 || !self->owner->field_0x298)
        return 6;
    f32 distance = CBigFistChaseAction_ComputeDesiredChaseDistance(reinterpret_cast<UnkBigFistChase_ovb0114*>(self));
    auto* path = static_cast<UnkBigFistPath_l3e4f44*>(self->owner->field_0x240);
    if (path->current < 0)
        return 6;
    u32 index = path->current;

    UnkBigFistPathPoint_l3e4f44* points = path->points;
    u32 next = path->count - 1 == index ? 0 : index + 1;
    UnkSegment2D segment(points[index].pos, points[next].pos);

    CVector3D target = Position(fn_7100d7c990(self->owner));
    SnapToSegment(segment, target);
    CVector3D own = Position(self->owner);
    SnapToSegment(segment, own);

    f32 dx = target.x - own.x;
    f32 dy = target.y - own.y;
    f32 dz = target.z - own.z;
    f32 length = sqrtf(dx * dx + dy * dy + dz * dz);
    f32 inv = 1.0f / length;
    f32 step = distance < length ? distance : length;
    CVector3D goal;
    goal.x = target.x - dx * inv * step;
    goal.y = target.y - dy * inv * step;
    goal.z = target.z - dz * inv * step;
    self->target = goal;

    UnkPointTarget_l3e4f44 point;
    UnkChaseParams_l3e4f44 params;
    point.vtable = g_unk_7101a25b60 + 2;
    point.pos = goal;
    SetValue(&params.value, fn_7100701c54(), &point);
    params.field_0x3f = false;
    params.field_0x40 = true;
    params.facing = dx > 0.0f ? g_unk_7101649338 : g_unk_7101649344;
    params.has_facing = true;
    params.field_0x38 = self->field_0x2c;
    params.field_0x3c = true;
    params.field_0x18 = 1.0f;
    params.field_0x1c = true;
    fn_71006f4b0c(self->owner->field_0x298, &params, 1);
    return 2;
}
