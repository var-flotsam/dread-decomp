// behaviortree::CFlyingChaseTargetAction's unit (tools/tu_map.py unit 475): the node's run. While the base
// run keeps going (2), the point to chase is the target's height kept a margin above the floor (+0xe0),
// the speed factor (+0x2c) snaps to 1 when the height gap is small or the owner has no turn track or faces
// the target, and unless the node is in state 2 the navigation gets a point-target request. Names and
// layouts are placeholders.
#include <math.h>

#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];  // CAIGotoPointNavigationState::SPointTarget's vtable

// The navigation request (vtable 0x7101a25b38), cleared as
// src/game/CChozoCommanderAirChargeAttack/CChozoCommanderAirChargeAttack_Charge_ovb0113.cpp builds it.
struct UnkFlyingChaseParams_l37efb8 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[0x3];
    f32 speed;  // 0x18
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u8 pad_0x38[0x4];
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    u8 pad_0x3f;
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

    UnkFlyingChaseParams_l37efb8() {
        field_0x1c = false;
        vtable = g_unk_7101a25b38 + 2;
        __builtin_memset(field_0x8, 0, 13);
        field_0x3c = false;
        value = base::reflection::refl_EmptyValue();
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        field_0x88 = false;
        field_0x34 = 1;
        field_0x30 = true;
        __builtin_memset(field_0x48, 0, 16);
        __builtin_memset(field_0x20, 0, 16);
    }
    ~UnkFlyingChaseParams_l37efb8() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }
};

// CAIGotoPointNavigationState::SPointTarget: the point to go to.
struct UnkPointTarget_l37efb8 {
    void** vtable;
    UnkVec3f point;

    explicit UnkPointTarget_l37efb8(const UnkVec3f& p) : vtable(g_unk_7101a25b60 + 2), point(p) {}
};

struct UnkFlyingCharClass_l37efb8 {
    u8 pad_0x0[0x40c];
    f32 min_height;     // 0x40c
    f32 height_offset;  // 0x410
};

struct UnkFlyingAI_l37efb8 {
    u8 pad_0x0[0x1610];
    UnkReflObject* char_class;  // 0x1610
};

struct UnkFlyingChaseTargetAction_l37efb8 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x2c - 0x20];
    f32 speed;  // 0x2c
    u8 pad_0x30[0x3c - 0x30];
    UnkVec3f point;  // 0x3c
    u8 pad_0x48[0x60 - 0x48];
    u32 state;  // 0x60
    u8 pad_0x64[0xe0 - 0x64];
    f32 floor;  // 0xe0
};
static_assert(__builtin_offsetof(UnkFlyingChaseTargetAction_l37efb8, floor) == 0xe0, "layout");

s32 fn_710037a7e4(UnkFlyingChaseTargetAction_l37efb8* self);  // the base run

static inline UnkXform& XformOf(UnkActor* actor) {
    return *reinterpret_cast<UnkXform*>(actor->field_0x38);
}

// The AI's character class, when it is a CCharClassAIComponent.
static inline UnkFlyingCharClass_l37efb8* CharClassOf(UnkFlyingAI_l37efb8* ai) {
    if (!ai)
        return nullptr;
    UnkReflObject* obj = ai->char_class;
    if (obj && fn_710009d50c(obj->vf_0x0(), fn_7100408b2c()))
        return reinterpret_cast<UnkFlyingCharClass_l37efb8*>(obj);
    return nullptr;
}

// Whether the owner faces the target along x.
static inline bool FacesTarget(UnkFlyingChaseTargetAction_l37efb8* self, UnkActor* target) {
    if (XformOf(self->owner).GetAngle().y > 0.0f && self->owner->field_0x44 < target->field_0x44)
        return true;
    return XformOf(self->owner).GetAngle().y < 0.0f && self->owner->field_0x44 > target->field_0x44;
}

s32 fn_710037efb8(UnkFlyingChaseTargetAction_l37efb8* self) {
    f32 startY = self->owner->field_0x54;
    s32 status = fn_710037a7e4(self);
    if (status != 2)
        return status;

    UnkActor* owner = self->owner;
    auto* ai = owner ? static_cast<UnkFlyingAI_l37efb8*>(owner->field_0x240) : nullptr;
    UnkFlyingCharClass_l37efb8* char_class = CharClassOf(ai);
    UnkNavigation* navigation = ai ? fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai)) : nullptr;
    UnkActor* target = self->owner ? fn_7100d7c990(self->owner) : nullptr;
    auto* animation = self->owner ? static_cast<UnkAnimationComponent*>(self->owner->field_0x220) : nullptr;
    bool movable = self->owner && self->owner->field_0x218;
    if (!(target && char_class && ai && navigation && animation && movable))
        return status;

    UnkVec3f point = self->point;
    point.y = XformOf(target).GetTranslation().y;
    if (point.y - self->floor <= char_class->min_height)
        point.y = self->floor + char_class->height_offset;

    f32 speed = self->speed;
    if (!(speed + 0.001f >= 1.0f && speed - 0.001f <= 1.0f) && fabsf(startY - point.y) >= 25.0f) {
        if (!fn_710117b348(animation, fn_7100d2b3a8()) || FacesTarget(self, target))
            speed = 1.0f;
    }

    if (self->state != 2) {
        UnkFlyingChaseParams_l37efb8 params;
        params.speed = speed;
        params.field_0x1c = true;
        params.field_0x30 = false;
        params.field_0x88 = true;
        UnkPointTarget_l37efb8 request(point);
        const base::reflection::CClass* cls = fn_7100701c54();
        fn_71000a1c60(&params.value);
        fn_71000a18d4(&params.value, cls, &request, false);
        fn_71006f4b0c(navigation, &params, 0);
    }
    self->speed = speed;
    self->point = point;
    return status;
}
