// Unit 1452 (tools/tu_map.py): plays the animation whose id hash is the singleton's (fn_7100031894) part at
// +0x12a0 on the owner's (fn_710097f1b8) animation (+0x220) through fn_7101176ea8, with the default time and
// blend, flags 0x10 (bit 0 from the caller's flag) and the all-bits mask. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU1450.h"
#include "types.h"

struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;

struct UnkPartId_l981604 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAnimTime_l981604 {
    u64 a;
    u64 b;
};

struct UnkAnimBlend_l981604 {
    u32 a;
    u32 b;
    u32 c;
};

struct UnkAnimRequest_l981604 {
    u64 id;
    UnkAnimTime_l981604 time;
    UnkAnimBlend_l981604 blend;
    u32 flags;
    u32 mask;

    UnkAnimRequest_l981604(u64 id_, UnkAnimTime_l981604 time_, UnkAnimBlend_l981604 blend_, u32 flags_, u32 mask_)
        : id(id_), time(time_), blend(blend_), flags(flags_), mask(mask_) {}
};

struct UnkAnimOwner_l981604 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;
};

static inline u64 PartHash_l981604(u64 offset) {
    return (*reinterpret_cast<UnkPartId_l981604**>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) +
                                                    offset))
        ->hash;
}

void fn_7100981604(UnkUnit1452_b56* self, bool flag) {
    u64 id = PartHash_l981604(0x12a0);
    UnkAnimTime_l981604 time = reinterpret_cast<const UnkAnimTime_l981604&>(g_unk_710164b958);
    UnkAnimBlend_l981604 blend = reinterpret_cast<const UnkAnimBlend_l981604&>(g_unk_7172769740);
    u32 mask = UnkAll_SpitSend().value;
    UnkStruct_7101176ea8* animation = reinterpret_cast<UnkAnimOwner_l981604*>(fn_710097f1b8(self))->animation;
    UnkAnimRequest_l981604 req(id, time, blend, flag ? 0x11u : 0x10u, mask);
    fn_7101176ea8(animation, reinterpret_cast<const UnkRequest_7101176ea8*>(&req), 0xf4061ff25a2163d6);
}
