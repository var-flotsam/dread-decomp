// behaviortree::CTakumakuChaseAction's unit (tools/tu_map.py unit 1076): the node's run (fn_71006d1394): it
// projects the actor and its target onto the AI's current path segment and, by how far apart they are, has the
// navigation go to a point short of the target, turn on the spot, or back off and play a turn. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflTypeIdArg;
using base::reflection::UnkReflValue;

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;

// The two directions a move request faces: one when the target is ahead in x, the other otherwise.
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_710164a7b8;
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_710164a7c4;

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkGrabClassStaticZero_ovb0120 {
    u32 value;
    UnkGrabClassStaticZero_ovb0120() : value(0) {}
};

inline const UnkGrabClassStaticZero_ovb0120& UnkGrabClassStaticZero() {
    static UnkGrabClassStaticZero_ovb0120 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

// The complement of that zero's low eight bits.
inline u32 UnkGrabClassDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkGrabClassStaticZero().value & 0xff;
    return s_mask;
}

struct UnkPathPoint_l6d1394 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkTakumakuAI_l6d1394 {
    u8 pad_0x0[0x4c];
    s32 point;  // 0x4c: the current path point, or negative for none
    u8 pad_0x50[0x18];
    UnkPathPoint_l6d1394* points;  // 0x68
    u32 point_count;               // 0x70
    u8 pad_0x74[0x20];
    u8 looped;  // 0x94: whether the path's last point leads back to the first
    u8 pad_0x95[0x14699 - 0x95];
    bool field_0x14699;
    u8 pad_0x1469a[0x14758 - 0x1469a];
    bool field_0x14758;

    // How many segments the path's points make: one fewer than the points unless it loops.
    u32 SegmentCount() const { return point_count - ((point_count != 0) & (looped == 0)); }
};
static_assert(__builtin_offsetof(UnkTakumakuAI_l6d1394, field_0x14758) == 0x14758, "layout");

f32 fn_71006d3e10(UnkTakumakuAI_l6d1394* ai);
const CStrId& fn_71006d3e04(UnkTakumakuAI_l6d1394* ai);
const CStrId& fn_71006d3e20(UnkTakumakuAI_l6d1394* ai);
void fn_71006d3e2c(UnkTakumakuAI_l6d1394* ai);
bool fn_71006d5d68(UnkTakumakuAI_l6d1394* ai);
UnkVec2Result fn_71001261e8(const UnkSegment2D* seg, const CVector2D* point);

struct UnkTakumakuCharClass_l6d1394 {
    u8 pad_0x0[0x634];
    f32 near_distance;  // 0x634
    f32 far_distance;   // 0x638
};

struct UnkTakumakuChase_l6d1394 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0x10];
    UnkTakumakuAI_l6d1394* ai;                // 0x30
    UnkTakumakuCharClass_l6d1394* charclass;  // 0x38
    CVector3D goal;                           // 0x40
    s32 turns;                                // 0x4c
};

// A move request (kind 5) facing one way.
struct UnkMoveParams_l6d1394 {
    void** vtable;
    CVector3D dir;  // 0x8
    bool field_0x14;
    u8 pad_0x15[3];
    f32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 pad_0x38;
    bool field_0x39;
    u8 pad_0x3a;
    bool field_0x3b;
    u8 pad_0x3c;
    bool field_0x3d;

    explicit UnkMoveParams_l6d1394(const CVector3D& d) {
        vtable = g_unk_7101a25b10 + 2;
        field_0x1c = false;
        field_0x20[0] = 0;
        field_0x20[1] = 0;
        field_0x30 = false;
        field_0x39 = false;
        field_0x3b = false;
        field_0x3d = false;
        kind = 5;
        dir = d;
        field_0x14 = true;
    }
};

// A go-to-point request: the point is held as a reflected value.
struct UnkGotoParams_l6d1394 {
    void** vtable;
    CVector3D dir;  // 0x8
    bool field_0x14;
    u8 pad_0x15[3];
    f32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 pad_0x38[4];
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    bool field_0x3f;
    bool field_0x40;
    u8 pad_0x41[7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[5];
    UnkReflValue target;  // 0x70
    bool field_0x88;

    UnkGotoParams_l6d1394() : vtable(g_unk_7101a25b38 + 2), dir(), field_0x14(false) {
        field_0x1c = false;
        field_0x20[0] = 0;
        field_0x20[1] = 0;
        field_0x30 = false;
        kind = 1;
        field_0x3c = false;
        field_0x3e = false;
        field_0x40 = false;
        field_0x48[0] = 0;
        field_0x48[1] = 0;
        field_0x58 = false;
        field_0x68 = false;
        field_0x6a = false;
        target = base::reflection::refl_EmptyValue();
        field_0x88 = true;
    }
    ~UnkGotoParams_l6d1394() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&target);
    }

    // Holds `object` as a value of class `cls`: in place when the value already holds a compatible
    // class, else rebuilt.
    void SetTarget(const CClass* cls, const void* object) {
        const CType* current = reinterpret_cast<const CType*>(target.type);
        if (current && target.field_0x14 == 0 &&
            (current == cls ? cls->m_fn_0x48 != nullptr : fn_710009d52c(cls, current))) {
            fn_71000a1cc8(&target, cls, object);
        } else {
            fn_71000a1c60(&target);
            fn_71000a18d4(&target, cls, object, false);
        }
    }
};

// CAIGotoPointNavigationState::SPointTarget: a point to go to.
struct UnkPointTarget_l6d1394 {
    void** vtable;
    CVector3D pos;
};

