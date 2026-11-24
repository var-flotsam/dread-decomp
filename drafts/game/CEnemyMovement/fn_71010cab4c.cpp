// CEnemyMovement unit: starts the owner's turn. A reversible turn plays the main track's action with
// "_reverse" appended for the rest of the current one; otherwise the AI component turns, or the
// turn action the character class names is sent to the animation. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1134.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

static const u64 kMainTrack = 0xf4061ff25a2163d6;

extern char g_unk_7101649fc8[];

// The turn settings of the AI component: the turn action for the owner.
struct UnkTurnSettings_l0cab4c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual const CStrId* TurnAction(void* owner);  // vf_0x60
};

struct UnkTurnAI_l0cab4c;
struct UnkTurnAIVtbl_l0cab4c {
    void* slots_0x0[0x1b8 / 8];
    bool (*IsTurning)(UnkTurnAI_l0cab4c* self);  // 0x1b8
    void* slots_0x1c0[(0x518 - 0x1c0) / 8];
    void (*OnTurnStarted)(UnkTurnAI_l0cab4c* self);  // 0x518
    void* slots_0x520[(0x660 - 0x520) / 8];
    bool (*Turn)(UnkTurnAI_l0cab4c* self, bool flag);  // 0x660
};

struct UnkTurnAI_l0cab4c {
    UnkTurnAIVtbl_l0cab4c* vtable;
    u8 pad_0x8[0x121e - 0x8];
    bool can_reverse;  // 0x121e
    u8 pad_0x121f[0x1610 - 0x121f];
    UnkTurnSettings_l0cab4c* settings;  // 0x1610

    bool IsTurning() { return vtable->IsTurning(this); }
    bool Turn(bool flag) { return vtable->Turn(this, flag); }
    void OnTurnStarted() { vtable->OnTurnStarted(this); }
};
bool fn_7101110964(UnkTurnAI_l0cab4c* self, const CStrId* action, const void* desc, const u32* mode, f32 a, f32 b);

struct UnkTurnOwner_l0cab4c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkTurnAI_l0cab4c* ai;  // 0x240
};

struct UnkTurnListener_l0cab4c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void OnTurnStarted();  // vf_0x90
};

struct UnkTrackLength_l0cab4c {
    u8 pad_0x0[0x24];
    s32 frames;  // 0x24
};

struct UnkTurnTiming_l0cab4c {
    u64 start;
    f32 length;
};

struct UnkEnemyMovement_l0cab4c;
struct UnkEnemyMovementVtbl_l0cab4c {
    void* slots_0x0[0x348 / 8];
    void (*ResetTurn)(UnkEnemyMovement_l0cab4c* self);  // 0x348
};

struct UnkEnemyMovement_l0cab4c {
    UnkEnemyMovementVtbl_l0cab4c* vtable;
    u8 pad_0x8[0x28];
    UnkTurnOwner_l0cab4c* owner;  // 0x30
    u8 pad_0x38[0x6f4 - 0x38];
    u32 field_0x6f4;
    u8 pad_0x6f8[0x848 - 0x6f8];
    UnkTurnListener_l0cab4c* listener;  // 0x848

    void OnTurnStarted() {
        field_0x6f4 = 0;
        vtable->ResetTurn(this);
        if (UnkTurnAI_l0cab4c* ai = owner->ai)
            ai->OnTurnStarted();
        if (UnkTurnListener_l0cab4c* l = listener)
            l->OnTurnStarted();
    }
};

static inline bool HasValue(const CStrId* id) {
    return id->data != &g_unk_7101d47108->empty;
}

// Plays the main track's action backwards for the rest of the current one.
static inline bool PlayReverse(UnkEnemyMovement_l0cab4c* self, bool flag) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation || !fn_710117b348(animation, fn_7100d2b3a8()) || !fn_710117b348(animation, fn_7100bf6df8()) ||
        !self->owner->ai->can_reverse)
        return false;
    CStrId action = *fn_7101177ae8(animation, kMainTrack);
    UnkTunableLine name;
    CRntString_AssignStrId(name.Str(), &action);
    UnkTunableLine suffix("_reverse");
    u64 key;
    {
        CRntString reversed = CRntString_Concat(name.Str(), suffix.Str());
        fn_7100066630(&key, &reversed);
    }
    s32 done = static_cast<s32>(fn_710070d89c(CAnimationComponent_FindTrackByNameHash(animation, kMainTrack)));
    UnkTrackLength_l0cab4c* track = static_cast<UnkTrackLength_l0cab4c*>(fn_7101177d2c(animation, kMainTrack));
    UnkTurnTiming_l0cab4c timing;
    timing.start = 0;
    timing.length = static_cast<f32>(track->frames - done);
    bool started = false;
    if (base::reflection::refl_EmitU32(animation, key, kMainTrack, flag ? 9 : 8, &g_unk_710164b958, &timing)) {
        self->OnTurnStarted();
        started = true;
    }
    return started;
}

bool fn_71010cab4c(UnkEnemyMovement_l0cab4c* self, bool flag) {
    UnkTurnAI_l0cab4c* ai = self->owner->ai;
    if (!ai)
        return false;
    UnkTurnSettings_l0cab4c* settings = ai->settings;
    if (!settings)
        return false;
    bool reverse = false;
    if (UnkAnimationComponent* animation = self->owner->animation) {
        if (!fn_71011782ec(animation, kMainTrack))
            reverse = fn_710117b348(animation, fn_7100d14318()) ||
                      fn_7101179870(reinterpret_cast<UnkStateSet_1441*>(animation)) || ai->IsTurning();
    }
    if (reverse)
        return PlayReverse(self, flag);

    if (UnkTurnAI_l0cab4c* current = self->owner->ai) {
        if (current->Turn(flag))
            return true;
    }

    UnkTrackQuery query = {fn_7100d34868(), nullptr, -1.0f};
    const CStrId* action = nullptr;
    if (UnkAnimationComponent* animation = self->owner->animation) {
        fn_710117b08c(animation, &query);
        if (query.result && HasValue(&query.result->sources))
            action = &query.result->sources;
    }
    if (!action)
        action = settings->TurnAction(self->owner);

    if (HasValue(action) &&
        fn_7101176e78(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                      reinterpret_cast<void*>(action->data->Key()), nullptr)) {
        UnkAnimationComponent* animation = self->owner->animation;
        if (!animation || fn_710117b348(animation, fn_7100bfc588()))
            return false;
        if (!fn_710117b348(animation, fn_7100bfc078())) {
            u32 mode = flag;
            if (!fn_7101110964(ai, action, g_unk_7101649fc8, &mode, -1.0f, 0.0f)) {
                if (!base::reflection::refl_EmitU32(animation, action->data->Key(), kMainTrack, flag,
                                                    &g_unk_710164b958, &g_unk_7172769740))
                    return false;
            }
        }
    }
    self->OnTurnStarted();
    return true;
}
