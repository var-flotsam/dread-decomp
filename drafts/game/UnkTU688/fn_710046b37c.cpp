// Unit 688 (behaviortree::CChozoRobotSoldierGotoShootingPositionAction): the action's update. With the
// robot soldier's shooting point near its navigation side, it turns to face the target (and resets the
// soldier when its navigation is not in a kind-5 record); otherwise, while moves are pending, it sends the
// navigation to the target's position; with neither, it only faces the target. Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkTU362.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

bool fn_7101109e8c(UnkAIComponent* self, UnkNavMeshHit* side);

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];

// The two directions the soldier faces in: towards one side or the other.
extern const CVector3D g_unk_71016497b8 __attribute__((visibility("hidden")));
extern const CVector3D g_unk_71016497c4 __attribute__((visibility("hidden")));

struct UnkRobotSoldierAI_l46b37c {
    u8 pad_0x0[0x14b8c];
    u32 pending;  // 0x14b8c
    u8 field_0x14b90[0x8];  // 0x14b90
};

UnkActor* fn_71004739f8(UnkRobotSoldierAI_l46b37c* self);
void fn_7100474d30(UnkRobotSoldierAI_l46b37c* self);

struct UnkNavKind_l46b37c {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};
struct UnkNavEntry_l46b37c {
    u8 pad_0x0[0x10];
    UnkNavKind_l46b37c* kind;  // 0x10
};
struct UnkNavRecords_l46b37c {
    UnkNavEntry_l46b37c* first;
    u8 pad_0x8[0x20];
    void* field_0x28;
};

struct UnkNavClass_l46b37c {
    u8 pad_0x0[0x78];
    f32 reach;  // 0x78
};

// The navigation object as this action reads it.
struct UnkSoldierNav_l46b37c {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x98];
    UnkNavRecords_l46b37c* records;  // 0xb0
};

struct UnkGotoShootingPosition_l46b37c {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0xc];
    bool restart;  // 0x2c
};

// The kind-5 parameter block: which way to face.
struct UnkFaceParams_l46b37c {
    void** vtable;
    CVector3D direction;  // 0x8
    bool field_0x14;
    u8 pad_0x15[0x3];
    u32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 kind;  // 0x34
    u8 pad_0x38;
    bool field_0x39;
    u8 pad_0x3a;
    bool field_0x3b;
    u8 pad_0x3c;
    bool field_0x3d;
};

// The go-to-point parameter block (vtable 0x7101a25b38), with its target in a reflected value.
struct UnkGotoParams_l46b37c {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[0x3];
    f32 speed;  // 0x18
    bool has_speed;  // 0x1c
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u8 pad_0x38[0x4];
    bool field_0x3c;
    bool field_0x3d;
    bool field_0x3e;
    bool field_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0x7];
    void* field_0x60;
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue target;  // 0x70
    bool has_target;  // 0x88

    UnkGotoParams_l46b37c() {
        has_speed = false;
        vtable = g_unk_7101a25b38 + 2;
        __builtin_memset(field_0x8, 0, 13);
        field_0x3c = false;
        target = base::reflection::refl_EmptyValue();
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        has_target = false;
        field_0x34 = 1;
        field_0x30 = true;
        __builtin_memset(field_0x48, 0, 16);
        __builtin_memset(field_0x20, 0, 16);
    }
    ~UnkGotoParams_l46b37c() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&target);
    }
};

// CAIGotoPointNavigationState::SPointTarget: a position to go to.
struct UnkPointTarget_l46b37c {
    void** vtable;
    CVector3D position;

    explicit UnkPointTarget_l46b37c(const CVector3D& at) : vtable(g_unk_7101a25b60 + 2), position(at) {}
};

static inline CVector3D Translation(const UnkActor* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

// The object when it is of the class `Class` gives, else null.
template <const base::reflection::CClass* (*Class)()>
static inline bool IsKindOf(UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), Class());
}

