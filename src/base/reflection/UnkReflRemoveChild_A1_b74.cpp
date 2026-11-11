// tools/tu_map.py unit 281 (base::reflection): drops a pointer from the owner's packed pointer list (data at 0xa8,
// count at 0xb0; the gap is closed with memcpy as in the original), then hands it to the object at 0xa0
// (fn_71001eed18) when there is one. Names and layout are placeholders.
#include "base/reflection.h"
#include "types.h"

struct UnkReflRemoveOwner_A1_b74 {
    u8 pad_0x0[0xa0];
    void* target;  // 0xa0
    void** items;  // 0xa8
    u32 count;     // 0xb0
};

void fn_7100202c5c(UnkReflRemoveOwner_A1_b74* self, void* item) {
    u32 n = self->count;
    for (u32 i = 0; i < n; ++i) {
        if (self->items[i] == item) {
            __builtin_memcpy(&self->items[i], &self->items[i] + 1, (u64)(n - i - 1) * sizeof(void*));
            self->count--;
            break;
        }
    }
    if (self->target)
        fn_71001eed18(self->target, item);
}
