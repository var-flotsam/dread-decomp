// Unit 2571: adds an amount to one of three counters, capped at its maximum, then notifies.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCounters2571 {
    u32* values;

    u32 Get(s32 index) const { return values[index]; }
};

void fn_7100ec6a6c(UnkCounters2571* self, u32 index);

void fn_7100ec6a24(UnkCounters2571* self, u32 index, u32 amount) {
    if (index > 2 || amount == 0)
        return;
    u32 max = index == 1 ? 999999000u : 999999u;
    u32 value = self->Get(index) + amount;
    self->values[index] = max > value ? value : max;
    fn_7100ec6a6c(self, index);
}
