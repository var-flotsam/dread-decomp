// CDropterAIComponent unit (tools/tu_map.py unit 780): a message handler override. After the base handler
// (fn_7101111dc8), a message with its flag at +0x20 set, while the block's object (+0x14738) is in state 2
// and the block's own state is 0, starts the death: state 3, a fixed effect hash on the owner's effect list
// (+0x280), and, when the owner has its motor (+0x1d0), the collision (+0x2a0) refreshed, "bIsDying" stored
// in the blackboard section g_unk_71726b7cb8 (false), the motor switched on at its own speed, a death
// request sent to the animation (+0x220) and the timeline (+0x2e8) id g_unk_7101649ce0 set to the
// all-flags mask. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkTU2702.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;

struct UnkNailongAI_b46a2;
struct UnkNailongMsg_b46a2;
struct UnkWorld;

struct UnkDropterKey_ovb0131;
extern UnkDropterKey_ovb0131 g_unk_7101649ce0;  // reached through the GOT

static inline void SetSectionBool_l4eebd0(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name,
                                          bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

struct UnkDropterMotor_l4eebd0 {
    void** vtable;
    u8 pad_0x8[0x48];
    f32 speed;  // 0x50

    void vf_0x168(bool on, f32 speed) {
        reinterpret_cast<void (*)(UnkDropterMotor_l4eebd0*, bool, f32)>(vtable[0x168 / 8])(this, on, speed);
    }
};

struct UnkDropterOwner_l4eebd0 {
    u8 pad_0x0[0x1d0];
    UnkDropterMotor_l4eebd0* motor;  // 0x1d0
    u8 pad_0x1d8[0x220 - 0x1d8];
    void* animation;  // 0x220
    u8 pad_0x228[0x280 - 0x228];
    UnkWorld* effects;  // 0x280
    u8 pad_0x288[0x2a0 - 0x288];
    void* collision;  // 0x2a0
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkDropterTarget_l4eebd0 {
    u8 pad_0x0[0x620];
    s32 state;  // 0x620
};

struct UnkDropterBlock_l4eebd0 {
    UnkDropterTarget_l4eebd0* target;
    u8 pad_0x8[0x124];
    s32 state;    // 0x12c
    f32 elapsed;  // 0x130
};

struct UnkDropterAI_l4eebd0 {
    u8 pad_0x0[0x30];
    UnkDropterOwner_l4eebd0* owner;  // 0x30
    u8 pad_0x38[0x1228];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1260[0x14738 - 0x1260 - sizeof(CBlackboard)];
    UnkDropterBlock_l4eebd0 block;  // 0x14738
};
static_assert(__builtin_offsetof(UnkDropterAI_l4eebd0, block) == 0x14738, "layout");

struct UnkDropterMsg_l4eebd0 {
    u8 pad_0x0[0x20];
    bool field_0x20;
};

u32 fn_71004eebd0(UnkDropterAI_l4eebd0* self, UnkDropterMsg_l4eebd0* msg) {
    u32 result =
        fn_7101111dc8(reinterpret_cast<UnkNailongAI_b46a2*>(self), reinterpret_cast<UnkNailongMsg_b46a2*>(msg));
    UnkDropterBlock_l4eebd0* block = &self->block;
    if (msg->field_0x20 && block->target->state == 2 && block->state == 0) {
        block->state = 3;
        block->elapsed = 0.0f;
        fn_710114fe8c(self->owner->effects, 0x1865cf2096e1d55dull, 0, 0);
        UnkDropterMotor_l4eebd0* motor = self->owner->motor;
        if (!motor)
            return result;
        if (void* collision = self->owner->collision)
            fn_71008618cc(collision);
        {
            CBlackboard* blackboard = &self->blackboard;
            CStrId name("bIsDying", true);
            bool value = false;
            SetSectionBool_l4eebd0(blackboard, g_unk_71726b7cb8, reinterpret_cast<UnkPropertyName*>(&name), &value);
        }
        motor->vf_0x168(true, motor->speed);
        if (void* animation = self->owner->animation) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 2;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0xe2ea001660c8ac66ull, 0xf4061ff25a2163d6ull, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
        }
        if (UnkTimelineComponent* timeline = self->owner->timeline) {
            base::reflection::UnkReflTypeIdArg all;
            all.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649ce0), &all.id, false, -1.0f);
        }
    }
    return result;
}
