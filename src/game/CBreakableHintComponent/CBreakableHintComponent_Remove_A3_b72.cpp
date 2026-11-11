// CBreakableHintComponent unit (tools/tu_map.py, the name from reflection): fn_710075312c removes the entry
// stored under a key from the flat map at +0x48 (0x18-byte values, 8-byte keys at +0x50, count at +0x58,
// erase fn_7100752a78); once the map is empty it sets the flag at +0x41 and calls fn_71007523e8. Always
// returns false. Names and layouts are placeholders.
#include "game/CBreakableHintComponent.h"
#include "types.h"

struct UnkBreakableHintEntry_A3_b72 {
    u8 data[0x18];
};

struct UnkBreakableHintMap_A3_b72 {
    UnkBreakableHintEntry_A3_b72* values;
    u64* keys;
    u32 count;

    UnkBreakableHintEntry_A3_b72* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

struct UnkBreakableHint_A3_b72 {
    u8 pad_0x0[0x41];
    bool field_0x41;
    u8 pad_0x42[0x6];
    UnkBreakableHintMap_A3_b72 map;  // 0x48
};

void* fn_7100752a78(UnkBreakableHintMap_A3_b72* map, UnkBreakableHintEntry_A3_b72* it);

bool fn_710075312c(UnkBreakableHint_A3_b72* self, u64 key) {
    UnkBreakableHintEntry_A3_b72* it = self->map.Find(key);
    if (it != self->map.values + self->map.count)
        fn_7100752a78(&self->map, it);
    if (self->map.count == 0) {
        self->field_0x41 = true;
        fn_71007523e8(self);
    }
    return false;
}
