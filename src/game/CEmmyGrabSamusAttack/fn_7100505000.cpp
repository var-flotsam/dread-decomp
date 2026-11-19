// CEmmyGrabSamusAttack unit (tools/tu_map.py): builds a fixed array of four strings with 0x20 bytes of
// inline storage and copies up to four given strings into it, in order. Names and layouts are
// placeholders.
#include <new>

#include "game/CEmmyGrabSamusAttack.h"
#include "game/UnkInlineStr50.h"
#include "types.h"

struct UnkInlineStrArray_l505000 {
    UnkInlineStr50 strs[4];
};

// The count is a 64-bit value in the original (it is tested and walked whole), though the capacity
// check reads only its low half.
void fn_7100505000(void* storage, const UnkInlineStr50* strs, u64 count) {
    auto* self = new (storage) UnkInlineStrArray_l505000;
    const u32 n = static_cast<u32>(count);
    if (n > sizeof(self->strs) / sizeof(self->strs[0]))
        return;
    u32 i = 0;
    for (const UnkInlineStr50* s = strs; s != strs + count; ++s)
        self->strs[i++] = UnkInlineStr50(*s);
}
