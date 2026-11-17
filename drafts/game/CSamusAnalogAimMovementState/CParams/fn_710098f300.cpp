// CSamusAnalogAimMovementState::CParams's unit: unless the owner's run state is running (or its +0x50
// object is in state 3), plays on the owner's animation the movement's track request: the first kind
// (fn_71009767f4) while the movement's aim value (+0x7c) is within 0.001 of zero, else the second
// (fn_7100976944) after refreshing the owner's transform. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct UnkTrackTime_ovb0021 {
    u64 key;
    u64 time;
};
struct UnkTrackBlend_ovb0021 {
    u32 a, b, c;
};

// What a track request is built from (0x30 bytes): no id, the default time and blend descriptors,
// no flags and an all-bits mask.
struct UnkTrackParams_ovb0021 {
    s64 id;
    UnkTrackTime_ovb0021 time;
    UnkTrackBlend_ovb0021 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkTrackParams_ovb0021(UnkTrackTime_ovb0021 time_, UnkTrackBlend_ovb0021 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkTrackParams_ovb0021()
        : UnkTrackParams_ovb0021(reinterpret_cast<const UnkTrackTime_ovb0021&>(g_unk_710164b958),
                                 reinterpret_cast<const UnkTrackBlend_ovb0021&>(g_unk_7172769740),
                                 base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

struct UnkTrackMovement_ovb0021;
UnkTrackParams_ovb0021 fn_71009767f4(UnkTrackMovement_ovb0021* self, bool forced);
UnkTrackParams_ovb0021 fn_7100976944(UnkTrackMovement_ovb0021* self, bool forced);

struct UnkAimRunTask_l98f300 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkAimRunState_l98f300 {
    u8 pad_0x0[0x50];
    UnkAimRunTask_l98f300* task;  // 0x50
    u8 pad_0x58[0x360 - 0x58];
    bool running;  // 0x360
};

struct UnkAimOwner_l98f300 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x220 - 0x38 - sizeof(UnkMatrix44)];
    void* animation;  // 0x220
    u8 pad_0x228[0x88];
    UnkAimRunState_l98f300* run_state;  // 0x2b0
};
static_assert(__builtin_offsetof(UnkAimOwner_l98f300, run_state) == 0x2b0, "placeholder layout");

struct UnkAimMovement_l98f300 {
    u8 pad_0x0[0x7c];
    f32 aim;  // 0x7c
};

struct UnkAimParams_l98f300 {
    u8 pad_0x0[0x30];
    UnkAimOwner_l98f300* owner;        // 0x30
    UnkAimMovement_l98f300* movement;  // 0x38
};

static inline UnkTrackMovement_ovb0021* Track(UnkAimMovement_l98f300* movement) {
    return reinterpret_cast<UnkTrackMovement_ovb0021*>(movement);
}

void fn_710098f300(UnkAimParams_l98f300* self) {
    UnkAimOwner_l98f300* owner = self->owner;
    if (UnkAimRunState_l98f300* state = owner->run_state) {
        if (state->running)
            return;
        if (state->task && state->task->state == 3)
            return;
    }
    f32 aim = self->movement->aim;
    // Each request skips its send with a goto past the send's arguments (they get no lifetime markers in
    // the original: each keeps a stack slot of its own).
    if (aim + 0.001f >= 0.0f && aim - 0.001f <= 0.0f) {
        UnkTrackParams_ovb0021 params = fn_71009767f4(Track(self->movement), false);
        s64 track = params.id;
        if (track == -1)
            goto centered_done;
        void* animation;
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        animation = self->owner->animation;
        mode.id = 0;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, track, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    centered_done:;
    } else {
        fn_710002614c(&owner->transform);
        UnkTrackParams_ovb0021 params = fn_7100976944(Track(self->movement), false);
        s64 track = params.id;
        if (track == -1)
            goto aimed_done;
        void* animation;
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        animation = self->owner->animation;
        mode.id = 0;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, track, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    aimed_done:;
    }
}
