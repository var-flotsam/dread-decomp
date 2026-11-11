// Unit 1452 (tools/tu_map.py, no name hint): fn_7100980234 handles a message whose class is
// fn_7100cc21a8's (or derives from it): when the owner (fn_710097f1b8) has its sub-object at +0x220
// and the flag at +0x360 of the object at +0x2b0 is clear, sends that sub-object the hash of the
// message's string id (+0x10) with mode 2 and, in place of UnkSpitSend's first descriptor
// (src/game_s2/UnkSpitSend.h), a block {-1, 0.18f, 1}, unless the id is the pool's empty one.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1450.h"
#include "types.h"

struct UnkSendParams_b56 {
    s64 field_0x0;
    f32 field_0x8;
    u32 field_0xc;
};

struct UnkUnit1452Flags_b56 {
    u8 pad_0x0[0x360];
    bool field_0x360;
};

struct UnkUnit1452Owner_b56 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x88];
    UnkUnit1452Flags_b56* field_0x2b0;
};

struct UnkUnit1452_b56;

struct UnkUnit1452Message_b56 : base::reflection::UnkReflObject {
    base::global::UnkStringData* field_0x8;
    base::global::UnkStringData* field_0x10;  // CStrId
};

// The mode word as UnkReflTypeIdArg's first field, passed by value so that it and the block are
// stored (and placed) as the original's: mode, then the block, then the caller's mask.
struct UnkSendMode_b56 {
    u32 id;
};

static inline void SendParams_b56(void* target, u64 hash, UnkSendMode_b56 mode, UnkSendParams_b56 params,
                                  base::reflection::UnkReflTypeIdArg* mask) {
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, reinterpret_cast<base::reflection::UnkReflTypeIdArg*>(&mode),
                  &params, &g_unk_7172769740, mask);
}

void fn_7100980234(UnkUnit1452_b56* self, UnkUnit1452Message_b56* msg) {
    if (!fn_710009d50c(msg->vf_0x0(), fn_7100cc21a8()))
        return;
    UnkUnit1452Owner_b56* owner = fn_710097f1b8(self);
    void* target = owner->field_0x220;
    if (!target || owner->field_0x2b0->field_0x360)
        return;
    base::global::UnkStringData* id = msg->field_0x10;
    if (id == &g_unk_7101d47108->empty)
        return;
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id) + 0x30);
    base::reflection::UnkReflTypeIdArg mask;
    SendParams_b56(target, hash, {2}, {-1, 0.18f, 1}, &mask);
}
