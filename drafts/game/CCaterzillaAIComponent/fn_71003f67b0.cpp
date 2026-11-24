// CCaterzillaAIComponent's unit (tools/tu_map.py): starts the component's state block (+0x146d0). It drops the
// counted reference, registers the owner (+0x30) with the global manager's registry, finds the owner's spawn
// point component and its flags, clears the state, hands the AI sink a parameter block, installs the
// component's callback on the owner's +0x218 object, sets the animation's two channels (swapped when the
// component the reference at +0 holds has its flag set), and, with no spawn point, sends the animation the
// start request and measures how long the entry's motion lasts. Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkCounted.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkMotionRule.h"
#include "game/UnkStruct_71007142e8.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTimeline.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkCaterzillaAI_l3f67b0;
void fn_71003f8d6c(void* registry, void* owner);
void fn_71003f6d90(UnkCaterzillaAI_l3f67b0* self);
void fn_71003f85f8();
extern const base::reflection::CType g_unk_71726c2cc8 __attribute__((visibility("hidden")));

struct UnkCaterzillaSpawn_l3f67b0 {
    u8 pad_0x0[0x3d1];
    u8 field_0x3d1;
    u8 pad_0x3d2[0x420 - 0x3d2];
    void* field_0x420;
};

struct UnkCaterzillaHandle_l3f67b0 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
};

struct UnkCaterzillaTarget_l3f67b0 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* field_0x240;
};

struct UnkCaterzillaCallback_l3f67b0 {
    u8 pad_0x0[0x19d0];
    void* context;   // 0x19d0
    void* callback;  // 0x19d8
};

struct UnkCaterzillaOwner_l3f67b0 {
    u8 pad_0x0[0x218];
    UnkCaterzillaCallback_l3f67b0* field_0x218;
    void* animation;  // 0x220
    u8 pad_0x228[0x298 - 0x228];
    void* field_0x298;
    u8 pad_0x2a0[0x398 - 0x2a0];
    UnkCaterzillaHandle_l3f67b0* field_0x398;
};

struct UnkCaterzillaState_l3f67b0 {
    UnkCounted* field_0x0;
    u32 field_0x8;
    u8 field_0xc;
    u8 field_0xd;
    u8 field_0xe;
    u8 pad_0xf[0x11];
    UnkCaterzillaSpawn_l3f67b0* field_0x20;
    u8 field_0x28;
    u8 field_0x29;
    u8 pad_0x2a[6];
    void* field_0x30;
    void* field_0x38;
    u64 field_0x40;
    u8 field_0x48;
    u8 field_0x49;
    u8 field_0x4a;
    u8 field_0x4b;
    u8 field_0x4c;
    u8 field_0x4d;
    u8 field_0x4e;
    u8 pad_0x4f;
    f32 field_0x50;
    u8 field_0x54;
    u8 pad_0x55[3];
    f32 field_0x58;
    u8 field_0x5c;
};

struct UnkCaterzillaAI_l3f67b0 {
    u8 pad_0x0[0x30];
    UnkCaterzillaOwner_l3f67b0* owner;  // 0x30
    u8 pad_0x38[0x14698];
    UnkCaterzillaState_l3f67b0 state;  // 0x146d0
};
static_assert(__builtin_offsetof(UnkCaterzillaAI_l3f67b0, state) == 0x146d0, "layout");

void fn_71003f67b0(UnkCaterzillaAI_l3f67b0* self) {
    base::reflection::UnkReflTypeIdArg mode;
    UnkCaterzillaState_l3f67b0* state = &self->state;
    state->field_0xc = 0;
    if (UnkCounted* ref = state->field_0x0) {
        if (--ref->count == 0)
            ref->vf_0x10();
        state->field_0x0 = nullptr;
    }
    state->field_0x5c = 0;
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_71003f8d6c(manager->field_0x330, self->owner);

    UnkCaterzillaSpawn_l3f67b0* spawn = nullptr;
    if (UnkCaterzillaHandle_l3f67b0* handle = self->owner->field_0x398) {
        if (base::reflection::UnkReflObject* object = handle->field_0x10) {
            if (fn_710009d50c(object->vf_0x0(), fn_7100a15a10()))
                spawn = reinterpret_cast<UnkCaterzillaSpawn_l3f67b0*>(object);
        }
    }
    state->field_0x20 = spawn;
    state->field_0x29 = 0;
    state->field_0x48 = 1;
    if (spawn) {
        if (spawn->field_0x420 || !spawn->field_0x3d1)
            state->field_0x29 = 1;
        if (spawn->field_0x3d1)
            state->field_0x48 = 0;
    }
    fn_7100369650(reinterpret_cast<char*>(self));
    state->field_0x58 = 1.0f;
    state->field_0xd = 0;
    state->field_0x54 = 0;
    state->field_0x40 = 0x40000000;
    state->field_0x28 = 0;
    state->field_0x4e = 0;
    state->field_0x8 = 0;
    state->field_0x49 = 0;
    state->field_0x4a = 0;
    state->field_0x4b = 0;
    state->field_0x4c = 0;

    UnkParams params;
    base::reflection::UnkReflTypeIdArg mask;
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x30 = 0;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x8, 0, 13);
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x34 = 5;
    params.pad_0x3c = 1;
    params.field_0x3d = 1;
    fn_71006f4b0c(self->owner->field_0x298, &params, 1);

    UnkCaterzillaCallback_l3f67b0* sink = self->owner->field_0x218;
    state->field_0x30 = self;
    state->field_0x38 = reinterpret_cast<void*>(fn_71003f85f8);
    sink->callback = reinterpret_cast<void*>(fn_71003f85f8);
    sink->context = self;
    state->field_0xe = 0;
    if (UnkActorTarget* animation = static_cast<UnkActorTarget*>(self->owner->animation)) {
        fn_7101178e08(animation, 0xc, false, false, -1.0f);
        fn_710117662c(animation, 6, 0, true, false, -1.0f);
    }
    if (state->field_0x8)
        state->field_0x8 = 0;
    if (UnkCounted* ref = state->field_0x0) {
        if (auto* target = reinterpret_cast<UnkCaterzillaTarget_l3f67b0*>(ref->field_0x10)) {
          if (base::reflection::UnkReflObject* object = target->field_0x240) {
            UnkCaterzillaAI_l3f67b0* other = reinterpret_cast<UnkCaterzillaAI_l3f67b0*>(object);
            if (fn_710009d50c(object->vf_0x0(), (fn_71003f6484(), &g_unk_71726c2cc8)) && other->state.field_0xe) {
                UnkActorTarget* animation = static_cast<UnkActorTarget*>(self->owner->animation);
                state->field_0xe = 1;
                if (animation) {
                    fn_7101178e08(animation, 6, false, false, -1.0f);
                    fn_710117662c(animation, 0xc, 0, true, false, -1.0f);
                }
                if (state->field_0x8 != 1)
                    state->field_0x8 = 1;
            }
          }
        }
    }
    state->field_0x4d = 1;
    if (!state->field_0x20) {
        if (void* animation = self->owner->animation) {
            mode.id = 0;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0x64692faf0bfdfd6dull, 0xf4061ff25a2163d6ull, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
            fn_71003f6d90(self);
        }
    }
    if (UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->animation)) {
        if (UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176d34(
                reinterpret_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(0xac868ecbb1191e55ull)))) {
            UnkStruct_71007142e8 motion;
            fn_71007142e8(&motion);
            UnkMotionTracks* tracks = timeline->tracks;
            UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
            fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
            state->field_0x50 = fabsf(motion.field_0x2c);
        }
    }
}
