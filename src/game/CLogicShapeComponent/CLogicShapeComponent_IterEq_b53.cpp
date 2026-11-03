// CLogicShapeComponent unit (tools/tu_map.py): iterator-value helpers for a one-byte flag element keyed by a
// string id: the by-value comparison (refl_IterValueEqualByValue, fn_71000cea70's family) and the move
// (refl_IterValueMove, as fn_7100251208 in src/game_s1/fn_71000cd580.cpp).
#include "UnkClone.h"
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"

// A flag in an 8-byte slot, copied member by member through a user-written copy constructor.
template <class Tag>
struct alignas(8) UnkFlagSlot_b53 {
    bool value;

    UnkFlagSlot_b53() {}
    UnkFlagSlot_b53(const UnkFlagSlot_b53& other) { value = other.value; }
    bool operator==(const UnkFlagSlot_b53& other) const { return value == other.value; }
};

#define DREAD_FLAG_ITER_EQ_B53(addr)                                                                     \
    struct UnkClone_fn_##addr;                                                                           \
    template bool base::refl_IterValueEqualByValue(                                                      \
        const base::UnkIterValue<UnkFlagSlot_b53<UnkClone_fn_##addr>, UnkCloneStrId<UnkClone_fn_##addr>>&, \
        const base::UnkIterValue<UnkFlagSlot_b53<UnkClone_fn_##addr>, UnkCloneStrId<UnkClone_fn_##addr>>&);

DREAD_FLAG_ITER_EQ_B53(7100915a7c)
DREAD_FLAG_ITER_EQ_B53(7100915fb8)

// A string id moved out by taking its data and leaving the pool's empty string (as in
// src/game_s1/fn_71000cd580.cpp).
struct UnkMovedStrId_b53 {
    base::global::UnkStringData* data;

    UnkMovedStrId_b53(UnkMovedStrId_b53&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
};


#define DREAD_FLAG_ITER_MOVE_B53(addr)                                    \
    __attribute__((flatten)) void fn_##addr(void* dst, const void* src) { \
        base::refl_IterValueMove<bool, UnkMovedStrId_b53>(dst, src); \
    }

DREAD_FLAG_ITER_MOVE_B53(7100915b50)
DREAD_FLAG_ITER_MOVE_B53(710091608c)
