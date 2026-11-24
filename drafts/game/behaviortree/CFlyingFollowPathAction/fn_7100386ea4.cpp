// behaviortree::CFlyingFollowPathAction's unit (tools/tu_map.py unit 484): the start. After the base start it
// clears the finished flag and the time, marks the track as the action's (+0x50) and, when the owner has a
// character class animation and the action asks for the track change (+0x71), requests it and checks the
// track. For an owner whose AI has a navigation component and enemy movement, it takes the path holder of
// the AI's linked actor and builds the follow-path parameters (the extended kind when +0x72 is set) that
// it hands to the navigation. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAIComponent.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "game/behaviortree/CGoToWallFronPathAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

#include "../../../../src/game_s2/UnkSpitSend.h"

using base::global::CRntVector;
using base::reflection::UnkReflObject;

struct UnkAnimationComponent;
extern char g_unk_7101a264a8[];
void fn_71010fe334(UnkAIComponent* self);
void fn_7100387f88();

struct UnkFlyingTrackName_l386ea4 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};

struct UnkFlyingTrack_l386ea4 {
    u8 pad_0x0[0x38];
    void* field_0x38;
};

// The movement the path holder's AI is built on: where its limits are.
struct UnkFlyingMovement_l386ea4 : UnkReflObject {
    u8 pad_0x8[0x134];
    u32 field_0x13c;
    u32 field_0x140;
};

struct UnkFlyingNavClass_l386ea4 : UnkReflObject {
    u8 pad_0x8[0x6c];
    u8 field_0x74;
};

// The slot at +0x218 of the owner: the movement's holder, with a point at +0x70.
struct UnkFlyingMoverHolder_l386ea4 {
    u8 pad_0x0[0x10];
    UnkReflObject* movement;  // 0x10
    u8 pad_0x18[0x58];
    UnkVec3f field_0x70;
};

struct UnkFlyingNav_l386ea4 {
    u8 pad_0x0[0x10];
    UnkReflObject* charclass;  // 0x10
};

struct UnkFlyingPathHolder_l386ea4 {
    u8 pad_0x0[0x30];
    u8 field_0x30[8];
};

// What the AI's linked part points at: an actor at +0x10.
struct UnkFlyingLink_l386ea4 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkFlyingAI_l386ea4 : UnkReflObject {
    u8 pad_0x8[0x1348];
    u8 part[8];  // 0x1350
    u8 pad_0x1358[0x60];
    u32 field_0x13b8;
};
static_assert(__builtin_offsetof(UnkFlyingAI_l386ea4, field_0x13b8) == 0x13b8, "layout");

// A callback and its object.
struct UnkFlyingCallback_l386ea4 {
    void* object;
    void* function;
};

// A value that may be unset.
struct UnkFlyingOptDir_l386ea4 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    bool set = false;
};

struct UnkFlyingTwoWords_l386ea4 {
    u64 first = 0;
    u64 second = 0;
};

// The follow-path parameter block of kind 2.
struct UnkFlyingParams_l386ea4 {
    void** vtable = reinterpret_cast<void**>(g_unk_7101a264d0 + 0x10);
    UnkFlyingOptDir_l386ea4 field_0x8;
    u32 field_0x18;
    bool has_0x18 = false;
    UnkFlyingTwoWords_l386ea4 field_0x20;
    bool has_0x20 = false;
    u32 mode = 0;
    u64 field_0x38;
    bool has_0x38 = false;
    u32 field_0x44;
    bool has_0x44 = false;
    u32 field_0x4c;
    bool has_0x4c = false;
    u32 field_0x54;
    bool has_0x54 = false;
    u32 field_0x5c;
    bool has_0x5c = false;
    u32 field_0x64;
    bool has_0x64 = false;
    u32 field_0x6c;
    bool has_0x6c = false;
    u32 field_0x74;
    bool has_0x74 = false;
    UnkFlyingOptDir_l386ea4 field_0x7c;
    u32 field_0x8c;
    bool has_0x8c = false;
    u8 field_0x91;
    bool has_0x91 = false;
    u8 field_0x93;
    bool has_0x93 = false;
    u32 field_0x98;
    bool has_0x98 = false;
    u8 field_0x9d;
    bool has_0x9d = false;
    u8 field_0x9f;
    bool has_0x9f = false;
    UnkFlyingTwoWords_l386ea4 field_0xa8;
    bool has_0xa8 = false;
    UnkFlyingTwoWords_l386ea4 field_0xc0;
    bool has_0xc0 = false;
    CRntVector<UnkVec3f> path{0xf};
    bool has_path = false;
    u8 field_0x101;
    bool has_0x101 = false;
    u8 field_0x103;
    bool has_0x103 = false;
    u8 field_0x105;
    bool has_0x105 = false;
};
static_assert(__builtin_offsetof(UnkFlyingParams_l386ea4, mode) == 0x34, "layout");
static_assert(__builtin_offsetof(UnkFlyingParams_l386ea4, path) == 0xd8, "layout");
static_assert(__builtin_offsetof(UnkFlyingParams_l386ea4, has_0x105) == 0x106, "layout");

