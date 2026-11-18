// CInfesterBallAIComponent's unit (tools/tu_map.py, unit 898): the per-frame update. After the base update it
// runs the pending requests, advances the roll timer and the looping phase (shown on the model's vector), blends
// towards 1 over the roll, and starts or stops rolling from the movement's state. Names and layouts are
// placeholders.
#include "base/math/CVector4D.h"
#include "game/CEnemyMovement.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::math::CVector4D;

struct UnkInfesterBallAI_b60;
struct UnkInfesterBallAI_ovb0128;

struct UnkInfesterBallState_l5cf514 {
    u8 pad_0x0[0x70];
    s32 state;
};

struct UnkInfesterBallVecSlot_l5cf514 {
    u8 pad_0x0[0x30];
    CVector4D* value;
};

struct UnkInfesterBallVecOwner_l5cf514 {
    u8 pad_0x0[0x240];
    CVector4D vec;  // 0x240
    u8 pad_0x250[0x2e0 - 0x250];
    UnkInfesterBallVecSlot_l5cf514* slot;  // 0x2e0

    void SetVec(const CVector4D& v) {
        fn_710007b518(&vec, &v);
        if (slot)
            *slot->value = v;
    }
};

struct UnkInfesterBallLevel4_l5cf514 {
    u8 pad_0x0[0x48];
    UnkInfesterBallVecOwner_l5cf514** owner;
};

struct UnkInfesterBallLevel3_l5cf514 {
    u8 pad_0x0[0x78];
    UnkInfesterBallLevel4_l5cf514* field_0x78;
};

struct UnkInfesterBallLevel2_l5cf514 {
    u8 pad_0x0[0x138];
    UnkInfesterBallLevel3_l5cf514* field_0x138;
};

struct UnkInfesterBallLevel1_l5cf514 {
    u8 pad_0x0[0x50];
    UnkInfesterBallLevel2_l5cf514* field_0x50;
};

struct UnkInfesterBallAI_l5cf514 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x146c8 - 0x38];
    UnkMovement_71010d08d4* movement;  // 0x146c8
    u8 pad_0x146d0[0x146e8 - 0x146d0];
    f32 duration;  // 0x146e8
    f32 time;      // 0x146ec
    u8 pad_0x146f0[0x14708 - 0x146f0];
    u32 state;  // 0x14708
    u8 pad_0x1470c;
    bool reverse;  // 0x1470d
    u8 pad_0x1470e;
    bool field_0x1470f;
    bool field_0x14710;
    u8 pad_0x14711;
    bool field_0x14712;
    bool field_0x14713;
    u8 pad_0x14714[0x14760 - 0x14714];
    f32 phase;                             // 0x14760
    u8 target[0x10];                       // 0x14764
    f32 blend;                             // 0x14774
};
static_assert(__builtin_offsetof(UnkInfesterBallAI_l5cf514, target) == 0x14764, "target");
static_assert(__builtin_offsetof(UnkInfesterBallAI_l5cf514, blend) == 0x14774, "blend");

// From `a` (t = 0) to `b` (t = 1).
inline f32 Lerp(f32 a, f32 b, f32 t) {
    return t * b + (1.0f - t) * a;
}

void fn_71005cf72c(UnkInfesterBallAI_l5cf514* self);
void fn_71005d013c(UnkInfesterBallAI_l5cf514* self);
void fn_71005cfa94(UnkInfesterBallAI_b60* self, bool flag);
void fn_71005cfdc0(UnkInfesterBallAI_l5cf514* self, bool flag);
void fn_71005cffd0(UnkInfesterBallAI_ovb0128* self);

void fn_71005cf514(UnkInfesterBallAI_l5cf514* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    if (static_cast<UnkInfesterBallState_l5cf514*>(self->actor->field_0x1b8)->state == 2)
        return;

    if (self->field_0x1470f || self->field_0x14710)
        fn_71005cf72c(self);
    if (self->field_0x14712) {
        self->field_0x14712 = false;
        fn_71010d08d4(self->movement, reinterpret_cast<const UnkMovementVec_71010d08d4*>(self->target), true);
    }
    if (self->field_0x14713) {
        self->field_0x14713 = false;
        fn_71005d013c(self);
    }

    if (self->state < 2) {
        self->time += dt;
        f32 step = dt * 2.0f;
        f32 phase;
        if (self->reverse) {
            phase = self->phase - step;
            self->phase = phase;
            if (phase < 0.0f) {
                phase += 1.0f;
                self->phase = phase;
            }
        } else {
            phase = step + self->phase;
            self->phase = phase;
            if (phase > 1.0f) {
                phase -= 1.0f;
                self->phase = phase;
            }
        }

        if (auto* level1 = static_cast<UnkInfesterBallLevel1_l5cf514*>(self->actor->field_0x228)) {
            if (UnkInfesterBallLevel2_l5cf514* level2 = level1->field_0x50) {
                UnkInfesterBallVecOwner_l5cf514* owner = *level2->field_0x138->field_0x78->owner;
                CVector4D v;
                v.x = phase;
                v.y = 0.0f;
                v.z = 0.0f;
                v.w = 1.0f;
                owner->SetVec(v);
            }
        }

        if (self->time >= self->duration)
            fn_71005cfa94(reinterpret_cast<UnkInfesterBallAI_b60*>(self), false);

        f32 t = self->time / self->duration;
        f32 blend;
        if (t >= 1.0f)
            blend = 1.0f;
        else if (t < 0.0f)
            blend = self->blend;
        else
            blend = Lerp(self->blend, 1.0f, t);
        self->blend = blend;
    }

    if (self->state != 2 && self->actor->field_0x218) {
        u32 first = fn_71010cc3a4(self->actor->field_0x218);
        u32 second = fn_71010cc3a4(self->actor->field_0x218);
        if (first == 1 && self->state == 0)
            fn_71005cfdc0(self, false);
        else if (second == 2 && self->state == 1)
            fn_71005cffd0(reinterpret_cast<UnkInfesterBallAI_ovb0128*>(self));
    }
}
