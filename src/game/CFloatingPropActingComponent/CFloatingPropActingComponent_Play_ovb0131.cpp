// CFloatingPropActingComponent's unit (tools/tu_map.py unit 1253): plays the animation 0xebbf0dc6e95286de
// on the object at +0x40 (its +0x10) through fn_7101176ea8 with key 0x459ee01c881801a, a random start
// in [0, 1] and the all-flags mask, then sets the flag at +0x9c. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

extern "C" int rand();

struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;
bool fn_7101176ea8(UnkStruct_7101176ea8* self, const UnkRequest_7101176ea8* req, u64 key);

struct UnkFloatingPropDesc_ovb0131 {
    u64 a, b;
};

// The request record with the mask after it.
struct UnkFloatingPropRequest_ovb0131 {
    u64 id;
    UnkFloatingPropDesc_ovb0131 desc;
    u32 field_0x18;
    u32 field_0x1c;
    f32 start;  // 0x20
    u32 flags;  // 0x24
    u32 mask;   // 0x28
};

struct UnkFloatingPropHolder_ovb0131 {
    u8 pad_0x0[0x10];
    UnkStruct_7101176ea8* animation;  // 0x10
};

struct UnkFloatingProp_ovb0131 {
    u8 pad_0x0[0x40];
    UnkFloatingPropHolder_ovb0131* holder;  // 0x40
    u8 pad_0x48[0x54];
    bool playing;  // 0x9c
};

void fn_71007e64d0(UnkFloatingProp_ovb0131* self) {
    const UnkFloatingPropDesc_ovb0131& source = *reinterpret_cast<const UnkFloatingPropDesc_ovb0131*>(&g_unk_710164b958);
    u64 desc_a = source.a;
    u64 desc_b = source.b;
    u32 mask = UnkAll_SpitSend().value;
    UnkFloatingPropRequest_ovb0131 req;
    req.desc.a = desc_a;
    req.desc.b = desc_b;
    req.id = 0xebbf0dc6e95286deull;
    req.flags = 0;
    req.mask = mask;
    f32 start = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
    req.field_0x18 = 0;
    req.field_0x1c = 2;
    req.start = start;
    UnkStruct_7101176ea8* animation = self->holder ? self->holder->animation : nullptr;
    fn_7101176ea8(animation, reinterpret_cast<const UnkRequest_7101176ea8*>(&req), 0x459ee01c881801aull);
    self->playing = true;
}