// The extended kind (4): more optional values.
struct UnkFlyingParamsExt_l386ea4 : UnkFlyingParams_l386ea4 {
    u8 pad_0x107;
    u32 field_0x108;
    bool has_0x108 = false;
    u32 field_0x110;
    bool has_0x110 = false;
    u8 field_0x115;
    bool has_0x115 = false;
    u8 field_0x117;
    bool has_0x117 = false;
    u8 field_0x119;
    bool has_0x119 = false;
    u32 field_0x11c;
    bool has_0x11c = false;
    u32 field_0x124;
    bool has_0x124 = false;
    u8 field_0x129;
    bool has_0x129 = false;
    u32 field_0x12c;
    bool has_0x12c = false;
    u32 field_0x134;
    bool has_0x134 = false;
    u8 field_0x139 = 1;
    bool has_0x139 = true;
    u8 field_0x13b = 1;
    bool has_0x13b = true;
    u32 field_0x140;
    bool has_0x140 = false;
    u32 field_0x144;
    bool has_0x144 = false;
    u32 field_0x148;
    bool has_0x148 = false;
    u8 field_0x14d;
    bool has_0x14d = false;
    u8 field_0x14f;
    bool has_0x14f = false;
    u32 field_0x154;
    bool has_0x154 = false;
    u32 field_0x15c;
    bool has_0x15c = false;
    u32 field_0x164;
    bool has_0x164 = false;
    u32 field_0x16c;
    bool has_0x16c = false;
};

struct UnkFlyingFollow_l386ea4 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0x10];
    void* field_0x30;  // 0x30
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u8 field_0x48;
    u8 pad_0x49[7];
    bool field_0x50;
    u8 field_0x51;
    u8 pad_0x52[6];
    UnkFlyingTrackName_l386ea4* name;  // 0x58
    UnkFlyingCallback_l386ea4 callback;  // 0x60
    bool finished;  // 0x70
    bool track_change;  // 0x71
    bool extended;  // 0x72
    u8 pad_0x73;
    bool field_0x74;
    u8 pad_0x75[3];
    f32 elapsed;  // 0x78
};
static_assert(__builtin_offsetof(UnkFlyingFollow_l386ea4, elapsed) == 0x78, "layout");

