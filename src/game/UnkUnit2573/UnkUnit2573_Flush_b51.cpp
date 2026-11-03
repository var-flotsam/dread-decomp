// Unit 2573 (tools/tu_map.py; no reflected class): when the pending flag at +0x14b is set, clears the
// nine flags there and, unless the value at +0x170 is within 0.1 of zero, hands the record at +0x158
// and `source` (or the default at +0xd0) to the manager at g_unk_7101d47318 (fn_7101024b28).
// Names and layouts are placeholders.
#include "types.h"

struct UnkDropTarget;
extern UnkDropTarget* g_unk_7101d47318;
void fn_7101024b28(UnkDropTarget* self, void* record, void* source, void* a, u32 b);

struct UnkUnit2573_b51 {
    u8 unk_0x0[0xd0];
    u8 source[0x14b - 0xd0];  // 0xd0
    bool flags[9];            // 0x14b
    u8 unk_0x154[4];
    u8 record[0x170 - 0x158];  // 0x158
    f32 value;                 // 0x170
};

void fn_7100ecf450(UnkUnit2573_b51* self, void* source) {
    if (!self->flags[0])
        return;
    __builtin_memset(self->flags, 0, sizeof(self->flags));
    f32 v = self->value;
    if (!(v + 0.1f >= 0.0f) || !(v - 0.1f <= 0.0f))
        fn_7101024b28(g_unk_7101d47318, self->record, source ? source : self->source, nullptr, 0);
}
