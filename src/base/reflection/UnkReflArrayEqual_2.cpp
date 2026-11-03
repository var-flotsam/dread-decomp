// A copy of fn_71000e00a0 (src/base/reflection/UnkReflArrayEqual.cpp): equality of two arrays of
// 0x48-byte values compared by fn_71011136b8 (src/game_s1/fn_71000d4d50.cpp). Names are placeholders.
#include "types.h"

struct UnkCompared_0x48;
bool fn_71011136b8(const UnkCompared_0x48* a, const UnkCompared_0x48* b);

struct UnkCompared48Array_710111410c {
    u8* data;  // 0x48-byte values
    u32 count;
};

bool fn_710111410c(const UnkCompared48Array_710111410c* self, const UnkCompared48Array_710111410c* other) {
    if (self->count != other->count)
        return false;
    for (u64 i = 0; i < self->count; ++i)
        if (!fn_71011136b8(reinterpret_cast<const UnkCompared_0x48*>(other->data + i * 0x48),
                           reinterpret_cast<const UnkCompared_0x48*>(self->data + i * 0x48)))
            return false;
    return true;
}
