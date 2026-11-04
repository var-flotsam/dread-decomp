// Copies of the owned-pointer vector erase of src/game/querysystem/UnkOwnedPtrVectorErase_b41.cpp
// (fn_710036828c): the reflected erase slots (0x128) of the Kraid attacks' launch-pattern vectors, one per
// unit (CAcidBlobsLaunchPattern, CBouncingCreaturesLaunchPattern, CKraidSpinningNailsDef; tools/tu_map.py).
// The generated src/reflection_types/collection_types_00.cpp declares them untyped. Names are placeholders.
#include "game_s2/UnkOwnedList.h"
#include "types.h"

struct UnkOwnedPtrVector_A3_b68 : base::UnkVector<UnkOwnedPtr> {};

static inline __attribute__((always_inline)) void Erase_A3_b68(UnkOwnedPtrVector_A3_b68* self, const u32* index) {
    u32 at = *index;
    if (at < self->count) {
        u64 i = at;
        self->data[i].~UnkOwnedPtr();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwnedPtr* cur = &self->data[i];
            // Moves the pointer of the next slot down, leaving that slot empty.
            UnkOwned* t = cur[1].ptr;
            cur[1].ptr = nullptr;
            cur->ptr = t;
            (&self->data[i])[1].~UnkOwnedPtr();
        }
        self->count = self->count - 1;
    }
}

#define UNK_ERASE_A3_B68(addr) \
    void fn_##addr(UnkOwnedPtrVector_A3_b68* self, const u32* index) { Erase_A3_b68(self, index); }

UNK_ERASE_A3_B68(71005d95ec)
UNK_ERASE_A3_B68(71005e2638)
UNK_ERASE_A3_B68(71005ed664)
