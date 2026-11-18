// behaviortree::CGoToTargetAction (unit 478): starts the walk to the target. With a navigation object
// and a target, it takes the AI component's path (or plans one when the action follows the target),
// then hands the navigation object a go-to-point block aimed at the target's position. Returns 6 when
// it cannot start, else 2. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];

void fn_7101103080(UnkAIComponent* ai, void* path, const void* from, const void* to);

struct UnkGoToTargetAIState_l3817f0 {
    u8 pad_0x0[0xc5];
    u8 field_0xc5;
};

struct UnkGoToTargetAI_l3817f0 {
    u8 pad_0x0[0x1610];
    UnkGoToTargetAIState_l3817f0* field_0x1610;
    u8 pad_0x1618[0x19c0 - 0x1618];
    bool has_path;  // 0x19c0
    u8 pad_0x19c1[7];
    u8 path[8];  // 0x19c8
};

// CAIGotoPointNavigationState::SPointTarget: a point to walk to.
struct UnkPointTarget_l3817f0 {
    void** vtable;
    f32 x, y, z;

    UnkPointTarget_l3817f0(f32 x_, f32 y_, f32 z_) : vtable(g_unk_7101a25b60 + 2), x(x_), y(y_), z(z_) {}
};

// The go-to-point block (vtable 0x7101a25b38) that the navigation object takes.
struct UnkGoToParams_l3817f0 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    f32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38[4];
    bool field_0x3c;
    u8 field_0x3d;
    bool field_0x3e;
    bool field_0x3f;
    bool field_0x40;
    u8 pad_0x41[7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[7];
    void* path;  // 0x60
    bool field_0x68;
    bool field_0x69;
    bool field_0x6a;
    u8 pad_0x6b[5];
    UnkReflValue target;  // 0x70
    bool has_target;      // 0x88

    explicit UnkGoToParams_l3817f0(void* path_) {
        __builtin_memset(field_0x8, 0, 13);
        __builtin_memset(field_0x20, 0, 16);
        vtable = g_unk_7101a25b38 + 2;
        target = base::reflection::refl_EmptyValue();
        field_0x18 = 1.0f;
        field_0x3e = false;
        field_0x3f = true;
        field_0x40 = true;
        field_0x69 = false;
        field_0x6a = true;
        field_0x3c = false;
        field_0x58 = false;
        field_0x30 = false;
        has_target = false;
        field_0x34 = 1;
        field_0x1c = true;
        path = path_;
        field_0x68 = true;
        __builtin_memset(field_0x48, 0, 16);
    }
    ~UnkGoToParams_l3817f0() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&target);
    }

    void SetTarget(const UnkPointTarget_l3817f0& point) {
        has_target = true;
        const CType* type = fn_7100701c54();
        fn_71000a1c60(&target);
        fn_71000a18d4(&target, type, &point, false);
    }
};

struct UnkGoToTargetAction_l3817f0 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x10];
    u8 path[0x12abc - 0x30];  // 0x30
    bool follow;              // 0x12abc
};
static_assert(__builtin_offsetof(UnkGoToTargetAction_l3817f0, follow) == 0x12abc, "layout");

s32 fn_71003817f0(UnkGoToTargetAction_l3817f0* self) {
    UnkGoToTargetAI_l3817f0* ai = nullptr;
    UnkNavigation* navigation = nullptr;
    if (self->owner) {
        ai = static_cast<UnkGoToTargetAI_l3817f0*>(self->owner->field_0x240);
        if (ai)
            navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai));
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!navigation || !target)
        return 6;
    if (!ai->has_path)
        return 6;
    if (self->follow) {
        if (fn_71010cc3a4(self->owner->field_0x218) == 4)
            return 2;
        if (fn_710117b348(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), fn_7100d2b3a8()))
            return 2;
    }
    if (self->follow)
        fn_7101103080(reinterpret_cast<UnkAIComponent*>(ai), self->path, nullptr, nullptr);
    else
        fn_71002ad420(self->path, ai->path);

    UnkGoToParams_l3817f0 params(self->path);
    if (ai && ai->field_0x1610) {
        params.field_0x3d = ai->field_0x1610->field_0xc5;
        params.field_0x3e = true;
    }
    UnkPointTarget_l3817f0 point(target->field_0x44, target->field_0x54, target->field_0x64);
    params.SetTarget(point);
    fn_71006f4b0c(navigation, &params, 1);
    return 2;
}
