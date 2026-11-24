// CAbilityOpticCamouflage (unit 432): the cloak's per-frame drain, before the colour pulse. The special
// energy drains at the tunable's rate (faster while fn_7100a1f84c holds); once it is empty, life drains
// too (never below 1), and the cloak's timeline event follows the energy running out or coming back.
// Names and layouts are placeholders.
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityOpticCamouflage.h"
#include "game/CSpecialEnergyComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkOpticAbility_ovb0059;
struct UnkSpecialEnergyComponent;
struct UnkLifeValues;
struct UnkTarget429;

bool CLifeComponent_SubtractLifeAmount(UnkLifeValues* self, f32 amount);
void CAbilityOpticCamouflage_PulseModelColor(UnkOpticAbility_ovb0059* self, f32 dt);

extern const u64 g_unk_7101648e28;  // the timeline event played when the energy runs out
extern const u64 g_unk_7101648e30;  // the one played when it comes back

// The fields of the camouflage tunable read here.
struct UnkOpticTunable_l323f10 {
    u8 pad_0x0[0x24];
    f32 energy_drain;       // 0x24
    f32 energy_drain_fast;  // 0x28
    u8 pad_0x2c[0x4];
    f32 life_drain;         // 0x30
    f32 life_drain_fast;    // 0x34
};

// The fields of the special energy tunable read here.
struct UnkEnergyTunable_l323f10 {
    u8 pad_0x0[0x30];
    f32 recover_time_fast;  // 0x30
    f32 recover_time;       // 0x34
};

static inline const UnkOpticTunable_l323f10& OpticTunable() {
    return reinterpret_cast<const UnkOpticTunable_l323f10&>(g_unk_71726affc0);
}

// The drain per second, the fast one while fast holds.
static inline f32 Drain(bool fast, const f32& rate_fast, const f32& rate) {
    if (fast)
        return rate_fast;
    return rate;
}

static inline const UnkEnergyTunable_l323f10& EnergyTunable() {
    return reinterpret_cast<const UnkEnergyTunable_l323f10&>(g_unk_7172745770);
}

struct UnkOpticLife_l323f10 {
    u8 pad_0x0[0x54];
    f32 life;  // 0x54
    u8 pad_0x58[0x4];
    bool field_0x5c;
};

struct UnkOpticEnergy_l323f10 {
    u8 pad_0x0[0x44];
    f32 energy;  // 0x44
    u8 pad_0x48[0x8];
    f32 recover_time_fast;  // 0x50
    f32 recover_time;       // 0x54
};

struct UnkOpticActor_l323f10 {
    u8 pad_0x0[0x1b8];
    UnkOpticLife_l323f10* life;  // 0x1b8
};

struct UnkOpticPart_l323f10 {
    u8 pad_0x0[0x10];
    UnkOpticEnergy_l323f10* energy;  // 0x10
};

struct UnkOpticOwner_l323f10 {
    u8 pad_0x0[0x30];
    UnkOpticActor_l323f10* actor;  // 0x30
    u8 pad_0x38[0xb0];
    UnkOpticPart_l323f10* part;  // 0xe8
};

struct UnkOpticAbility_l323f10 {
    u8 pad_0x0[0xc0];
    UnkOpticOwner_l323f10* owner;  // 0xc0
    u8 pad_0xc8[0x398 - 0xc8];
    bool out_of_energy;  // 0x398
};

void CAbilityOpticCamouflage_ConsumeEnergyAndDamageLife(UnkOpticAbility_l323f10* self, f32 dt) {
    UnkOpticOwner_l323f10* owner = self->owner;
    if (owner->part) {
        if (UnkOpticEnergy_l323f10* energy = owner->part->energy) {
            UnkOpticActor_l323f10* actor = owner->actor;
            bool fast = fn_7100a1f84c(reinterpret_cast<UnkSpecialEnergyComponent*>(energy));
            const UnkOpticTunable_l323f10& tunable = OpticTunable();
            if (energy->energy <= 0.0f) {
                UnkOpticLife_l323f10* life = actor->life;
                if (!life->field_0x5c) {
                    f32 amount = Drain(fast, tunable.life_drain_fast, tunable.life_drain) * dt;
                    f32 limit = life->life - 1.0f;
                    f32 damage = amount >= limit ? limit : amount;
                    if (amount <= 0.0f)
                        damage = 0.0f;
                    CLifeComponent_SubtractLifeAmount(reinterpret_cast<UnkLifeValues*>(life), damage);
                }
            }
            fn_7100a1feac(reinterpret_cast<UnkTarget429*>(energy),
                          Drain(fast, tunable.energy_drain_fast, tunable.energy_drain) * dt);
            energy->recover_time_fast = EnergyTunable().recover_time_fast;
            energy->recover_time = EnergyTunable().recover_time;
            if (energy->energy <= 0.0f) {
                if (!self->out_of_energy) {
                    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner->actor), &g_unk_7101648e28, false);
                    self->out_of_energy = true;
                }
            } else if (self->out_of_energy) {
                fn_7100d7c828(reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310)), &g_unk_7101648e30,
                              false);
                self->out_of_energy = false;
            }
        }
    }
    CAbilityOpticCamouflage_PulseModelColor(reinterpret_cast<UnkOpticAbility_ovb0059*>(self), dt);
}
