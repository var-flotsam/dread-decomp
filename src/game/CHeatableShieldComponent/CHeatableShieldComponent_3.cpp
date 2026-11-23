// In the CHeatableShieldComponent unit: hands each 0xd0-byte entry of the array at +0x40 (count at
// +0x50) the owner at +0x30, then runs fn_710086e410 on it (as fn_710086c768 does without the
// owner). Names and layouts are placeholders.
#include "game/UnkHeatableShieldEntries.h"
#include "game/UnkTU1323.h"
#include "types.h"

struct UnkHeatableShieldEntry {
    u8 data[0xc8];
    void* owner;  // 0xc8
};

void fn_710086b268(UnkHeatableShieldEntries* self) {
    for (UnkHeatableShieldEntry *p = self->data, *end = p + self->count; p != end; ++p) {
        p->owner = self->owner;
        fn_710086e410(p);
    }
}
