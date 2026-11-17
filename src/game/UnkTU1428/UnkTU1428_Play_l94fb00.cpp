// Unit 1428 (CMorphBallMovement's neighbourhood): plays the animation whose id is the id table's slot at
// +0x1d78 on the owner's (+0x30) animation (+0x220), with the second default descriptor and flag 2 set.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/CAnimationComponent.h"
#include "types.h"

struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;

struct UnkPlayDesc_l94fb00 {
    u64 words[2];
};

struct UnkPlayBlend_l94fb00 {
    u64 kind;
    u32 value;
};

struct UnkPlayRequest_l94fb00 {
    s64 id;
    u64 desc[2];     // 0x8
    u64 blend_kind;  // 0x18
    u32 blend_value; // 0x20
    u32 flags;       // 0x24
    u32 mask;        // 0x28
};

struct UnkPlayOwner_l94fb00 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
};

struct UnkPlay_l94fb00 {
    u8 pad_0x0[0x30];
    UnkPlayOwner_l94fb00* owner;  // 0x30
};

void fn_710094fb00(UnkPlay_l94fb00* self) {
    UnkPlayRequest_l94fb00 request;
    const UnkPlayDesc_l94fb00* desc = reinterpret_cast<const UnkPlayDesc_l94fb00*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkPlayBlend_l94fb00* blend = reinterpret_cast<const UnkPlayBlend_l94fb00*>(&g_unk_7172769740);
    u64 kind = blend->kind;
    u32 value = blend->value;
    u32 mask = UnkAll_SpitSend().value;
    request.id = -1;
    request.desc[0] = desc0;
    request.desc[1] = desc1;
    request.blend_kind = kind;
    request.blend_value = value;
    request.flags = 0;
    request.mask = mask;
    request.id = GameManagerIdTable().field_0x1d78.data->Key();
    const UnkPlayDesc_l94fb00* desc2 = reinterpret_cast<const UnkPlayDesc_l94fb00*>(&g_unk_710164b968);
    request.desc[0] = desc2->words[0];
    request.desc[1] = desc2->words[1];
    request.flags |= 2;
    fn_7101176ea8(self->owner->animation, reinterpret_cast<const UnkRequest_7101176ea8*>(&request),
                  0xf4061ff25a2163d6);
}
