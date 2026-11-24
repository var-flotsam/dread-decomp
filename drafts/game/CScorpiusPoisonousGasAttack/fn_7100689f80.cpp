// CScorpiusPoisonousGasAttack's unit (tools/tu_map.py unit 1020): the update. After the base update the
// timer (+0x34) runs. While charging (state 0) the attack, once in range and charged for
// fMinChargingTime, starts (state 1) and tells the owner's animation the attack's hash. While attacking
// (state 1) the shock wave grows with the timer until fAttackTime, when it is stopped; the Scorpius AI is
// then flagged as gassing, and as gassing its target when the target's helper agrees (fn_71009e6434).
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CScorpiusAttack.h"
#include "game/UnkActor.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::reflection::UnkReflObject;

void fn_710068685c(CAttack* self, f32 dt);
bool fn_7100689c0c(CAttack* self);
void fn_710080f3c4(void* wave, f32 radius, f32 length);

// CTunableCharClassScorpiusPoisonousGasAttack (its fields by their reflected names).
struct UnkTunable;
extern UnkTunable g_unk_7172702030;
struct UnkPoisonGasTunable_l689f80 {
    u8 pad_0x0[0x21];
    bool bEnabled;
    u8 pad_0x22[2];
    f32 fMinAttackDistance;
    f32 fMaxAttackDistance;
    f32 fAttackTime;                // 0x2c
    f32 fShockWaveLength;           // 0x30
    f32 fShockWaveTimeGrowing;      // 0x34
    f32 fShockWaveTimeAtMaxRadius;  // 0x38
    f32 fMinChargingTime;           // 0x3c
};
static inline const UnkPoisonGasTunable_l689f80& Tunable() {
    return reinterpret_cast<const UnkPoisonGasTunable_l689f80&>(g_unk_7172702030);
}

struct UnkPoisonGasEnd_l689f80;
__attribute__((visibility("hidden"))) extern UnkPoisonGasEnd_l689f80 g_unk_71727034b8;

// What the start sends with its hash.
struct UnkPoisonGasSendValue_l689f80 {
    s64 id;
    f32 time;
    u32 count;
};

struct UnkPoisonGasHashed_l689f80 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkPoisonGasData_l689f80 {
    u8 pad_0x0[0x100];
    UnkPoisonGasHashed_l689f80* attack;  // 0x100
};

struct UnkPoisonGasOwner_l689f80 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

// The Scorpius AI's gas flags (+0x14cf8).
struct UnkScorpiusGasFlags_l689f80 {
    bool gassing;
    bool gassingTarget;
};

struct UnkScorpiusAI_l689f80 {
    u8 pad_0x0[0x14cf8];
    UnkScorpiusGasFlags_l689f80 gas;  // 0x14cf8
};

struct UnkPoisonGasAttack_l689f80 : CAttack {
    u8 pad_0x28[0x8];
    bool spread;  // 0x30
    u8 pad_0x31[3];
    f32 timer;  // 0x34
    u8 pad_0x38[8];
    bool growing;  // 0x40
    u8 pad_0x41[7];
    void* wave;  // 0x48
    u8 pad_0x50[0xc];
    u32 state;                       // 0x5c
    UnkPoisonGasData_l689f80* data;  // 0x60
};
static_assert(__builtin_offsetof(UnkPoisonGasAttack_l689f80, data) == 0x60, "layout");

void fn_7100689f80(UnkPoisonGasAttack_l689f80* self, f32 dt) {
    fn_710068685c(self, dt);
    f32 time = self->timer + dt;
    self->timer = time;
    if (self->state == 0) {
        if (fn_7100689c0c(self) && self->timer >= Tunable().fMinChargingTime) {
            self->state = 1;
            self->timer = 0.0f;
            void* target = reinterpret_cast<UnkPoisonGasOwner_l689f80*>(self->field_0x8)->animation;
            u64 hash = self->data->attack->hash;
            base::reflection::UnkReflTypeIdArg mode;
            UnkPoisonGasSendValue_l689f80 value;
            value.id = -1;
            mode.id = 0;
            value.time = 0.12f;
            value.count = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(target, hash, 0xf4061ff25a2163d6, &mode, &value, &g_unk_7172769740, &mask);
        }
    } else if (self->state == 1) {
        if (self->growing) {
            if (self->spread && time >= Tunable().fAttackTime) {
                fn_7100686960(reinterpret_cast<UnkGasAttack*>(self), &g_unk_71727034b8, 0, -1.0f, -1.0f);
                fn_710080f3c4(self->wave, 0.0f, -1.0f);
                self->growing = false;
            } else {
                fn_710080f3c4(self->wave, 0.0f,
                              time * Tunable().fShockWaveLength / Tunable().fShockWaveTimeGrowing);
            }
        }
        UnkReflObject* object = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
        bool isScorpius = object ? fn_710009d50c(object->vf_0x0(), fn_7100690c74()) : false;
        UnkReflObject* ai = isScorpius ? object : nullptr;
        UnkScorpiusGasFlags_l689f80& gas = reinterpret_cast<UnkScorpiusAI_l689f80*>(ai)->gas;
        gas.gassing = true;
        if (fn_7100d7c990(self->field_0x8)) {
            if (void* helper = fn_7100d7c990(self->field_0x8)->field_0x208) {
                if (fn_71009e6434(static_cast<UnkHelper*>(helper)))
                    gas.gassingTarget = true;
            }
        }
    }
}
