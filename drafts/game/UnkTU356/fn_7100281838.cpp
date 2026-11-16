// Unit 356 (navmesh query state): takes the cheapest node off the open list, a binary heap on the estimated
// total cost (src/game/navmesh/UnkNavMeshQueryState.cpp pushes onto it). Names and layouts are placeholders.
#include <string.h>

#include <algorithm>

#include "types.h"

// A search node: only the two costs the heap orders by.
struct UnkNavOpenNode_l281838 {
    u8 pad_0x0[0x114];
    f32 costs[2];  // 0x114: the cost so far and the estimate to the goal

    f32 Cost() const { return costs[0]; }
    f32 Total() const { return costs[0] + costs[1]; }
};

// The open list: a fixed-capacity array of node pointers with byte storage right after its count.
struct UnkNavOpenList_l281838 {
    u32 count;
    u8 storage[1200 * sizeof(UnkNavOpenNode_l281838*)];

    UnkNavOpenNode_l281838** begin() { return reinterpret_cast<UnkNavOpenNode_l281838**>(storage); }
    UnkNavOpenNode_l281838** end() { return begin() + count; }
    void Resize(u32 n) {
        if (n > count)
            memset(begin() + count, 0, (n - count) * sizeof(UnkNavOpenNode_l281838*));
        count = n;
    }
};

void fn_7100281838(UnkNavOpenList_l281838* open) {
    // A min-heap on the total, ties going to the larger cost so far.
    auto greater = [](const UnkNavOpenNode_l281838* a, const UnkNavOpenNode_l281838* b) {
        f32 ta = a->Total();
        f32 tb = b->Total();
        return ta > tb || (ta == tb && a->Cost() < b->Cost());
    };
    std::pop_heap(open->begin(), open->end(), greater);
    open->Resize(open->count - 1);
}
