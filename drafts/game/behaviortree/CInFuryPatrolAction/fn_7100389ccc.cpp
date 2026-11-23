// behaviortree::CInFuryPatrolAction's unit (tools/tu_map.py unit 486): a step of the action. It ends (3) while
// the actor's movement is in state 4 or its animation plays a turn track; otherwise it sends the animation the
// track named at +0x90 once, ends when that track has stopped, clears the owner's blackboard flag when it is
// set and returns 7. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;

struct UnkInFuryOwner_l389ccc {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;  // 0x1260
};

struct UnkInFuryActor_l389ccc {
    u8 pad_0x0[0x218];
    void* field_0x218;
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkInFuryAction_l389ccc {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkInFuryActor_l389ccc* actor;  // 0x18
    u8 pad_0x20[0x10];
    UnkInFuryOwner_l389ccc* owner;  // 0x30
    u8 pad_0x38[0x58];
    base::UnkStrIdMember field_0x90;
    u8 pad_0x98[8];
    bool field_0xa0;
    u8 pad_0xa1[7];
    bool field_0xa8;
};
static_assert(__builtin_offsetof(UnkInFuryAction_l389ccc, field_0xa8) == 0xa8, "layout");

static const u64 kTrackKind_l389ccc = 0xf4061ff25a2163d6;

static inline const CStrId& Id_l389ccc(u32 index) {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[index];
}

// Stores `value` under `name` in the blackboard section `section` names, unless the blackboard is locked.
static inline void SetFlag_l389ccc(CBlackboard* blackboard, const CStrId& section, const CStrId& name,
                                   bool value) {
    UnkTypedValue v;
    bool stored = value;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_710000d228();
            v.value = &stored;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<CStrId*>(&name)), &v);
        }
    }
}

s32 fn_7100389ccc(UnkInFuryAction_l389ccc* self) {
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    UnkInFuryActor_l389ccc* actor = self->actor;
    UnkAnimationComponent* animation = actor ? actor->animation : nullptr;
    void* movement = actor->field_0x218;
    if ((movement && fn_71010cc3a4(movement) == 4) ||
        (animation && fn_710117b348(animation, fn_7100d2b3a8()))) {
        self->field_0xa8 = false;
        return 3;
    }
    u64 track = self->field_0x90.data->Key();
    if (!self->field_0xa0) {
        CStrId current = *fn_7101177ae8(animation, kTrackKind_l389ccc);
        if (current.data->Key() == 0xe2ea001660c8ac66)
            self->field_0xa8 = false;
    }
    if (track != (u64)-1 && animation && self->field_0xa8 && !self->field_0xa0) {
        mode.id = 0;
        mask.id = UnkAll_SpitSend().value;
        if (fn_7101176690(animation, track, kTrackKind_l389ccc, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask))
            self->field_0xa0 = true;
    }
    if (animation && self->field_0xa0) {
        if (fn_7101177ae8(animation, kTrackKind_l389ccc)->data->Key() == track &&
            !fn_71011782ec(animation, kTrackKind_l389ccc))
            return 3;
    }
    bool value = false;
    if (UnkInFuryOwner_l389ccc* owner = self->owner) {
        CBlackboard* blackboard = &owner->blackboard;
        const CStrId& name = Id_l389ccc(0x11e);
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, g_unk_71726b7cb8)) {
            bool found = fn_710006d11c(section, &name, &value);
            if (value && found)
                SetFlag_l389ccc(blackboard, g_unk_71726b7cb8, Id_l389ccc(0x11e), false);
        }
    }
    return 7;
}
