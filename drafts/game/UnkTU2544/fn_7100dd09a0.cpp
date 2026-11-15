// CBossRushSurvivalConfiguration's unit (unit 2544): equality of two small key/value tables (each key of
// one must be in the other with the same float value). Names and layouts are placeholders.
#include "types.h"

struct UnkKeyValueTable_dd09a0 {
    f32* values;  // 0x0
    u32* keys;    // 0x8
    u32 count;    // 0x10
};

bool fn_7100dd09a0(const UnkKeyValueTable_dd09a0* self, const UnkKeyValueTable_dd09a0* other) {
    u64 count = self->count;
    if (count != other->count)
        return false;
    const f32* otherValues = other->values;
    const u32* otherKeys = other->keys;
    for (u64 i = 0; i != count; i++) {
        u64 j = 0;
        while (otherKeys[j] != self->keys[i]) {
            if (++j == count)
                return false;
        }
        if (j == count || otherValues[j] != self->values[i])
            return false;
    }
    return true;
}
