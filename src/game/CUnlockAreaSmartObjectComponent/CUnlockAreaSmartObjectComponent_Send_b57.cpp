// CUnlockAreaSmartObjectComponent unit (tools/tu_map.py): with a player (fn_7100dec788), sends the owner's
// (+0x30) sub-object at +0x220 the hash of the object at +0x70 with mode 1 (as UnkSpitSend,
// src/game_s2/UnkSpitSend.h, with the mode written out), forwards an input request (fn_71008724b4, with
// the tunable's +0x24) to the player's input component (+0x1d8) and clears +0x150, and asks the player's
// +0x1f0 component, when it is of the class fn_710093b204 describes, for mode 1 (fn_710093fa80); then
// resets the id at +0x148 to the empty id. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkInputComponent;

struct UnkUnlockAreaTunable_b57 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
};
// As src/reflection/CUnlockAreaSmartObjectComponent.cpp declares it (the unit's tunable).
extern base::reflection::UnkTunable g_unk_7172743078;
extern __attribute__((visibility("hidden"))) char g_unk_71727430f0[];

void fn_71008724b4(f32 time, UnkInputComponent* self, void* arg, bool reset);

struct UnkUnlockAreaComp_b57 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkUnlockAreaComp_b57*)>(vtable[0])(this);
    }
};
void fn_710093fa80(UnkUnlockAreaComp_b57* self, s32 mode, bool a, bool b);

struct UnkUnlockAreaPlayer_b57 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;
    u8 pad_0x1e0[0x10];
    UnkUnlockAreaComp_b57* field_0x1f0;
};

struct UnkUnlockAreaId_b57 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkUnlockAreaOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkUnlockAreaSmartObject_b57 {
    u8 pad_0x0[0x30];
    UnkUnlockAreaOwner_b57* owner;
    u8 pad_0x38[0x38];
    UnkUnlockAreaId_b57* field_0x70;
    u8 pad_0x78[0x148 - 0x78];
    base::global::CStrId field_0x148;
    bool field_0x150;
};

void fn_7100a0782c(UnkUnlockAreaSmartObject_b57* self) {
    auto* player = reinterpret_cast<UnkUnlockAreaPlayer_b57*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        if (void* target = self->owner->field_0x220) {
            u64 hash = self->field_0x70->hash;
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
        }
        if (player->input) {
            f32 time = reinterpret_cast<UnkUnlockAreaTunable_b57*>(&g_unk_7172743078)->field_0x24;
            fn_71008724b4(time, player->input, g_unk_71727430f0, false);
            self->field_0x150 = false;
        }
        if (UnkUnlockAreaComp_b57* comp = player->field_0x1f0) {
            if (fn_710009d50c(comp->vf_0x0(), fn_710093b204()))
                fn_710093fa80(comp, 1, true, true);
        }
    }
    self->field_0x148 = g_unk_7101d470e0;
}
