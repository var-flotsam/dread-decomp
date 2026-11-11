// In the CHeatableShieldComponent unit: runs fn_710086e410 on each 0xd0-byte entry of the array at
// +0x40 (count at +0x50). Names and layouts are placeholders.
#include "game/UnkTU1323.h"
#include "types.h"

struct UnkHeatableShieldEntry {
    u8 data[0xd0];
};

struct UnkHeatableShieldEntries {
    u8 pad_0x0[0x40];
    UnkHeatableShieldEntry* data;
    u8 pad_0x48[0x8];
    u32 count;
};

void fn_710086c768(UnkHeatableShieldEntries* self) {
    for (UnkHeatableShieldEntry *p = self->data, *end = p + self->count; p != end; ++p)
        fn_710086e410(p);
}
