// CEmmyGrabSamusAttack's unit: releases the grab. Moves the state machine to its first state, takes the
// "EmmyGrab" key off the E.M.M.I. registry, resets both characters' track 0xe, restores the player,
// fades back in and refreshes every music track. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAnimationComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CCameraComponent.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/CInputComponent.h"
#include "game/UnkActor.h"
#include "game/UnkEmmyGrabSamusAttack.h"
#include "game/UnkGameManager.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU2553.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkDoorKeyOwner_dae8a4;
void fn_710116a158(void* self, CStrId* id);
extern CStrId g_unk_71726de5c8 __attribute__((visibility("hidden")));

struct UnkGrabPlayer_l507d24 {
    u8 pad_0x0[0x1c0];
    void* field_0x1c0;
    u8 pad_0x1c8[0x10];
    UnkInputComponent* input;  // 0x1d8
};

static inline UnkMgr_0xf18* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return static_cast<UnkMgr_0xf18*>(manager);
}

struct UnkMusicTracks_l507d24 {
    u8 pad_0x0[0x250];
    s32 count;
};

void fn_7100507d24(UnkGrabAttack_b58a3* self) {
    self->state_machine.Request(1);
    static CStrId s_key("EmmyGrab", true);
    fn_7100dae8a4(reinterpret_cast<UnkDoorKeyOwner_dae8a4*>(g_unk_71726af0a8), false, &s_key);
    fn_7101178e08(static_cast<UnkActorTarget*>(self->owner->field_0x220), 0xe, false, false, -1.0f);
    fn_7101178e08(static_cast<UnkActorTarget*>(fn_7100d7c990(self->owner)->field_0x220), 0xe, false, false,
                  -1.0f);
    if (auto* player = reinterpret_cast<UnkGrabPlayer_l507d24*>(fn_7100dec788(g_unk_7101d47310))) {
        if (player->field_0x1c0)
            fn_710116a158(player->field_0x1c0, &g_unk_71726de5c8);
        if (player->input)
            fn_7100872460(player->input, false, false, &g_unk_71726de5d0);
    }
    fn_7100df6c78(g_unk_7101d47310, 1.0f, 0.0f);
    if (void* game = fn_7100defcec(g_unk_7101d47310)) {
        static CStrId s_water("EmmyGrabWater", true);
        fn_71010e4a80(game, &s_water);
    }
    for (s32 i = 0; i < reinterpret_cast<UnkMusicTracks_l507d24*>(MusicManager())->count; ++i)
        fn_7100fd8720(reinterpret_cast<UnkMusicSlotOwner_fd8720*>(MusicManager()), i);
}
