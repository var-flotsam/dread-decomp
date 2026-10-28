// Resets each of 15 slots to a freshly constructed value: builds a zeroed temporary,
// initialises it, move-assigns it into the slot and destroys it.
#include "types.h"

struct UnkSlot_0x40 {
    u64 words[8];
};

void fn_7100eda600(UnkSlot_0x40* self);                             // constructor
void fn_7100edbef4(UnkSlot_0x40* self, UnkSlot_0x40* other);        // move assignment
void fn_7100ed977c(UnkSlot_0x40* self);                             // destructor

struct UnkSlotTable_7100edce2c {
    UnkSlot_0x40 slots[15];
};

static inline void Reset(UnkSlotTable_7100edce2c* self, s32 i) {
    UnkSlot_0x40 tmp = {};
    fn_7100eda600(&tmp);
    fn_7100edbef4(&self->slots[i], &tmp);
    fn_7100ed977c(&tmp);
}

void fn_7100edce2c(UnkSlotTable_7100edce2c* self) {
    Reset(self, 0);
    Reset(self, 1);
    Reset(self, 2);
    Reset(self, 3);
    Reset(self, 4);
    Reset(self, 5);
    Reset(self, 6);
    Reset(self, 7);
    Reset(self, 8);
    Reset(self, 9);
    Reset(self, 10);
    Reset(self, 11);
    Reset(self, 12);
    Reset(self, 13);
    Reset(self, 14);
}