void fn_7100386ea4(UnkFlyingFollow_l386ea4* self) {
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->finished = false;
    self->elapsed = 0.0f;
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->actor->field_0x220);
    self->field_0x50 = true;
    if (animation && self->track_change) {
        u64 key = self->name->key;
        mode.id = 0;
        mask.id = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
        fn_7101176690(animation, key, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        UnkFlyingTrack_l386ea4* track =
            static_cast<UnkFlyingTrack_l386ea4*>(fn_7101177d2c(animation, 0xf4061ff25a2163d6));
        self->field_0x50 = track->field_0x38 == self->name;
    }
    UnkFlyingAI_l386ea4* ai = static_cast<UnkFlyingAI_l386ea4*>(self->actor->field_0x240);
    if (!ai)
        return;
    UnkNavigation* navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai));
    UnkFlyingNavClass_l386ea4* nav_class = nullptr;
    if (navigation) {
        if (UnkReflObject* object = reinterpret_cast<UnkFlyingNav_l386ea4*>(navigation)->charclass)
            nav_class = fn_710009d50c(object->vf_0x0(), fn_71006f5fbc())
                            ? static_cast<UnkFlyingNavClass_l386ea4*>(object)
                            : nullptr;
    }
    UnkFlyingMoverHolder_l386ea4* mover = static_cast<UnkFlyingMoverHolder_l386ea4*>(self->actor->field_0x218);
    if (!mover)
        return;
    UnkReflObject* movement_object = mover->movement;
    if (!movement_object)
        return;
    bool is_movement = fn_710009d50c(movement_object->vf_0x0(), fn_710095629c());
    UnkFlyingMovement_l386ea4* movement = is_movement ? static_cast<UnkFlyingMovement_l386ea4*>(movement_object) : nullptr;
    if (!nav_class || !is_movement)
        return;
    fn_71010fe334(reinterpret_cast<UnkAIComponent*>(ai));
    UnkFlyingLink_l386ea4* link =
        *reinterpret_cast<UnkFlyingLink_l386ea4**>(fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(ai->part)));
    if (link && link->actor) {
        UnkFlyingPathHolder_l386ea4* holder =
            reinterpret_cast<UnkFlyingPathHolder_l386ea4*>(fn_7100382150(link->actor));
        self->field_0x30 = holder->field_0x30;
        self->field_0x38 = ai->field_0x13b8;
    }
    if (self->extended) {
        UnkFlyingParamsExt_l386ea4 params;
        params.mode = 4;
        params.vtable = reinterpret_cast<void**>(g_unk_7101a264a8 + 0x10);
        params.field_0x38 = reinterpret_cast<u64>(self->field_0x30);
        params.has_0x38 = true;
        self->callback.object = self;
        self->callback.function = reinterpret_cast<void*>(fn_7100387f88);
        params.field_0x44 = static_cast<UnkFlyingAI_l386ea4*>(self->actor->field_0x240)->field_0x13b8;
        params.has_0x44 = true;
        params.field_0x18 = self->field_0x40;
        params.has_0x18 = true;
        params.field_0x5c = self->field_0x3c;
        params.has_0x5c = true;
        params.field_0x148 = movement->field_0x140;
        params.has_0x148 = true;
        params.field_0x140 = movement->field_0x13c;
        params.has_0x140 = true;
        params.field_0x54 = self->field_0x44;
        params.has_0x54 = true;
        params.field_0x164 = self->field_0x44;
        params.has_0x164 = true;
        params.field_0x93 = self->field_0x51;
        params.has_0x93 = true;
        params.field_0x105 = nav_class->field_0x74;
        params.has_0x105 = true;
        fn_71006f4b0c(navigation, &params, 1);
        params.vtable = reinterpret_cast<void**>(g_unk_7101a264d0 + 0x10);
    } else {
        UnkFlyingParams_l386ea4 params;
        params.mode = 2;
        self->callback.object = self;
        self->callback.function = reinterpret_cast<void*>(fn_7100387f88);
        params.field_0x38 = reinterpret_cast<u64>(self->field_0x30);
        params.has_0x38 = true;
        params.field_0x44 = static_cast<UnkFlyingAI_l386ea4*>(self->actor->field_0x240)->field_0x13b8;
        params.has_0x44 = true;
        params.field_0x18 = self->field_0x40;
        params.has_0x18 = true;
        params.field_0x54 = self->field_0x44;
        params.has_0x54 = true;
        params.field_0x5c = self->field_0x3c;
        params.has_0x5c = true;
        params.field_0x101 = self->field_0x48;
        params.has_0x101 = true;
        if (self->field_0x74) {
            params.field_0x7c.x = mover->field_0x70.x;
            params.field_0x7c.y = mover->field_0x70.y;
            params.field_0x7c.z = mover->field_0x70.z;
            params.field_0x7c.set = true;
        }
        params.field_0x105 = nav_class->field_0x74;
        params.has_0x105 = true;
        params.field_0x20.first = reinterpret_cast<u64>(self->callback.object);
        params.field_0x20.second = reinterpret_cast<u64>(self->callback.function);
        params.has_0x20 = true;
        params.field_0x93 = self->field_0x51;
        params.has_0x93 = true;
        fn_71006f4b0c(navigation, &params, 1);
    }
}
