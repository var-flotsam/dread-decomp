// CForcedMovementAreaComponent's unit: whether the entry keyed by the collider's id (+0x30) in the
// flat map at +0x48 (0x28-byte values, u64 keys at +0x50, count at +0x58) is set (+8). Names are
// placeholders.
#include "types.h"

struct UnkAreaEntry_A5_b65 {
    u64 field_0x0;
    u32 field_0x8;
    u8 pad_0xc[0x28 - 0xc];
};

struct UnkAreaKeyed_A5_b65 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkAreaMap_A5_b65 {
    u8 pad_0x0[0x48];
    UnkAreaEntry_A5_b65* values;
    u64* keys;
    u32 count;

    UnkAreaEntry_A5_b65* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

bool fn_71007ecd38(UnkAreaMap_A5_b65* self, const UnkAreaKeyed_A5_b65* collider) {
    UnkAreaEntry_A5_b65* it = self->Find(collider->id);
    if (it == self->values + self->count)
        return false;
    return it->field_0x8 != 0;
}
