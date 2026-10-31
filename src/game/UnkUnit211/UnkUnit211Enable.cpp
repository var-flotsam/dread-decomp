// From unit 211 of tools/tu_map.py (0x7100190154-0x7100194a9c, no reflected class): sets the flag at
// +0x258; turning it off calls fn_710016e42c on each of the items at +0x78 (as many as the count at
// +0x2c). Names and layouts are placeholders.
#include "types.h"

void fn_710016e42c(void* item);

struct UnkUnit211Object {
    u8 pad_0x0[0x2c];
    u32 count;  // 0x2c
    u8 pad_0x30[0x48];
    void** items;  // 0x78
    u8 pad_0x80[0x258 - 0x80];
    bool enabled;  // 0x258
};

void fn_7100192f08(UnkUnit211Object* self, bool enabled) {
    if (self->enabled != enabled && !enabled) {
        for (u64 i = 0; i < self->count; i++)
            fn_710016e42c(self->items[i]);
    }
    self->enabled = enabled;
}
