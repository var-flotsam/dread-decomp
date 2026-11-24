// CPlatformTrapGrapplePointComponent's unit (tools/tu_map.py): passes its arguments to fn_710082cdb0,
// then for state 0 has the owner's animation (+0x30 -> +0x220) play a fixed request, for state 1
// sends it a fixed hash with mode 0 (as UnkSpitSend). Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct UnkGrappleSwitch_b56;
void fn_710082cdb0(UnkGrappleSwitch_b56* self, void* arg, s32 state);

struct UnkTrapDesc_l82c28c {
    u64 words[2];
};

struct __attribute__((packed, aligned(4))) UnkTrapBlend_l82c28c {
    u64 kind;
    u32 value;
};

// An animation request (as fn_7101176ea8 reads it).
struct UnkTrapRequest_l82c28c {
    u64 id;
    u64 desc[2];      // 0x8
    u64 blend_kind;   // 0x18
    u32 blend_value;  // 0x20
    u32 flags;        // 0x24
    u32 mask;         // 0x28
};

struct UnkTrapOwner_l82c28c {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
};

struct UnkTrapGrapple_l82c28c {
    u8 pad_0x0[0x30];
    UnkTrapOwner_l82c28c* owner;  // 0x30
};

void fn_710082c28c(UnkTrapGrapple_l82c28c* self, void* arg, s32 state) {
    UnkTrapRequest_l82c28c request;
    fn_710082cdb0(reinterpret_cast<UnkGrappleSwitch_b56*>(self), arg, state);
    if (state == 1) {
        UnkSpitSend(self->owner->animation, 0x4e11536af9ada8fc, 0xf4061ff25a2163d6);
    } else if (state == 0) {
        const UnkTrapDesc_l82c28c* desc = reinterpret_cast<const UnkTrapDesc_l82c28c*>(&g_unk_710164b958);
        u64 desc0 = desc->words[0];
        u64 desc1 = desc->words[1];
        const UnkTrapBlend_l82c28c* blend = reinterpret_cast<const UnkTrapBlend_l82c28c*>(&g_unk_7172769794);
        u64 kind = blend->kind;
        u32 value = blend->value;
        u32 mask = UnkAll_SpitSend().value;
        request.id = 0x6909d27d2c787fba;
        request.desc[0] = desc0;
        request.desc[1] = desc1;
        request.blend_kind = kind;
        request.blend_value = value;
        request.flags = 0x400;
        request.mask = mask;
        CAnimationComponent_SendActionRequestRecordToTrack(self->owner->animation, reinterpret_cast<const UnkRequest_7101176ea8*>(&request),
                      0xf4061ff25a2163d6);
    }
}
