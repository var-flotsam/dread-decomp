// behaviortree::CQuetzoaFollowInPathAction's unit (tools/tu_map.py, unit 969): runs the node. Before the path is
// followed it only turns the owner to face the target; while following, it keeps a timer of how long the owner
// has faced one way and hands the navigation object the path's next point. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a59f90[];
extern UnkTunable g_unk_71726fbbf0;

// CCharClassQuetzoaAIComponent's tunable: +0x24 is how long the owner may face away before it turns.
struct UnkQuetzoaTunable_l646b48 : UnkTunable {
    u8 pad_0x8[0x1c];
    f32 turnDelay;  // 0x24
};

struct UnkPoint_l646b48 {
    f32 x;
    f32 y;
    f32 z;
};

// A point that may be unset.
struct UnkOptionalPoint_l646b48 {
    UnkPoint_l646b48 value;
    bool set;

    UnkOptionalPoint_l646b48() : value(), set(false) {}
    void Set(const UnkPoint_l646b48& point) {
        value = point;
        set = true;
    }
    void Set(f32 x, f32 y, f32 z) {
        value.x = x;
        value.y = y;
        value.z = z;
        set = true;
    }
};

// The kind-1 block handed to the navigation object: a facing.
struct UnkFaceParams_l646b48 {
    void** vtable;
    UnkOptionalPoint_l646b48 facing;  // 0x8
    u8 pad_0x18[0x4];
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 kind;  // 0x34
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

    UnkFaceParams_l646b48() {
        field_0x1c = false;
        vtable = g_unk_7101a25b38 + 2;
        field_0x3c = false;
        value = base::reflection::refl_EmptyValue();
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        field_0x88 = false;
        kind = 1;
        field_0x30 = false;
        __builtin_memset(field_0x48, 0, 16);
        __builtin_memset(field_0x20, 0, 16);
    }
    ~UnkFaceParams_l646b48() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }
};

// The kind-3 block handed to the navigation object: a point to go to, and a facing.
struct UnkGotoParams_l646b48 {
    void** vtable;
    UnkOptionalPoint_l646b48 facing;  // 0x8
    u8 pad_0x18[0x4];
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 kind;  // 0x34
    u8 pad_0x38[0x8];
    bool field_0x40;
    u8 pad_0x41[0x7];
    bool field_0x48;
    u8 pad_0x49[0x7];
    bool field_0x50;
    u8 pad_0x51;
    bool field_0x52;
    u8 pad_0x53;
    UnkOptionalPoint_l646b48 point;  // 0x54

    UnkGotoParams_l646b48() {
        field_0x1c = false;
        field_0x30 = false;
        __builtin_memset(field_0x20, 0, 16);
        vtable = g_unk_7101a59f90 + 2;
        kind = 3;
        field_0x40 = false;
        field_0x48 = false;
        field_0x50 = false;
        field_0x52 = false;
    }
};

// The owner's CQuetzoaAIComponent.
struct UnkQuetzoaAI_l646b48 : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* charClass;  // 0x10
    u8 pad_0x18[0x14af4 - 0x18];
    UnkPoint_l646b48 pathPoint;  // 0x14af4

    f32 TurnDelay() {
        UnkReflObject* cc = charClass;
        if (cc)
            fn_710009d50c(cc->vf_0x0(), fn_7100648ce0());
        return static_cast<UnkQuetzoaTunable_l646b48&>(g_unk_71726fbbf0).turnDelay;
    }
};

bool fn_710064b3f4(UnkQuetzoaAI_l646b48* ai);

struct UnkPathFollower_l646b48 {
    u8 pad_0x0[0x70];
    f32 direction;  // 0x70
};

struct UnkQuetzoaOwner_l646b48 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f0 - 0x78];
    UnkPathFollower_l646b48* pathFollower;  // 0x1f0
    u8 pad_0x1f8[0x240 - 0x1f8];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x298 - 0x248];
    void* navigation;  // 0x298
};
static_assert(__builtin_offsetof(UnkQuetzoaOwner_l646b48, navigation) == 0x298, "");

struct UnkQuetzoaFollowInPathAction_l646b48 {
    u8 pad_0x0[0x18];
    UnkQuetzoaOwner_l646b48* owner;  // 0x18
    u8 pad_0x20[0xc];
    f32 timer;  // 0x2c
    f32 side;   // 0x30
    bool following;  // 0x34
};

static inline f32 Sign(f32 x) { return (f32)((x > 0.0f) - (x < 0.0f)); }

static inline UnkActor* Target(UnkQuetzoaOwner_l646b48* owner) {
    return fn_7100d7c990(reinterpret_cast<UnkActor*>(owner));
}

static inline f32 PosX(UnkQuetzoaOwner_l646b48* owner) { return owner->transform.m[3]; }

s32 fn_7100646b48(UnkQuetzoaFollowInPathAction_l646b48* self, f32 dt) {
    void* navigation = self->owner->navigation;
    UnkActor* target = Target(self->owner);
    if (!navigation || !target)
        return 6;

    bool following = self->following;
    f32 facing = fn_710002614c(&self->owner->transform).m[2];
    if (!following) {
        if (__builtin_fabsf(facing) > 0.99f) {
            UnkFaceParams_l646b48 params;
            params.facing.Set(target->field_0x44 - PosX(self->owner) >= 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);
            fn_71006f4b0c(navigation, &params, 0);
        }
        return 2;
    }

    if (self->side != Sign(facing)) {
        f32 z = fn_710002614c(&self->owner->transform).m[2];
        self->timer = 0.0f;
        self->side = Sign(z);
    } else {
        self->timer += dt;
    }

    UnkReflObject* object = self->owner->ai;
    if (!object)
        return 2;
    UnkQuetzoaAI_l646b48* ai = fn_710009d50c(object->vf_0x0(), fn_7100649630())
                                   ? static_cast<UnkQuetzoaAI_l646b48*>(object)
                                   : nullptr;
    if (!ai)
        return 2;
    UnkPathFollower_l646b48* path = self->owner->pathFollower;
    if (!path)
        return 2;

    UnkPoint_l646b48 point = ai->pathPoint;
    if (__builtin_fabsf(point.x - PosX(self->owner)) < 10.0f)
        fn_7101105e14(reinterpret_cast<UnkQuetzoaDodgeAI_A3_b76*>(ai), 0.0f);

    UnkGotoParams_l646b48 params;
    params.point.Set(point);
    f32 side = Sign(Target(self->owner)->field_0x44 - PosX(self->owner));
    UnkQuetzoaAI_l646b48* quetzoa = static_cast<UnkQuetzoaAI_l646b48*>(object);
    if (side != Sign(path->direction) && self->timer < quetzoa->TurnDelay() && fn_710064b3f4(quetzoa)) {
        self->timer = 3.40282347e+38f;
    } else if (__builtin_fabsf(fn_710002614c(&self->owner->transform).m[6]) < 0.1f) {
        params.facing.Set(side, 0.0f, 0.0f);
    }
    fn_71006f4b0c(navigation, &params, 0);
    return 2;
}
