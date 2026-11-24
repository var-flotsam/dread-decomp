// CAbilitySpeedBooster's unit (tools/tu_map.py): the ability's end. Switches the boost state machine (+0x3a8)
// to state 1 ("none" is 4), cancels the boost (fn_710033c350), resets the model colour of the player and of
// the camera's actor, clears two of the owner's tracks and replays its boost track, plays the ability's
// timeline key when the reason is 2, then runs CAbility's end (fn_71002fa068). Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CAnimationComponent.h"
#include "game/CSamusModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAnimComponent_b52;
void fn_7101178e08(UnkAnimComponent_b52* self, u32 value, bool apply, bool flag, f32 time);
struct UnkSpeedBoosterAbility_ovb0088;
void fn_710033c350(UnkSpeedBoosterAbility_ovb0088* self);
extern const u8 g_unk_7101649064[] __attribute__((visibility("hidden")));  // the model colour of the end
extern const u8 g_unk_7101649074[] __attribute__((visibility("hidden")));
extern UnkDesc_SpitSend g_unk_710164b958;

struct UnkBoostState_l33d340 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

struct UnkBoostMachine_l33d340 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkBoostState_l33d340* previous, UnkBoostState_l33d340* next);
    virtual void vf_0x28(UnkBoostState_l33d340* current, UnkBoostState_l33d340* next);

    u64 field_0x8;
    UnkBoostState_l33d340* current;
    UnkBoostState_l33d340* previous;
    u32 count;
    u8 storage[16 * sizeof(UnkBoostState_l33d340*)];
    s32 pending;
    bool busy;

    UnkBoostState_l33d340** begin() { return reinterpret_cast<UnkBoostState_l33d340**>(storage); }
    UnkBoostState_l33d340** end() { return begin() + count; }

    UnkBoostState_l33d340* Find(s32 id) {
        UnkBoostState_l33d340** it = begin();
        for (; it != end(); ++it) {
            if ((*it)->id == id)
                break;
        }
        return it != end() ? *it : nullptr;
    }

    void Request(s32 id) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != id) {
            if (UnkBoostState_l33d340* next = Find(id)) {
                if (current) {
                    current->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : 4);
                vf_0x20(previous, next);
            }
        }
        pending = 4;
    }
};

struct UnkBoostOwner_l33d340 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkBoostModelActor_l33d340 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    u8 pad_0x1b0[0x228 - 0x1b0];
    UnkReflObject* model_updater;  // 0x228
};

struct UnkBoostModelHolder_l33d340 {
    u8 pad_0x0[0x10];
    UnkBoostModelActor_l33d340* actor;  // 0x10
};

struct UnkBoostManager_l33d340 {
    u8 pad_0x0[0x29a0];
    UnkBoostModelHolder_l33d340* holder;  // 0x29a0
};

struct UnkBoostTrackDef_l33d340 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkBoostTrack_l33d340 {
    u8 pad_0x0[0x38];
    UnkBoostTrackDef_l33d340* def;  // 0x38
    u8 pad_0x40[0x8];
    u64 hash;  // 0x48
};

struct UnkSpeedBooster_l33d340 {
    void** vtable;
    u8 pad_0x8[0xc0 - 0x8];
    UnkBoostOwner_l33d340* owner;  // 0xc0
    u8 pad_0xc8[0x3a2 - 0xc8];
    bool field_0x3a2;
    u8 pad_0x3a3[0x3a8 - 0x3a3];
    UnkBoostMachine_l33d340 machine;  // 0x3a8

    // vtable slot 0xe8: the ability's timeline key.
    u64 vf_0xe8() { return reinterpret_cast<u64 (*)(UnkSpeedBooster_l33d340*)>(vtable[0xe8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkSpeedBooster_l33d340, machine.pending) == 0x44c, "layout");

static inline void ResetModelColour(UnkBoostModelActor_l33d340* actor) {
    UnkReflObject* comp = actor->model_updater;
    if (comp && fn_710009d50c(comp->vf_0x0(), fn_7100939af4()))
        fn_710093adc0(comp, g_unk_7101649064, g_unk_7101649074, actor->field_0x1a8 ? 0.2f : -1.0f);
}

static inline UnkBoostModelActor_l33d340* CameraActor() {
    UnkBoostModelHolder_l33d340* holder = reinterpret_cast<UnkBoostManager_l33d340*>(g_unk_7101d47310)->holder;
    return holder ? holder->actor : nullptr;
}

void fn_710033d340(UnkSpeedBooster_l33d340* self, s32 reason) {
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    self->machine.Request(1);
    fn_710033c350(reinterpret_cast<UnkSpeedBoosterAbility_ovb0088*>(self));
    ResetModelColour(reinterpret_cast<UnkBoostModelActor_l33d340*>(fn_7100defcd4(g_unk_7101d47310)));
    ResetModelColour(CameraActor());
    UnkActor* actor = self->owner->actor;
    if (actor && actor->field_0x220 && actor->field_0x208) {
        void* animation = actor->field_0x220;
        self->field_0x3a2 = false;
        fn_7101178e08(static_cast<UnkAnimComponent_b52*>(animation), 3, true, true, -1.0f);
        fn_7101178e08(static_cast<UnkAnimComponent_b52*>(animation), 2, true, true, -1.0f);
        auto* track = static_cast<UnkBoostTrack_l33d340*>(
            fn_7101177d2c(static_cast<UnkAnimationComponent*>(animation), 0xf4061ff25a2163d6));
        if (track->def->id == 0x4f23b5bdcddb4370) {
            u64 hash = track->hash;
            mode.id = 0;
            mask.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        }
    }
    if (reason == 2) {
        u64 key = self->vf_0xe8();
        fn_7100d7c828(actor, &key, false);
    }
    fn_71002fa068(reinterpret_cast<UnkSmLockon_A1_b64*>(self), reason);
}
