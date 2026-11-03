// From units 296 and 297 of tools/tu_map.py (0x710020f450-0x710020ffbc): hands every occupied slot
// of a fixed slot table (pointers from +8, the slot count after them) to a visitor's third virtual.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSlotVisitor {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Visit(void* item);
};

template <u32 Slots>
struct UnkSlotTable {
    void* vtable;
    void* slots[Slots];  // 0x8
    u32 count;
};

template <typename Table>
__attribute__((always_inline)) static inline void VisitSlots(Table* table, UnkSlotVisitor* visitor) {
    u64 n = table->count;
    if (n == 0)
        return;
    void** it = table->slots;
    do {
        if (void* item = *it)
            visitor->Visit(item);
        ++it;
    } while (--n != 0);
}

void fn_710020f8bc(UnkSlotTable<0x1400>* table, UnkSlotVisitor* visitor) {
    VisitSlots(table, visitor);
}

void fn_710020fe60(UnkSlotTable<0xa000>* table, UnkSlotVisitor* visitor) {
    VisitSlots(table, visitor);
}
