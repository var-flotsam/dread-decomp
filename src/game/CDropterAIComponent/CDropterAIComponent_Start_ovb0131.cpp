// CDropterAIComponent's unit (tools/tu_map.py unit 780): after fn_7100369d80, when the flag at +0x50c of
// the block's object is set and the block's +0x128 flag is still clear, claims a fixed effect hash in the
// owner's (+0x30) effect list (+0x280) and sets the timeline (+0x2e8) id g_unk_7101649cd8 to the all-flags
// mask; then sets the +0x128 flag and runs fn_71004ee590. fn_71004ee750 handles a hit: unless the target
// flag is clear, the hit has one of the flags 0x2008040 or the +0x128 flag is set (then the base handler
// fn_710110cca4 takes it), it stores "bIsDying" in the blackboard section g_unk_71726b7cb8 (as
// SetSectionBool in src/game/CActionSwitcherComponent/CActionSwitcherComponent_b48.cpp), turns the actor
// upright about its y angle (vtable slot 0xd8), switches the +0x1d0 object on (slot 0x168), marks the
// block and the message and runs fn_71004ec714. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkWarriorSend_b58;
void fn_7100369d80(UnkWarriorSend_b58* self, void* arg);
struct UnkDropterHit_b58;
void fn_71004ee590(UnkDropterHit_b58* self);
struct UnkWorld;
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);

struct UnkDropterKey_ovb0131;
extern UnkDropterKey_ovb0131 g_unk_7101649cd8;  // reached through the GOT

struct UnkDropterOwner_ovb0131 {
    u8 pad_0x0[0x280];
    UnkWorld* effects;  // 0x280
    u8 pad_0x288[0x60];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkDropterTarget_ovb0131 {
    u8 pad_0x0[0x50c];
    u8 flag;  // 0x50c
};

struct UnkDropterBlock_ovb0131 {
    UnkDropterTarget_ovb0131* target;
    u8 pad_0x8[0x120];
    u8 started;  // 0x128
    u8 pad_0x129[0x24];
    bool dying;  // 0x14d
    bool dying_hit;  // 0x14e
};

struct UnkDropterAI_ovb0131 {
    u8 pad_0x0[0x30];
    UnkDropterOwner_ovb0131* owner;  // 0x30
    u8 pad_0x38[0x14738 - 0x38];
    UnkDropterBlock_ovb0131 block;  // 0x14738
};

void fn_71004ee434(UnkDropterAI_ovb0131* self, void* arg) {
    UnkDropterBlock_ovb0131* block = &self->block;
    fn_7100369d80(reinterpret_cast<UnkWarriorSend_b58*>(self), arg);
    if ((block->target->flag != 0) & (block->started == 0)) {
        fn_710114fe8c(self->owner->effects, 0xb1fd954dc1d337abull, 0, 0);
        UnkTimelineComponent* timeline = self->owner->timeline;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649cd8), &mask.id, false, -1.0f);
    }
    block->started = 1;
    fn_71004ee590(reinterpret_cast<UnkDropterHit_b58*>(self));
}

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
extern base::global::CStrId g_unk_71726b7cb8;
void fn_710110cca4(void*, void*);
struct UnkDropterHitAI_ovb0131;
void fn_71004ec714(UnkDropterHitAI_ovb0131* self, bool flag);

static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

struct UnkDropterMotor_ovb0131 {
    void** vtable;

    void vf_0x168(bool on, f32 speed) {
        reinterpret_cast<void (*)(UnkDropterMotor_ovb0131*, bool, f32)>(vtable[0x168 / 8])(this, on, speed);
    }
};

struct UnkDropterActor_ovb0131 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x13c];
    UnkDropterMotor_ovb0131* motor;  // 0x1d0

    void vf_0xd8(const UnkVec3f* angle, bool flag) {
        reinterpret_cast<void (*)(UnkDropterActor_ovb0131*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, angle,
                                                                                                    flag);
    }
};
static_assert(__builtin_offsetof(UnkDropterActor_ovb0131, motor) == 0x1d0, "layout");

struct UnkDropterHitMessage_ovb0131 {
    u8 pad_0x0[0x30];
    bool handled;  // 0x30
    u8 pad_0x31[0x83];
    u32 flags;  // 0xb4
};

struct UnkDropterHitAI_ovb0131 {
    u8 pad_0x0[0x30];
    UnkDropterActor_ovb0131* owner;  // 0x30
    u8 pad_0x38[0x1228];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1260[0x14738 - 0x1260 - sizeof(CBlackboard)];
    UnkDropterBlock_ovb0131 block;  // 0x14738
};

void fn_71004ee750(UnkDropterHitAI_ovb0131* self, UnkDropterHitMessage_ovb0131* msg) {
    UnkDropterBlock_ovb0131* block = &self->block;
    if (!block->target->flag || (msg->flags & 0x2008040) || block->started) {
        fn_710110cca4(self, msg);
        return;
    }
    {
        CBlackboard* blackboard = &self->blackboard;
        CStrId name("bIsDying", true);
        bool value = true;
        SetSectionBool(blackboard, g_unk_71726b7cb8, reinterpret_cast<UnkPropertyName*>(&name), &value);
    }
    UnkDropterActor_ovb0131* actor = self->owner;
    UnkDropterMotor_ovb0131* motor = actor->motor;
    UnkVec3f angle = actor->xform.GetAngle();
    UnkVec3f upright;
    upright.y = angle.y;
    upright.z = 0.0f;
    upright.x = 0.0f;
    self->owner->vf_0xd8(&upright, true);
    motor->vf_0x168(true, 1.0f);
    block->dying = true;
    msg->handled = true;
    if (msg->flags & 0x40000)
        block->dying_hit = true;
    fn_71004ec714(self, false);
}
