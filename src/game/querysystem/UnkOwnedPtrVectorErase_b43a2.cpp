// Copies of fn_710036828c (src/game/querysystem/UnkOwnedPtrVectorErase_b41.cpp) in six more units: an
// erase by index from a base::UnkVector of owning pointers (game_s2/UnkOwnedList.h's UnkOwnedPtr, each
// destroyed through its vtable slot 0x10). When the index is in range, the entry is destroyed, each later
// one is moved down a slot (the moved-from slot destroyed, now empty) and the count decremented.
#include "game_s2/UnkOwnedList.h"
#include "types.h"

namespace {

// Moves the pointer in `src` into the empty slot `dst`, leaving `src` empty.
inline void MoveInto_b43a2(UnkOwnedPtr* dst, UnkOwnedPtr* src) {
    UnkOwned* t = src->ptr;
    src->ptr = nullptr;
    dst->ptr = t;
}

}  // namespace

struct UnkOwnedPtrVector_b43a2 : base::UnkVector<UnkOwnedPtr> {};

static inline __attribute__((always_inline)) void Erase_b43a2(UnkOwnedPtrVector_b43a2* self, const u32* index) {
    u32 at = *index;
    if (at < self->count) {
        u64 i = at;
        self->data[i].~UnkOwnedPtr();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwnedPtr* cur = &self->data[i];
            MoveInto_b43a2(cur, &cur[1]);
            (&self->data[i])[1].~UnkOwnedPtr();
        }
        self->count = self->count - 1;
    }
}

#define UNK_ERASE_B43A2(addr) \
    void fn_##addr(UnkOwnedPtrVector_b43a2* self, const u32* index) { Erase_b43a2(self, index); }

UNK_ERASE_B43A2(710070c400)
UNK_ERASE_B43A2(7100778e9c)
UNK_ERASE_B43A2(71007cbad4)
UNK_ERASE_B43A2(71007f26a0)
UNK_ERASE_B43A2(71009fd5f0)
UNK_ERASE_B43A2(7100a572f0)
