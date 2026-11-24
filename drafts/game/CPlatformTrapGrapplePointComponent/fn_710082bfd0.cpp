// CPlatformTrapGrapplePointComponent's unit (tools/tu_map.py): passes its arguments to fn_710082ccec;
// when grabbed, requests state 1 of the owner's (+0x30) state machine if it allows (fn_7100aca444) and
// tells the game manager; when released, the owner's animation (+0x220) plays a fixed request while one
// track plays, else is sent the request's hash with mode 0 unless that plays already (as UnkSpitSend).
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/CAnimationComponent.h"
#include "game/CPullableGrapplePointComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

void* fn_710082bd74(void* owner);
bool fn_7100aca444(void* state);
void fn_7100aca484(void* state);

struct UnkTrapEvent_l82bfd0;
__attribute__((visibility("hidden"))) extern UnkTrapEvent_l82bfd0 g_unk_7172724cb0;

struct UnkTrapGrabDesc_l82bfd0 {
    u64 words[2];
};

struct __attribute__((packed, aligned(4))) UnkTrapGrabBlend_l82bfd0 {
    u64 kind;
    u32 value;
};

// An animation request (as fn_7101176ea8 reads it).
struct UnkTrapGrabRequest_l82bfd0 {
    u64 id;
    u64 desc[2];      // 0x8
    u64 blend_kind;   // 0x18
    u32 blend_value;  // 0x20
    u32 flags;        // 0x24
    u32 mask;         // 0x28
};

struct UnkTrapGrabVec_l82bfd0 {
    f32 x, y, z;
};

struct UnkTrapGrabOwner_l82bfd0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkTrapGrab_l82bfd0 {
    u8 pad_0x0[0x30];
    UnkTrapGrabOwner_l82bfd0* owner;  // 0x30
};

void fn_710082bfd0(UnkTrapGrab_l82bfd0* self, void* arg, bool grabbed) {
    // The mode of the send below lives for the whole function (its own stack slot in the original).
    base::reflection::UnkReflTypeIdArg mode;
    fn_710082ccec(reinterpret_cast<UnkDoorGrapplePoint_A5_b66*>(self), arg, grabbed);
    if (grabbed) {
        void* state = fn_710082bd74(self->owner);
        if (fn_7100aca444(state)) {
            fn_7100aca484(state);
            UnkGameManager* manager = g_unk_7101d47310;
            UnkTrapGrabVec_l82bfd0 v;
            v.x = -1.0f;
            v.y = 0.0f;
            v.z = 0.0f;
            fn_7100e038b0(manager, &g_unk_7172724cb0, reinterpret_cast<UnkVec*>(&v), 0);
        }
        return;
    }
    UnkAnimationComponent* animation = self->owner->animation;
    const u64 id = 0x6909d27d2c787fba;
    if (fn_710117ae38(animation, 0x4e11536af9ada8fc, false)) {
        UnkTrapGrabRequest_l82bfd0 request;
        const UnkTrapGrabDesc_l82bfd0* desc = reinterpret_cast<const UnkTrapGrabDesc_l82bfd0*>(&g_unk_710164b958);
        u64 desc0 = desc->words[0];
        u64 desc1 = desc->words[1];
        const UnkTrapGrabBlend_l82bfd0* blend =
            reinterpret_cast<const UnkTrapGrabBlend_l82bfd0*>(&g_unk_7172769794);
        u64 kind = blend->kind;
        u32 value = blend->value;
        u32 mask = UnkAll_SpitSend().value;
        request.id = id;
        request.desc[0] = desc0;
        request.desc[1] = desc1;
        request.blend_kind = kind;
        request.blend_value = value;
        request.flags = 0x400;
        request.mask = mask;
        CAnimationComponent_SendActionRequestRecordToTrack(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                      reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
    } else if (!fn_710117ae38(animation, id, false)) {
        mode.id = 0;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, id, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    }
}
