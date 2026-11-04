// From unit 233 of tools/tu_map.py (no class hint): when the object is enabled (+0) and has its link at
// +0x10, looks a 32-bit key up in its flat map (values +0x18, keys +0x20, count +0x28) and returns the
// value, or 0 when absent. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit233Map_A1_b73 {
    bool enabled;
    u8 pad_0x1[0xf];
    void* link;    // 0x10
    u32* values;   // 0x18
    u32* keys;     // 0x20
    u32 count;     // 0x28

    u32* Find(u32 key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return values + count;
    }
};

u32 fn_71001abbe0(UnkUnit233Map_A1_b73* self, u32 key) {
    if (!self->link || !self->enabled)
        return 0;
    u32* v = self->Find(key);
    if (v == self->values + self->count)
        return 0;
    return *v;
}
