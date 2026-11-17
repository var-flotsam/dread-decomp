// CAIAttackComponent unit (tools/tu_map.py unit 2705): with an attack and an AI component, hands the attack to
// the AI (fn_7101105990), runs the attack's two timing queries into the AI, updates the component, notifies the
// AI unless flagged, and clears the AI's crazy-track flag when the owner is not a crazy track.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkAIAttackOwner_l13430c;
struct UnkAIAttackAI_l13430c;

struct UnkAIAttack_l13430c {
    virtual void vf_0x0();
    f32 vf_0xa0(UnkAIAttackOwner_l13430c* owner, u32 id) {
        return reinterpret_cast<f32 (*)(UnkAIAttack_l13430c*, UnkAIAttackOwner_l13430c*, u32)>(
            (*reinterpret_cast<void***>(this))[0xa0 / 8])(this, owner, id);
    }
    f32 vf_0xa8(UnkAIAttackOwner_l13430c* owner, u32 id) {
        return reinterpret_cast<f32 (*)(UnkAIAttack_l13430c*, UnkAIAttackOwner_l13430c*, u32)>(
            (*reinterpret_cast<void***>(this))[0xa8 / 8])(this, owner, id);
    }
};

struct UnkAIAttackAI_l13430c {
    virtual void vf_0x0();
    void vf_0x290(UnkAIAttack_l13430c* attack) {
        reinterpret_cast<void (*)(UnkAIAttackAI_l13430c*, UnkAIAttack_l13430c*)>(
            (*reinterpret_cast<void***>(this))[0x290 / 8])(this, attack);
    }
    u8 pad_0x8[0x1140 - 0x8];
    u32 field_0x1140;
    u8 pad_0x1144[0x179e - 0x1144];
    bool field_0x179e;
};
static_assert(__builtin_offsetof(UnkAIAttackAI_l13430c, field_0x179e) == 0x179e, "layout");

struct UnkAIAttackOwner_l13430c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    UnkAIAttackAI_l13430c* ai;
};

struct UnkAIAttackComponent_l13430c {
    u8 pad_0x0[0x30];
    UnkAIAttackOwner_l13430c* owner;
    u8 pad_0x38[0x4a - 0x38];
    bool field_0x4a;
    u8 pad_0x4b[0x1a0 - 0x4b];
    UnkAIAttack_l13430c* attack;
    bool field_0x1a8;
};

void fn_7101105990(UnkAIAttackAI_l13430c* ai, UnkAIAttack_l13430c* attack, bool flag);
// Declared as its other caller has it (CQuetzoaAIComponent): the AI component.
struct UnkQuetzoaDodgeAI_A3_b76;
void fn_7101105e14(UnkQuetzoaDodgeAI_A3_b76* self, f32 value);
void fn_7101105e1c(UnkAIAttackAI_l13430c* ai, f32 value);
void fn_710113789c(UnkAIAttackComponent_l13430c* self);

void fn_710113430c(UnkAIAttackComponent_l13430c* self) {
    UnkAIAttack_l13430c* attack = self->attack;
    UnkAIAttackAI_l13430c* ai = self->owner->ai;
    if (attack && ai) {
        fn_7101105990(ai, attack, self->field_0x1a8);
        fn_7101105e14(reinterpret_cast<UnkQuetzoaDodgeAI_A3_b76*>(ai), self->attack->vf_0xa0(self->owner, ai->field_0x1140));
        fn_7101105e1c(ai, self->attack->vf_0xa8(self->owner, ai->field_0x1140));
    }
    fn_710113789c(self);
    if (!self->field_0x4a && ai)
        ai->vf_0x290(attack);
    if (UnkAnimationComponent* anim = self->owner->field_0x220) {
        bool is_track = fn_710117b348(anim, fn_7100cf3508());
        if (!is_track && ai)
            ai->field_0x179e = false;
    }
}
