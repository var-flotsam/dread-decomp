// Equality of two arrays of 0x18-byte values (base::reflection's unit): the same count and each
// value equal by fn_71000a1ed4, as src/game_s1/fn_71000d4d50.cpp compares them (fn_71000d56ac); and
// 0x30-byte values compared by fn_710007f120 (fn_71000e00a0).
// Names are placeholders.
#include "base/reflection.h"
#include "game/UnkTU57.h"
#include "types.h"

struct UnkCompared_0x18 {
    u64 words[3];
};

struct UnkComparedArray_71000d56ac {
    UnkCompared_0x18* data;
    u32 count;
};

bool fn_71000d56ac(const UnkComparedArray_71000d56ac* self, const UnkComparedArray_71000d56ac* other) {
    if (self->count != other->count)
        return false;
    for (u64 i = 0; i < self->count; ++i)
        if (!fn_71000a1ed4(&other->data[i], &self->data[i]))
            return false;
    return true;
}

// The same for 0x30-byte values compared by fn_710007f120 (fn_71000e00a0).
struct UnkCompared_0x30 {
    u64 words[6];
};

struct UnkComparedArray_71000e00a0 {
    UnkCompared_0x30* data;
    u32 count;
};

bool fn_71000e00a0(const UnkComparedArray_71000e00a0* self, const UnkComparedArray_71000e00a0* other) {
    if (self->count != other->count)
        return false;
    for (u64 i = 0; i < self->count; ++i)
        if (!fn_710007f120(&other->data[i], &self->data[i]))
            return false;
    return true;
}