static const CVector3D& MoveDirection(f32 dx) { return dx > 0.0f ? g_unk_710164a7b8 : g_unk_710164a7c4; }

s32 fn_71006d1394(UnkTakumakuChase_l6d1394* self) {
    UnkActor* target = self->actor ? fn_7100d7c990(self->actor) : nullptr;
    UnkAnimationComponent* animation =
        self->actor ? static_cast<UnkAnimationComponent*>(self->actor->field_0x220) : nullptr;
    if (!self->ai)
        return 6;
    UnkNavigation* navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(self->ai));
    UnkTakumakuAI_l6d1394* ai = self->ai;
    if (!navigation || !animation || !ai || !self->charclass)
        return 6;

    s32 point = ai->point;
    bool tracking = point >= 0 && !fn_710117b348(animation, fn_7100c526a8());
    UnkTakumakuCharClass_l6d1394* charclass = self->charclass;
    u32 count = ai->point_count;
    if (ai->SegmentCount() == 0 || !tracking)
        return 2;

    f32 near_distance = charclass->near_distance;
    f32 far_distance = charclass->far_distance;
    u32 index = point;
    const UnkPathPoint_l6d1394& next = count - 1 == index ? ai->points[0] : ai->points[index + 1];
    UnkSegment2D segment(ai->points[index].pos, next.pos);

    // The actor and its target, each moved onto the segment.
    f32 from_x = self->actor->field_0x44;
    f32 from_y = self->actor->field_0x54;
    f32 from_z = self->actor->field_0x64;
    CVector3D from;
    from.x = from_x;
    from.y = from_y;
    from.z = from_z;
    {
        CVector2D flat;
        fn_710007b430(&flat, &from);
        UnkVec2Result on = fn_71001261e8(&segment, &flat);
        from.x = on.x;
        from.y = on.y;
        from.z = 0.0f;
    }
    f32 to_x = target->field_0x44;
    f32 to_y = target->field_0x54;
    f32 to_z = target->field_0x64;
    CVector3D to;
    to.x = to_x;
    to.y = to_y;
    to.z = to_z;
    {
        CVector2D flat;
        fn_710007b430(&flat, &to);
        UnkVec2Result on = fn_71001261e8(&segment, &flat);
        to.x = on.x;
        to.y = on.y;
        to.z = 0.0f;
    }

    CVector3D dir;
    dir.x = to.x - from.x;
    dir.y = 0.0f;
    dir.z = to.z - from.z;
    f32 distance = __builtin_fabsf(to.x - from.x);
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
    f32 facing = dir.x * m.m[2] + dir.y * m.m[6] + dir.z * m.m[10];

    if (distance > far_distance && facing > 0.001f) {
        // Too far: go to a point short of the target.
        f32 inv = 1.0f / distance;
        f32 back = -__builtin_fminf(far_distance, distance);
        CVector3D goal;
        goal.x = to.x + back * (dir.x * inv);
        goal.y = to.y + back * (dir.y * inv);
        goal.z = to.z + back * (dir.z * inv);
        self->goal.x = goal.x;
        self->goal.y = goal.y;
        self->goal.z = goal.z;
        UnkGotoParams_l6d1394 params;
        UnkPointTarget_l6d1394 point_target;
        point_target.pos = goal;
        point_target.vtable = g_unk_7101a25b60 + 2;
        params.SetTarget(fn_7100701c54(), &point_target);
        params.field_0x3f = false;
        params.field_0x40 = true;
        params.field_0x14 = true;
        params.dir = MoveDirection(dir.x);
        params.field_0x18 = 1.0f;
        params.field_0x1c = true;
        fn_71006f4b0c(navigation, &params, 1);
        return 2;
    }

    if (!(distance < near_distance && facing > 0.001f) || self->ai->field_0x14699) {
        // Close enough: stay where the actor is.
        self->goal = from;
        UnkMoveParams_l6d1394 params(MoveDirection(dir.x));
        fn_71006f4b0c(navigation, &params, 1);
        return 2;
    }

    // Too close: back off, turning when the AI's speed calls for it.
    f32 limit = near_distance - distance;
    f32 reported = 0.0f;
    if (fn_71010fc778(self->ai, &reported) && reported < limit)
        limit = reported;
    CStrId turn = g_unk_7101d470e0;
    if (fn_71006d3e10(self->ai) > limit) {
        turn = fn_71006d3e04(self->ai);
        fn_71006d3e10(self->ai);
    } else {
        turn = fn_71006d3e20(self->ai);
        fn_71006d3e2c(self->ai);
    }
    if (self->turns != 0 && !fn_710117b348(animation, fn_7100d2b3a8()) &&
        !fn_71011057fc(reinterpret_cast<UnkRobotSoldierAI_A2_b69*>(self->ai)) && fn_71006d5d68(self->ai) &&
        turn.data != &g_unk_7101d47108->empty) {
        UnkReflTypeIdArg mode;
        mode.id = 0;
        UnkReflTypeIdArg mask;
        mask.id = UnkGrabClassDefaultMask();
        if (CAnimationComponent_SendActionToTrack(animation, turn.data->Key(), 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask)) {
            if (self->turns > 0 && --self->turns == 0)
                self->ai->field_0x14758 = true;
        }
    }
    f32 inv = 1.0f / distance;
    self->goal.x = to.x - near_distance * (dir.x * inv);
    self->goal.y = to.y - near_distance * (dir.y * inv);
    self->goal.z = to.z - near_distance * (dir.z * inv);
    UnkMoveParams_l6d1394 params(MoveDirection(dir.x));
    fn_71006f4b0c(navigation, &params, 1);
    return 2;
}
