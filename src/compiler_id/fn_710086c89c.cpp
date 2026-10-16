// True when every 0xd0-byte element has field_0xa1 == 1 (vacuously true when empty).
// Not matching: same instructions, different register assignment and increment order.
#include "types.h"

struct UnkStruct_0xd0 {
    u8 unk_0x0[0xa1];
    u8 field_0xa1;
    u8 unk_0xa2[0xd0 - 0xa2];
};

struct UnkStruct_0x58 {
    u8 unk_0x0[0x40];
    UnkStruct_0xd0* data;
    u8 unk_0x48[0x8];
    u32 count;
};

bool fn_710086c89c(const UnkStruct_0x58* self) {
    for (const UnkStruct_0xd0* it = self->data; it != self->data + self->count; ++it) {
        if (it->field_0xa1 != 1)
            return false;
    }
    return true;
}
