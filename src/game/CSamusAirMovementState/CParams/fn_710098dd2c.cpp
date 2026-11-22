// CSamusAirMovementState::CParams's unit (tools/tu_map.py unit 1455): a target visitor that keeps up to two
// targets in a fixed list at +0x8 (a count, then the records). Results: 1 added, 2 full, 3 added and now
// full. Names and layouts are placeholders.
#include "game/CAimComponent.h"
#include "game/UnkVisitResult_b0040.h"
#include "types.h"

#include <new>

// A fixed-capacity list: a count, then room for N elements.
template <typename T, int N>
struct UnkFixedList_l98dd2c {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }

    // Adds a copy of the value: 1 added, 2 full, 3 added and now full.
    UnkVisitResult_b0040 Add(const T& value) {
        s32 free = N - count;
        if (free < 1)
            return 2;
        new (&begin()[count]) T(value);
        ++count;
        return free == 1 ? 3 : 1;
    }
};

struct UnkTargetPair_l98dd2c {
    void* vtable;
    UnkFixedList_l98dd2c<UnkAimTarget, 2> targets;  // 0x8
};

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
UnkVisitResult_b0040 fn_710098dd2c(UnkTargetPair_l98dd2c* self, const UnkAimTarget& target) {
    return self->targets.Add(target);
}
