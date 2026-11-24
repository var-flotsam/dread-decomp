// CDoorLifeComponent's unit (tools/tu_map.py unit 1376): the door's update. Opens or closes it when the
// "ForceOpenOnDisabled" key changes, plays the shield animations and clears the shield flags, starts the
// opening blend, sends the actor's animation the open request, resets both sound handles and enters state 1
// when the door is told to. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CLifeComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkDoorLifeTunable.h"
#include "game/UnkDoorShieldIds.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU101.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU39.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

#include "../../game_s2/UnkSpitSend.h"

using base::global::CStrId;

struct UnkFlagTarget;

extern const u64 g_unk_7172731788 __attribute__((visibility("hidden")));

struct UnkDoorIdHolder_l8d0250 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkDoorState_l8d0250 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkDoorState_l8d0250* __attribute__((aligned(4))) UnkDoorStatePtr_l8d0250;

struct UnkDoorStateMachine_l8d0250 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkDoorState_l8d0250* previous, UnkDoorState_l8d0250* next);  // after entering
    virtual void vf_0x28(UnkDoorState_l8d0250* current, UnkDoorState_l8d0250* next);   // after leaving

    u64 field_0x8;
    UnkDoorState_l8d0250* current;
    UnkDoorState_l8d0250* previous;
    u32 count;
    UnkDoorStatePtr_l8d0250 states[16];
    s32 pending;
    bool busy;
};

struct UnkDoorOwner_l8d0250 {
    u8 pad_0x0[0x1b4];
    bool field_0x1b4;
    u8 pad_0x1b5[0x220 - 0x1b5];
    void* animation;        // 0x220
    UnkFlagTarget* flags;  // 0x228
};

struct UnkDoorConfig_l8d0250 {
    u8 pad_0x0[0x12e];
    bool field_0x12e;
};

struct UnkDoorLife_l8d0250 {
    u8 pad_0x0[0x30];
    UnkDoorOwner_l8d0250* owner;  // 0x30
    u8 pad_0x38[0x419 - 0x38];
    bool field_0x419;
    bool field_0x41a;
    u8 pad_0x41b;
    bool field_0x41c;
    u8 pad_0x41d[2];
    bool field_0x41f;
    UnkDoorStateMachine_l8d0250 machine;  // 0x420
    u8 pad_0x4d0[0x644 - 0x4d0];
    u32 field_0x644;
    u32 field_0x648;
    u8 pad_0x64c[0x7c4 - 0x64c];
    bool field_0x7c4;
    u8 pad_0x7c5[0x9f8 - 0x7c5];
    UnkDoorConfig_l8d0250* config;  // 0x9f8
    u8 pad_0xa00[0xac8 - 0xa00];
    UnkSoundHandle sounds[2];  // 0xac8
    u8 pad_0xae8[2];
    bool field_0xaea;
    bool field_0xaeb;
    u8 pad_0xaec[0xaf8 - 0xaec];
    UnkGuiKeyList keys;  // 0xaf8
};
static_assert(__builtin_offsetof(UnkDoorLife_l8d0250, machine) == 0x420, "layout");
static_assert(__builtin_offsetof(UnkDoorStateMachine_l8d0250, pending) == 0xa4, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d0250, field_0x644) == 0x644, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d0250, config) == 0x9f8, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d0250, keys) == 0xaf8, "layout");

void fn_71008d07f8(UnkDoorLife_l8d0250* self);
bool fn_71008d0db4(UnkDoorLife_l8d0250* self);

static inline UnkDoorState_l8d0250* FindState(UnkDoorStateMachine_l8d0250* self, s32 id) {
    return FindStateById(self->states, self->count, id);
}

static inline void RequestState(UnkDoorStateMachine_l8d0250* self, s32 id) {
    self->pending = id;
    if (self->busy)
        return;
    UnkDoorState_l8d0250* current = self->current;
    if (!current || current->id != id) {
        if (UnkDoorState_l8d0250* next = FindState(self, id)) {
            if (current) {
                current->vf_0x18(id);
                self->vf_0x28(self->current, next);
            }
            self->previous = self->current;
            self->current = next;
            next->vf_0x10(self->previous ? self->previous->id : 0x7fffffff);
            self->vf_0x20(self->previous, next);
        }
    }
    self->pending = 0x7fffffff;
}

static inline bool InState(UnkDoorStateMachine_l8d0250* self, s32 id) {
    return self->current && self->current->id == id;
}

static inline void PlayShield(UnkDoorLife_l8d0250* self, const u64* animation, bool reverse) {
    fn_71008d0a08(reinterpret_cast<UnkDoorShieldLife6_ovb0001*>(self), animation, reverse, false);
}

static inline bool IsLocked(const UnkDoorLife_l8d0250* self) {
    return (self->field_0x644 & 0x8000400) || (self->field_0x648 & 0x8000400);
}

static inline bool IsFlagged(const UnkDoorLife_l8d0250* self) {
    return (self->field_0x644 & 1) || (self->field_0x648 & 1);
}

void fn_71008d0250(UnkDoorLife_l8d0250* self) {
    fn_710115fe1c(reinterpret_cast<char*>(self));

    if (!self->owner->field_0x1b4) {
        static CStrId s_force_open("ForceOpenOnDisabled", true);
        bool before = fn_710002fb70(&self->keys);
        fn_710002fd10(&self->keys, &s_force_open);
        bool after = fn_710002fb70(&self->keys);
        if (before != after) {
            if (after)
                OpenDoorUnlessOpen(self);
            else
                fn_71008d498c(reinterpret_cast<UnkDoorLife_b44a2*>(self), 0, 0, 0);
        }
    }

    if (InState(&self->machine, 2))
        fn_71008d07f8(self);

    // One name buffer serves both shield flags and lives to the end (the original's frame keeps it apart from
    // the request below).
    u64 name;
    if (self->owner->flags) {
        if (!self->field_0x41f)
            PlayShield(self, &g_unk_7172731788, false);
        PlayShield(self, &g_unk_71727317a0.open, self->field_0xaea);
        PlayShield(self, &g_unk_71727317a0.close, self->field_0xaeb);
        if (UnkFlagTarget* target = self->owner->flags) {
            name = g_unk_7172731790->name;
            fn_7101172780(target, reinterpret_cast<const char*>(&name));
        }
        if (UnkFlagTarget* target = self->owner->flags) {
            name = g_unk_7172731798->name;
            fn_7101172780(target, reinterpret_cast<const char*>(&name));
        }
    }

    if (!self->field_0x41f && self->field_0x41c) {
        if (IsLocked(self) || IsFlagged(self) || fn_71008d89b0(self->owner))
            fn_71008d0c24(reinterpret_cast<UnkDoorLifeA5_b61*>(self), IsLocked(self) ? 0.0f : 0.3f, 0.0f);
    }

    self->field_0x7c4 = fn_71008d0db4(self);

    if (g_unk_7172731368.bForceAllDoorsOpened || self->field_0x419 || (!self->field_0x41c && self->field_0x41a)) {
        if (!self->field_0x41c) {
            void* animation = self->owner->animation;
            u64 hash = reinterpret_cast<UnkDoorIdHolder_l8d0250*>(GameManagerIdTable().field_0x1b40.data)->hash;
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 0x82;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &g_unk_7172769740, &mask);
        }
        OpenDoorUnlessOpen(self);
    }

    self->sounds[0] = g_unk_7108b6c460;
    self->sounds[1] = g_unk_7108b6c460;

    if (self->config->field_0x12e && !InState(&self->machine, 1))
        RequestState(&self->machine, 1);
}
