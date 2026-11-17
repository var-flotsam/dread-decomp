// behaviortree::CChangeAutsniperDirAction's unit (tools/tu_map.py 578): fn_71003d9e00 starts the turn. After
// the base start, when the owner's AI is a CAutsniperAIComponent and its CEnemyMovement is in state 1, the AI's
// limits of kind 2 are read; the owner's navigation is sent to the far limit (the near one when the movement
// is flipped), facing along the movement's direction (reversed when flipped). Names and layouts are
// placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkTU2702.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];  // CAIGotoPointNavigationState::SPointTarget's vtable

// The owner's CEnemyMovement as the turn reads it.
struct UnkAutsniperMovement_l3d9e00 {
    u8 pad_0x0[0x971];
    u8 field_0x971;
    u8 pad_0x972[0x9a0 - 0x972];
    f32 start_x;  // 0x9a0
    f32 start_y;  // 0x9a4
    f32 end_x;    // 0x9a8
    f32 end_y;    // 0x9ac
    u8 pad_0x9b0[0xa0c - 0x9b0];
    u8 field_0xa0c;
};
static_assert(__builtin_offsetof(UnkAutsniperMovement_l3d9e00, field_0xa0c) == 0xa0c, "layout");

struct UnkAutsniperOwner_l3d9e00 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x50];
    void* navigation;  // 0x298
};

struct UnkChangeAutsniperDir_l3d9e00 {
    u8 pad_0x0[0x18];
    UnkAutsniperOwner_l3d9e00* owner;  // 0x18
};

// The navigation's point target.
struct UnkPointTarget_l3d9e00 {
    void** vtable;
    CVector3D pos;
};

// The parameter block of the vtable-0x7101a25b38 kind.
struct UnkTurnParams_l3d9e00 {
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

    UnkTurnParams_l3d9e00() {
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
    ~UnkTurnParams_l3d9e00() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }
};
static_assert(__builtin_offsetof(UnkTurnParams_l3d9e00, value) == 0x70, "layout");

// Whether the object is of the class the getter describes (its own class read first).
template <const base::reflection::CClass* (*GetClass)()>
static inline bool IsA(UnkReflObject* object) {
    const CType* type = object->vf_0x0();
    return fn_710009d50c(type, GetClass());
}

// Stores the object of the given type in the value: in place when the value holds a compatible type,
// else by rebuilding it.
static inline void SetValue(UnkReflValue* value, const CType* type, const void* object) {
    const CType* current = reinterpret_cast<const CType*>(value->type);
    if (current && value->field_0x14 == 0 &&
        (current == type ? type->m_fn_0x48 != nullptr : fn_710009d52c(type, current))) {
        fn_71000a1cc8(value, type, object);
    } else {
        fn_71000a1c60(value);
        fn_71000a18d4(value, type, object, false);
    }
}

// The movement's direction in the plane, of unit length (zero stays zero).
static inline CVector3D Direction(const UnkAutsniperMovement_l3d9e00* movement) {
    f32 x = movement->end_x - movement->start_x;
    f32 y = movement->end_y - movement->start_y;
    f32 length = sqrtf(x * x + y * y);
    if (length != 0.0f) {
        f32 inv = 1.0f / length;
        x *= inv;
        y *= inv;
    }
    CVector3D dir;
    dir.x = x;
    dir.y = y;
    dir.z = 0.0f;
    return dir;
}

void fn_71003d9e00(UnkChangeAutsniperDir_l3d9e00* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* ai = self->owner->ai;
    if (!ai || !IsA<fn_71003da2e0>(ai))
        return;
    UnkReflObject* object = self->owner->movement;
    if (object && !IsA<fn_71010c74e0>(object))
        object = nullptr;
    if (fn_71010cc3a4(object) != 1)
        return;
    auto* movement = reinterpret_cast<UnkAutsniperMovement_l3d9e00*>(object);

    CVector3D near_limit;
    near_limit.x = 0.0f;
    near_limit.y = 0.0f;
    near_limit.z = 0.0f;
    CVector3D far_limit;
    far_limit.x = 0.0f;
    far_limit.y = 0.0f;
    far_limit.z = 0.0f;
    bool found;
    {
        u32 kind = 2;
        found = fn_71010fc864(reinterpret_cast<UnkBigFistAI3_ovb0050*>(ai), reinterpret_cast<UnkBigFistKind_ovb0050*>(&kind),
                              reinterpret_cast<UnkBigFistLimit_ovb0050*>(&near_limit),
                              reinterpret_cast<UnkBigFistLimit_ovb0050*>(&far_limit));
    }
    if (!found)
        return;
    bool flipped = (movement->field_0xa0c != 0) ^ (movement->field_0x971 != 0);

    UnkTurnParams_l3d9e00 params;
    UnkPointTarget_l3d9e00 point;
    point.vtable = g_unk_7101a25b60 + 2;
    const CVector3D& to = flipped ? near_limit : far_limit;
    point.pos.x = to.x;
    point.pos.y = to.y;
    point.pos.z = to.z;
    SetValue(&params.value, fn_7100701c54(), &point);
    params.field_0x18 = 1.0f;
    params.field_0x1c = true;
    CVector3D facing = Direction(movement);
    if (flipped) {
        facing = Direction(movement);
        facing.x = -facing.x;
        facing.y = -facing.y;
        facing.z = -facing.z;
    }
    params.facing = facing;
    params.has_facing = true;
    if (void* navigation = self->owner->navigation)
        fn_71006f4b0c(navigation, &params, 1);
}
