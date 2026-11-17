// CAbilityEnergyWave's unit (tools/tu_map.py unit 429): CAbilityComponent's update. After the base
// update, while the component is active and the game not halted, follows the game manager's flag at
// +0x1af0 (dropping the selected ability when it clears), updates the selection and the input, runs the
// fire-button hold and updates every ability. Names and layouts are placeholders.
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CActorComponent.h"
#include "game/CComponent.h"
#include "game/CGrabComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU2730.h"
#include "types.h"

struct UnkEnergyWaveCompA_b49;
struct UnkEnergyWaveCompB_b49;
struct UnkEnergyWaveObject_b49;

void fn_71002fe5f8(CAbilityComponent* self, f32 dt);
void fn_71002fe9c0(CAbilityComponent* self, f32 dt);
void fn_71002ffcb0(CAbilityComponent* self);

struct UnkAbilityOwner_l2fe82c {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x210 - 0x1e0];
    UnkEnergyWaveCompB_b49* field_0x210;
    u8 pad_0x218[0x260 - 0x218];
    UnkEnergyWaveCompA_b49* field_0x260;
};

// Puts the fire-button hold in `mode`, restarting its time, unless it is in that mode already.
static inline void SetHoldMode(CAbilityComponent* self, s32 mode) {
    if (self->hold.mode != mode) {
        self->hold.mode = mode;
        self->hold.time = 0.0f;
    }
}

void fn_71002fe82c(CAbilityComponent* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    if (!fn_710024fbc8(self) || fn_7101196fd8(g_unk_7101d47330))
        return;
    if (reinterpret_cast<u8*>(g_unk_7101d47310)[0x1af0]) {
        if (!self->field_0xd1)
            self->field_0xd1 = true;
    } else if (self->field_0xd1) {
        self->field_0xd1 = false;
        if (self->current) {
            self->current->Virtual(0x90);
            self->current = nullptr;
        }
        if (self->Owner<UnkAbilityOwner_l2fe82c>()->field_0x260)
            fn_710113f270(self->Owner<UnkAbilityOwner_l2fe82c>()->field_0x260, nullptr);
        if (self->Owner<UnkAbilityOwner_l2fe82c>()->field_0x210)
            fn_710094f8f0(self->Owner<UnkAbilityOwner_l2fe82c>()->field_0x210,
                          reinterpret_cast<UnkEnergyWaveObject_b49*>(self->current));
        fn_71002fda50(self, true, false);
    }
    fn_71002fe5f8(self, dt);
    if (UnkInputComponent* input = self->Owner<UnkAbilityOwner_l2fe82c>()->input) {
        if (!fn_710087238c(input))
            fn_71002fe9c0(self, dt);
    }
    if (self->field_0xd3) {
        SetHoldMode(self, 0);
        self->field_0xd3 = false;
    }
    self->hold.time += dt;
    if (self->hold.mode == 1 && self->hold.time > 0.1f)
        SetHoldMode(self, 0);
    fn_71002ffcb0(self);
    for (CAbility **it = self->begin(), **end = self->end(); it != end; ++it)
        (*it)->Virtual(0x48, dt);
}
