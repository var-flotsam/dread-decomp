// behaviortree::CStopInIdleAction's unit (tools/tu_map.py unit 489): the node's start. After the base start
// it notes whether its value (+0x3c) is about -1, takes the owner's navigation (+0x298) and, unless the owner
// is being hit (movement mode 4, or the AI's flags) or turning, stops the navigation (a kind-5 parameter
// block) and plays the idle animation: the name at +0x48, overridden by the blackboard value the id at +0x50
// names. Names and layouts are placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU57.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::global::CStrId;

// An animation name built from a string id (fn_710007eb6c).
struct UnkIdleAnimName_l38dd0c : UnkBufferString<0x100, g_unk_7101a194b8> {
    explicit UnkIdleAnimName_l38dd0c(const CStrId* id) { fn_710007eb6c(Str(), id); }
};

struct UnkIdleAI_l38dd0c {
    u8 pad_0x0[0x14676];
    bool field_0x14676;
    bool field_0x14677;
};

struct UnkIdleOwner_l38dd0c {
    u8 pad_0x0[0x218];
    void* movement;                    // 0x218
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkIdleAI_l38dd0c* ai;  // 0x240
    u8 pad_0x248[0x50];
    void* navigation;  // 0x298
};

struct UnkStopInIdleAction_l38dd0c {
    u8 pad_0x0[0x18];
    UnkIdleOwner_l38dd0c* owner;  // 0x18
    u8 pad_0x20[0x10];
    void* navigation;  // 0x30
    bool hit;          // 0x38
    bool field_0x39;
    bool turning;  // 0x3a
    bool field_0x3b;
    f32 field_0x3c;
    u32 field_0x40;
    u8 pad_0x44[4];
    CStrId anim;           // 0x48
    CStrId anim_property;  // 0x50
};

// Signatures as other files declare them.
struct UnkStringEqualsCondition_A2_b72;
struct UnkInlineString_A2_b72;

static inline bool NearlyEqual(f32 value, f32 target, f32 tolerance) {
    bool below = value - tolerance <= target;
    bool above = value + tolerance >= target;
    return above & below;
}

void fn_710038dd0c(UnkStopInIdleAction_l38dd0c* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->field_0x3b = NearlyEqual(self->field_0x3c, -1.0f, 0.1f);
    self->hit = false;
    self->turning = false;
    self->field_0x40 = 0;
    self->navigation = self->owner->navigation;
    if (!self->navigation)
        return;

    UnkIdleOwner_l38dd0c* owner = self->owner;
    if ((owner->movement && fn_71010cc3a4(owner->movement) == 4) ||
        (self->owner->ai && self->owner->ai->field_0x14676)) {
        UnkAnimationComponent* animation = self->owner->animation;
        if ((animation && fn_710117b348(animation, fn_7100bf4588())) ||
            (self->owner->ai && self->owner->ai->field_0x14677))
            self->field_0x39 = true;
        self->hit = true;
    }
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100d2b3a8())) {
        self->turning = true;
        return;
    }
    if (self->turning || self->hit || self->field_0x39)
        return;

    UnkParams params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x30 = 0;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    fn_71006f4b0c(self->navigation, &params, 1);

    UnkIdleAnimName_l38dd0c name(&self->anim);
    if (self->anim_property.data != &g_unk_7101d47108->empty)
        fn_7100395044(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->anim_property,
                      reinterpret_cast<UnkInlineString_A2_b72*>(&name));
    const char* str = name.Str()->data;
    if (str && *str && self->owner) {
        if (UnkAnimationComponent* target = self->owner->animation) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 1;
            u64 key = name.Str()->key;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, key, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                          &mask);
        }
    }
}
