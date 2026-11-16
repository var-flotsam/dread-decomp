// CAbilitySpeedBooster's unit (tools/tu_map.py): the ability's stop. After CAbility's (fn_71002f9f18) it
// resets the model colour of the player and of the camera's actor, switches the boost state machine (+0x3a8)
// to state 2 ("none" is 4), stops the owner's boost animation and records whether the owner faces forward
// (+0x3a0), and, unless fn_7100df5a4c says otherwise, clears two timeline keys. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CSamusModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkActorTarget;
void fn_710117662c(UnkActorTarget*, u32 value, u32 extra, bool apply, bool flag, f32 time);
struct UnkV4_b47;
extern const UnkV4_b47 g_unk_7101649044 __attribute__((visibility("hidden")));  // the model colour reset
extern const u8 g_unk_7101649054[] __attribute__((visibility("hidden")));
extern UnkDesc_SpitSend g_unk_710164b958;
extern const u64 g_unk_7101649028;
extern const u64 g_unk_7101649030;

struct UnkBoostState_l33cf48 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

struct UnkBoostMachine_l33cf48 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkBoostState_l33cf48* previous, UnkBoostState_l33cf48* next);
    virtual void vf_0x28(UnkBoostState_l33cf48* current, UnkBoostState_l33cf48* next);

    u64 field_0x8;
    UnkBoostState_l33cf48* current;
    UnkBoostState_l33cf48* previous;
    u32 count;
    u8 storage[16 * sizeof(UnkBoostState_l33cf48*)];
    s32 pending;
    bool busy;

    UnkBoostState_l33cf48** begin() { return reinterpret_cast<UnkBoostState_l33cf48**>(storage); }
    UnkBoostState_l33cf48** end() { return begin() + count; }

    UnkBoostState_l33cf48* Find(s32 id) {
        UnkBoostState_l33cf48** it = begin();
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
            if (UnkBoostState_l33cf48* next = Find(id)) {
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

struct UnkBoostOwner_l33cf48 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkBoostModelActor_l33cf48 {
    u8 pad_0x0[0x228];
    UnkReflObject* model_updater;  // 0x228
};

struct UnkBoostModelHolder_l33cf48 {
    u8 pad_0x0[0x10];
    UnkBoostModelActor_l33cf48* actor;  // 0x10
};

struct UnkBoostManager_l33cf48 {
    u8 pad_0x0[0x29a0];
    UnkBoostModelHolder_l33cf48* holder;  // 0x29a0
};

struct UnkSpeedBooster_l33cf48 {
    u8 pad_0x0[0xc0];
    UnkBoostOwner_l33cf48* owner;  // 0xc0
    u8 pad_0xc8[0x390 - 0xc8];
    f32 phase;  // 0x390
    u8 pad_0x394[0x3a0 - 0x394];
    bool facing;  // 0x3a0
    u8 pad_0x3a1[0x3a8 - 0x3a1];
    UnkBoostMachine_l33cf48 machine;  // 0x3a8
};
static_assert(__builtin_offsetof(UnkSpeedBooster_l33cf48, machine.pending) == 0x44c, "layout");

static inline void ResetModelColour(UnkReflObject* comp) {
    if (comp && fn_710009d50c(comp->vf_0x0(), fn_7100939af4()))
        fn_710093adc0(comp, &g_unk_7101649044, g_unk_7101649054, 0.2f);
}

void fn_710033cf48(UnkSpeedBooster_l33cf48* self, s32 kind) {
    fn_71002f9f18(reinterpret_cast<UnkSmLockon_A1_b64*>(self), kind);
    ResetModelColour(reinterpret_cast<UnkBoostModelActor_l33cf48*>(fn_7100defcd4(g_unk_7101d47310))->model_updater);
    ResetModelColour(reinterpret_cast<UnkBoostManager_l33cf48*>(g_unk_7101d47310)->holder->actor->model_updater);
    bool keep = fn_7100df5a4c(g_unk_7101d47310);
    self->phase = 0.0f;
    self->machine.Request(2);
    if (UnkActor* actor = self->owner->actor) {
        if (void* animation = actor->field_0x220) {
            if (!keep) {
                base::reflection::UnkReflTypeIdArg mode;
                mode.id = 0;
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(animation, 0x4f23b5bdcddb4370, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                              &g_unk_7172769740, &mask);
            }
            fn_710117662c(static_cast<UnkActorTarget*>(animation), 2, 0, true, true, 0.0f);
        }
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
        self->facing = m.m[2] > 0.0f;
    }
    if (!keep) {
        fn_7100d7c828(self->owner->actor, &g_unk_7101649028, false);
        fn_7100d7c828(self->owner->actor, &g_unk_7101649030, false);
    }
}
