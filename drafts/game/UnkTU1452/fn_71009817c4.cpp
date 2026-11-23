// Unit 1452 (tools/tu_map.py): asks the object fn_710097f198 gives (vf_0x2e8) for a message; when it names a
// target (id not -1, +0x2 not 0xff) closer than 42, plays animation 0x8efe16c30efce215 on the owner's
// (fn_710097f1b8) animation (+0x220), else the one whose id hash is the singleton's part at +0x1da0, with the
// default time and blend, flags 0x10 (bit 0 from the caller's flag) and the all-bits mask (as fn_7100981604).
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU1450.h"
#include "game/UnkTU363.h"
#include "types.h"

struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;

struct UnkSmBase_b60;
struct UnkSmOther_b60;

struct UnkMsg_b152 {
    s16 id;
    u8 field_0x2;
    u8 pad_0x3[9];
    f32 field_0xc;
};

struct UnkMsgSource_l9817c4 {
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
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual void vf_0x230();
    virtual void vf_0x238();
    virtual void vf_0x240();
    virtual void vf_0x248();
    virtual void vf_0x250();
    virtual void vf_0x258();
    virtual void vf_0x260();
    virtual void vf_0x268();
    virtual void vf_0x270();
    virtual void vf_0x278();
    virtual void vf_0x280();
    virtual void vf_0x288();
    virtual void vf_0x290();
    virtual void vf_0x298();
    virtual void vf_0x2a0();
    virtual void vf_0x2a8();
    virtual void vf_0x2b0();
    virtual void vf_0x2b8();
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual void vf_0x2d0();
    virtual void vf_0x2d8();
    virtual void vf_0x2e0();
    virtual void vf_0x2e8(UnkMsg_b152* msg, bool* flag, u64 a, u64 b);
};

struct UnkPartId_l9817c4 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAnimTime_l9817c4 {
    u64 a;
    u64 b;
};

struct UnkAnimBlend_l9817c4 {
    u32 a;
    u32 b;
    u32 c;
};

struct UnkAnimRequest_l9817c4 {
    u64 id;
    UnkAnimTime_l9817c4 time;
    UnkAnimBlend_l9817c4 blend;
    u32 flags;
    u32 mask;

    UnkAnimRequest_l9817c4(u64 id_, UnkAnimTime_l9817c4 time_, UnkAnimBlend_l9817c4 blend_, u32 flags_, u32 mask_)
        : id(id_), time(time_), blend(blend_), flags(flags_), mask(mask_) {}
};

struct UnkAnimOwner_l9817c4 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;
};

static inline u64 PartHash_l9817c4(u64 offset) {
    return (*reinterpret_cast<UnkPartId_l9817c4**>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) +
                                                    offset))
        ->hash;
}

static inline UnkStruct_7101176ea8* Animation_l9817c4(UnkUnit1452_b56* self) {
    return reinterpret_cast<UnkAnimOwner_l9817c4*>(fn_710097f1b8(self))->animation;
}

void fn_71009817c4(UnkUnit1452_b56* self, bool flag) {
    UnkMsgSource_l9817c4* source =
        reinterpret_cast<UnkMsgSource_l9817c4*>(fn_710097f198(reinterpret_cast<UnkSmBase_b60*>(self)));
    UnkMsg_b152 msg;
    fn_71002878dc(&msg);
    source->vf_0x2e8(&msg, nullptr, 0, 0);
    u64 id;
    UnkAnimTime_l9817c4 time;
    UnkAnimBlend_l9817c4 blend;
    u32 mask;
    UnkStruct_7101176ea8* animation;
    if (msg.id != -1 && msg.field_0x2 != 0xff && msg.field_0xc < 42.0f) {
        id = 0x8efe16c30efce215;
        time = reinterpret_cast<const UnkAnimTime_l9817c4&>(g_unk_710164b958);
        blend = reinterpret_cast<const UnkAnimBlend_l9817c4&>(g_unk_7172769740);
        mask = UnkAll_SpitSend().value;
        animation = Animation_l9817c4(self);
    } else {
        id = PartHash_l9817c4(0x1da0);
        time = reinterpret_cast<const UnkAnimTime_l9817c4&>(g_unk_710164b958);
        blend = reinterpret_cast<const UnkAnimBlend_l9817c4&>(g_unk_7172769740);
        mask = UnkAll_SpitSend().value;
        animation = Animation_l9817c4(self);
    }
    UnkAnimRequest_l9817c4 req(id, time, blend, flag ? 0x11u : 0x10u, mask);
    fn_7101176ea8(animation, reinterpret_cast<const UnkRequest_7101176ea8*>(&req), 0xf4061ff25a2163d6);
    fn_7100287954(&msg);
}
