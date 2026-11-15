// UnkTU2772 (nn::atk): maps a block's type id 0x2201..0x2203 to a value in a table. Names and layouts are placeholders.
#include "types.h"

struct UnkAtkBlock {
    u8 pad_0x0[0xc];
    u16 type;  // 0xc
};

__attribute__((visibility("hidden"))) extern const s32 g_unk_710164d5ac[3];

s32 fn_710120ff8c(const UnkAtkBlock* block) {
    u32 index = block->type - 0x2201;
    if (index <= 2)
        return g_unk_710164d5ac[static_cast<s64>(static_cast<s16>(index))];
    return 0;
}
