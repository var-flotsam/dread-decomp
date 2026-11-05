// Units 364 and 369 (tools/tu_map.py, no class hint): two identical destructors of an object holding an
// array of arrays at 0 and the pair of slot pools (base::refl_SlotPoolPairDtor, the generated clones
// fn_71002880d4 and fn_71002b17a0) at +0x28: the pools go first, then every inner array is freed and
// the outer one with them. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkClone_fn_71002880d4;
struct UnkClone_fn_71002b17a0;
extern template void base::refl_SlotPoolPairDtor<UnkClone_fn_71002880d4>(char*);
extern template void base::refl_SlotPoolPairDtor<UnkClone_fn_71002b17a0>(char*);

// An inner array of unknown trivially destructible elements, freed when destroyed.
struct UnkSlotPoolOwnerList_b64a2 {
    base::UnkVector<u8> items;
    ~UnkSlotPoolOwnerList_b64a2() { items.Free(); }
};

template <class Tag>
struct UnkSlotPoolOwner_b64a2 {
    base::UnkVector<UnkSlotPoolOwnerList_b64a2> lists;
    char pools[0x100];  // base::UnkSlotPool pair
};

template <class Tag>
static inline __attribute__((always_inline)) void Destroy(UnkSlotPoolOwner_b64a2<Tag>* self) {
    base::refl_SlotPoolPairDtor<Tag>(self->pools);
    self->lists.Free();
}

void fn_7100288028(UnkSlotPoolOwner_b64a2<UnkClone_fn_71002880d4>* self) { Destroy(self); }
void fn_71002b16f4(UnkSlotPoolOwner_b64a2<UnkClone_fn_71002b17a0>* self) { Destroy(self); }
