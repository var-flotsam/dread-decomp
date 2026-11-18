// behaviortree::CQuetzoaFollowInPathAction's unit (tools/tu_map.py, unit 969): the action's start. It resets its
// distance and facing, then, unless the Quetzoa AI is already following, hands the owner's navigation a goto-point
// request towards where the AI puts the owner's position. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

// As src/game/CQuetzoaAIComponent/CQuetzoaAIComponent_Dodge_A3_b76.cpp declares them.
struct UnkQuetzoaPos_A3_b76 {
    alignas(8) u32 v[4];  // 16 bytes: shares the name's slot and keeps its own place
    UnkQuetzoaPos_A3_b76(u32 x, u32 y, u32 z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }
};

// Returned in memory: the type is not trivially copyable.
// 16 bytes: the original keeps its slot above the position's.
struct UnkQuetzoaVec_A3_b76 {
    f32 x, y, z;
    u32 unk_0xc;
    UnkQuetzoaVec_A3_b76() = default;
    UnkQuetzoaVec_A3_b76(const UnkQuetzoaVec_A3_b76& o) : x(o.x), y(o.y), z(o.z) {}
    UnkQuetzoaVec_A3_b76& operator=(const UnkQuetzoaVec_A3_b76& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

struct UnkQuetzoaDodgeAI_A3_b76;
UnkQuetzoaVec_A3_b76 fn_710064b0b4(UnkQuetzoaDodgeAI_A3_b76* self, const UnkQuetzoaPos_A3_b76& pos);

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];

struct UnkQuetzoaFollowInPath_l646760;
void fn_71006469ac(UnkQuetzoaFollowInPath_l646760* self);
void fn_7100647090(UnkQuetzoaFollowInPath_l646760* self);

struct UnkQuetzoaAI_l646760;
void fn_710064ac98(UnkQuetzoaAI_l646760* ai);

struct UnkQuetzoaAI_l646760 {
    u8 pad_0x0[0x14ae8];
    bool following;  // 0x14ae8
};

struct UnkQuetzoaOwner_l646760 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x9c[0x240 - 0x38 - sizeof(UnkXform)];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x298 - 0x248];
    void* navigation;  // 0x298
};

struct UnkQuetzoaFollowInPath_l646760 {
    u8 pad_0x0[0x18];
    UnkQuetzoaOwner_l646760* owner;  // 0x18
    u8 pad_0x20[0xc];
    f32 distance;  // 0x2c
    f32 facing;    // 0x30
    bool arrived;  // 0x34
};

// The target of a goto-point request: a point (CAIGotoPointNavigationState::SPointTarget).
struct UnkPointTarget_l646760 {
    void** vtable;
    f32 x, y, z;  // 0x8

    UnkPointTarget_l646760(const UnkQuetzoaVec_A3_b76& point) : x(point.x), y(point.y), z(point.z) {
        vtable = g_unk_7101a25b60 + 2;
    }
};

// The goto-point request block handed to fn_71006f4b0c (the vtable-0x7101a25b38 kind).
struct UnkGotoParams_l646760 {
    void** vtable;
    f32 direction;  // 0x8
    f32 field_0xc;
    f32 field_0x10;
    bool has_direction;  // 0x14
    u8 pad_0x15[0x7];
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    UnkQuetzoaFollowInPath_l646760* listener;               // 0x20
    void (*on_arrived)(UnkQuetzoaFollowInPath_l646760* self);  // 0x28
    bool has_listener;                                        // 0x30
    u8 pad_0x31[0x3];
    u32 field_0x34;
    f32 radius;  // 0x38
    bool field_0x3c;
    bool field_0x3d;
    bool field_0x3e;
    u8 pad_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48;
    u64 field_0x50;
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue target;  // 0x70
    bool has_target;      // 0x88

    UnkGotoParams_l646760() {
        field_0x1c = false;
        listener = nullptr;
        on_arrived = nullptr;
        vtable = g_unk_7101a25b38 + 2;
        direction = 0.0f;
        field_0xc = 0.0f;
        field_0x10 = 0.0f;
        has_direction = false;
        field_0x3c = false;
        field_0x40 = false;
        field_0x68 = false;
        target = base::reflection::refl_EmptyValue();
        has_listener = false;
        field_0x6a = false;
        field_0x58 = false;
        has_target = false;
        field_0x34 = 1;
        field_0x3d = true;
        field_0x3e = true;
        field_0x48 = 0;
        field_0x50 = 0;
    }
    ~UnkGotoParams_l646760() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&target);
    }

    // Stores the point as the request's target: in place when the value already holds a compatible
    // type, else by rebuilding it.
    void SetTarget(const UnkPointTarget_l646760* point) {
        const CType* type = reinterpret_cast<const CType*>(fn_7100701c54());
        const CType* current = reinterpret_cast<const CType*>(target.type);
        if (current && target.field_0x14 == 0 &&
            (current == type ? type->m_fn_0x48 != nullptr : fn_710009d52c(type, current))) {
            fn_71000a1cc8(&target, type, point);
        } else {
            fn_71000a1c60(&target);
            fn_71000a18d4(&target, type, point, false);
        }
    }
};

void fn_7100646760(UnkQuetzoaFollowInPath_l646760* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->distance = 3.4028235e38f;
    self->arrived = false;
    f32 forward = self->owner->xform.GetMatrix().m[2];
    self->facing = static_cast<f32>((forward > 0.0f) - (forward < 0.0f));

    UnkReflObject* o = self->owner->ai;
    if (!o)
        return;
    auto* ai = reinterpret_cast<UnkQuetzoaAI_l646760*>(fn_710009d50c(o->vf_0x0(), fn_7100649630()) ? o : nullptr);
    if (!ai)
        return;
    void* navigation = self->owner->navigation;
    if (!navigation)
        return;

    if (ai->following) {
        fn_71006469ac(self);
    } else {
        const UnkXform& xform = self->owner->xform;
        UnkQuetzoaVec_A3_b76 point = fn_710064b0b4(
            reinterpret_cast<UnkQuetzoaDodgeAI_A3_b76*>(ai),
            UnkQuetzoaPos_A3_b76(*reinterpret_cast<const u32*>(&xform.m[0][3]),
                                 *reinterpret_cast<const u32*>(&xform.m[1][3]),
                                 *reinterpret_cast<const u32*>(&xform.m[2][3])));
        UnkGotoParams_l646760 params;
        f32 ahead = self->owner->xform.GetMatrix().m[2];
        params.field_0xc = 0.0f;
        params.field_0x10 = 0.0f;
        params.has_direction = true;
        params.radius = 20.0f;
        params.listener = self;
        params.on_arrived = fn_7100647090;
        params.field_0x3c = true;
        params.direction = ahead >= 0.0f ? 1.0f : -1.0f;
        UnkPointTarget_l646760 target(point);
        params.has_listener = true;
        params.has_target = true;
        params.SetTarget(&target);
        fn_71006f4b0c(navigation, &params, 1);
    }
    fn_710064ac98(ai);
}