// Turns the owner towards the side of it the target is on.
static inline void FaceTarget(UnkGotoShootingPosition_l46b37c* self, UnkSoldierNav_l46b37c* nav, UnkActor* target) {
    UnkFaceParams_l46b37c params;
    params.field_0x1c = false;
    params.kind = 5;
    params.field_0x30 = false;
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x39 = false;
    params.field_0x3b = false;
    params.field_0x3d = false;
    params.vtable = g_unk_7101a25b10 + 2;
    params.direction = self->owner->field_0x44 > target->field_0x44 ? g_unk_71016497b8 : g_unk_71016497c4;
    params.field_0x14 = true;
    fn_71006f4b0c(nav, &params, 1);
}

// Stores the point in the value: in place when it already holds a compatible type, else rebuilt.
static inline void SetTarget(UnkReflValue* value, const UnkPointTarget_l46b37c* point) {
    const CType* type = fn_7100701c54();
    const CType* current = reinterpret_cast<const CType*>(value->type);
    if (current && value->field_0x14 == 0 &&
        (current == type ? type->m_fn_0x48 != nullptr : fn_710009d52c(type, current))) {
        fn_71000a1cc8(value, type, point);
    } else {
        fn_71000a1c60(value);
        fn_71000a18d4(value, type, point, false);
    }
}

enum UnkPlacement_l46b37c { kStay, kNear, kMove };

// Whether the soldier's shooting point is within its navigation reach of the soldier's side; if not,
// whether it still has moves pending.
static inline UnkPlacement_l46b37c Placement(UnkAIComponent* ai, UnkRobotSoldierAI_l46b37c* soldier, UnkSoldierNav_l46b37c* nav,
                                             UnkActor* point) {
    UnkNavMeshHit side;
    fn_7101109e8c(ai, &side);
    f32 reach = 10.0f;
    if (UnkReflObject* object = nav->char_class) {
        if (IsKindOf<fn_71006f5fbc>(object))
            reach = reinterpret_cast<UnkNavClass_l46b37c*>(object)->reach;
    }
    CVector2D at;
    CVector3D pos = Translation(point);
    fn_710007b430(&at, &pos);
    if (reach + side.point.x >= at.x && side.point.x - reach <= at.x && reach + side.point.y >= at.y &&
        side.point.y - reach <= at.y)
        return kNear;
    return soldier->pending ? kMove : kStay;
}

u32 fn_710046b37c(UnkGotoShootingPosition_l46b37c* self) {
    UnkAIComponent* ai = nullptr;
    UnkRobotSoldierAI_l46b37c* soldier = nullptr;
    UnkSoldierNav_l46b37c* nav = nullptr;
    if (self->owner) {
        UnkReflObject* object = static_cast<UnkReflObject*>(self->owner->field_0x240);
        if (object && IsKindOf<fn_710046ea2c>(object)) {
            ai = reinterpret_cast<UnkAIComponent*>(object);
            nav = reinterpret_cast<UnkSoldierNav_l46b37c*>(fn_71010fccf8(ai));
            soldier = reinterpret_cast<UnkRobotSoldierAI_l46b37c*>(object);
        }
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!nav || !target)
        return 6;

    // Near its point, the soldier is reset unless its navigation is in a kind-5 record that has one.
    bool reset = false;
    if (UnkActor* point = fn_71004739f8(soldier)) {
        switch (Placement(ai, soldier, nav, point)) {
        case kNear: {
            UnkNavRecords_l46b37c* records = nav->records;
            reset = true;
            if (records->first && records->first->kind && records->first->kind->kind == 5)
                reset = records->field_0x28 == nullptr;
            break;
        }
        case kMove: {
            UnkGotoParams_l46b37c params;
            params.speed = 1.0f;
            params.has_speed = true;
            params.field_0x30 = false;
            params.field_0x3f = true;
            params.field_0x40 = true;
            bool restart = true;
            if (self->restart || soldier->pending == 1) {
                self->restart = false;
                params.field_0x60 = soldier->field_0x14b90;
                params.field_0x68 = true;
            } else {
                restart = false;
            }
            params.field_0x3d = true;
            params.field_0x3e = true;
            UnkPointTarget_l46b37c goal(Translation(target));
            params.has_target = true;
            SetTarget(&params.target, &goal);
            fn_71006f4b0c(nav, &params, restart);
            return 2;
        }
        case kStay:
            break;
        }
    }
    FaceTarget(self, nav, target);
    if (reset)
        fn_7100474d30(soldier);
    return 2;
}
