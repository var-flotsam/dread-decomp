// In the CHeatableShieldComponent unit: runs fn_710086e410 on each 0xd0-byte entry of the array at
// +0x40 (count at +0x50). Names and layouts are placeholders.
#include "game/UnkHeatableShieldEntries.h"
#include "game/UnkTU1323.h"
#include "types.h"

struct UnkHeatableShieldEntry {
    u8 data[0xd0];
};

void fn_710086c768(UnkHeatableShieldEntries* self) {
    for (UnkHeatableShieldEntry *p = self->data, *end = p + self->count; p != end; ++p)
        fn_710086e410(p);
}
