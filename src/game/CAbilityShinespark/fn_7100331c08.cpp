// CAbilityShinespark's unit (tools/tu_map.py unit 433): adds a target record to an eight-record list.
// Results: 1 added, 2 full, 3 added and now full. Names and layouts are placeholders.
#include "game/CAimComponent.h"
#include "game/UnkVisitResult_b0040.h"
#include "types.h"

#include <new>

// A fixed-capacity list: a count, then room for N elements.
template <typename T, int N>
struct alignas(8) UnkFixedList_l331c08 {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }
};

struct UnkTargetList_l331c08 {
    void* vtable;
    UnkFixedList_l331c08<UnkAimTarget, 8> targets;  // 0x8
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkVisitResult_b0040 fn_7100331c08(UnkTargetList_l331c08* self, const UnkAimTarget& target) {
    u32 count = self->targets.count;
    s32 free = 8 - count;
    if (free <= 0)
        return 2;
    new (&self->targets.begin()[count]) UnkAimTarget(target);
    self->targets.count++;
    return free == 1 ? 3 : 1;
}
