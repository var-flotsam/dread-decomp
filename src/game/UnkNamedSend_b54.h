#pragma once

// UnkSpitSend (src/game_s2/UnkSpitSend.h) with its second descriptor at 0x717276974c instead of
// 0x7172769740: mode 0, the same kind hash and mask (two door and grapple-point components).

#include "../game_s2/UnkSpitSend.h"

extern UnkDesc_SpitSend g_unk_717276974c;  // reached through the GOT

inline void UnkSpitSend_974c(void* target, u64 hash, u64 kind) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_717276974c, &mask);
}
