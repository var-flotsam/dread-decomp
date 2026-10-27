// nn::atk::detail::StreamSound (the atk library unit, tools/tu_map.py unit 2772; the game links its
// own copy of the library): a getter the sound system's start-up reaches. Names and layouts are
// placeholders.
#include "types.h"

// The stream player at +0x300.
struct UnkAtkStreamPlayer;
u32 fn_7101221064(UnkAtkStreamPlayer* player);

struct UnkAtkStreamSound {
    u8 unk_0x0[0xf0];
    bool flag_0xf0;  // started (as nn::atk::detail::SequenceSound's, src/atk_id/fn_710121b93c.cpp)
    u8 unk_0xf1[0x300 - 0xf1];
    u8 player[1];  // +0x300
};

// What the stream player reports once the sound is started; 0 before.
u32 fn_710121e69c(UnkAtkStreamSound* self) {
    if (!self->flag_0xf0)
        return 0;
    return fn_7101221064(reinterpret_cast<UnkAtkStreamPlayer*>(self->player));
}
