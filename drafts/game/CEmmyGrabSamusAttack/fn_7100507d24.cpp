// CEmmyGrabSamusAttack's unit: releases the grab. Moves the state machine to its first state, takes the
// "EmmyGrab" key off the E.M.M.I. registry, resets both characters' track 0xe, restores the player,
// fades back in and refreshes every music track. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CBossAIComponent.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/CInputComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU2553.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkDoorKeyOwner_dae8a4;
void fn_7100dae8a4(UnkDoorKeyOwner_dae8a4* self, bool add, CStrId* key);
struct UnkAnimComponent_b52;
void fn_7101178e08(UnkAnimComponent_b52* self, u32 value, bool apply, bool flag, f32 time);
void fn_710116a158(void* self, CStrId* id);
void fn_71010e4a80(void* self, CStrId* id);
void fn_7100fd8720(UnkMgr_0xf18* self, s32 index);
extern CStrId g_unk_71726de5c8 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726de5d0 __attribute__((visibility("hidden")));

struct UnkGrabState_l507d24 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The grab's state machine; its states are only 4-aligned after the count.
struct UnkGrabStateMachine_l507d24 {
    static const s32 kNone = 0xe;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkGrabState_l507d24* previous, UnkGrabState_l507d24* next);  // after entering
    virtual void vf_0x28(UnkGrabState_l507d24* current, UnkGrabState_l507d24* next);   // after leaving

    u64 field_0x8;
    UnkGrabState_l507d24* current;
    UnkGrabState_l507d24* previous;
    u32 count;
    u8 storage[16 * sizeof(UnkGrabState_l507d24*)];
    s32 pending;
    bool field_0xa8;

    UnkGrabState_l507d24** items() { return reinterpret_cast<UnkGrabState_l507d24**>(storage); }

    UnkGrabState_l507d24* Find(s32 id) {
        return FindStateById(items(), count, id);
    }

    void Request(s32 id) {
        pending = id;
        if (field_0xa8)
            return;
        UnkGrabState_l507d24* cur = current;
        if (!cur || cur->id != id) {
            if (UnkGrabState_l507d24* next = Find(id)) {
                if (cur) {
                    cur->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : kNone);
                vf_0x20(previous, next);
            }
        }
        pending = kNone;
    }
};
static_assert(__builtin_offsetof(UnkGrabStateMachine_l507d24, pending) == 0xa4, "layout");

struct UnkGrabAttack_b58a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x18];
    UnkGrabStateMachine_l507d24 state_machine;  // 0x28
};

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
    fn_7101178e08(static_cast<UnkAnimComponent_b52*>(self->owner->field_0x220), 0xe, false, false, -1.0f);
    fn_7101178e08(static_cast<UnkAnimComponent_b52*>(fn_7100d7c990(self->owner)->field_0x220), 0xe, false, false,
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
        fn_7100fd8720(MusicManager(), i);
}
