// CLogicShapeComponent's unit: sets the bool stored under a key in a flat map (bool values at +0,
// pointer keys at +8, count at +0x10), when the key is there. Two identical copies. Names are placeholders.
#include "types.h"

struct UnkLogicShapeBoolMap_A5_b66 {
    bool* values;
    void** keys;
    u32 count;

    bool* Find(void* const& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

void fn_71009164ec(UnkLogicShapeBoolMap_A5_b66* self, void* const* key, const bool* value) {
    bool* it = self->Find(*key);
    if (it != self->values + self->count)
        *it = *value;
}

void fn_7100916558(UnkLogicShapeBoolMap_A5_b66* self, void* const* key, const bool* value) {
    bool* it = self->Find(*key);
    if (it != self->values + self->count)
        *it = *value;
}
