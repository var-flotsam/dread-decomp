// An erase by index from a base::UnkVector of owning pointers (game_s2/UnkOwnedList.h's UnkOwnedPtr,
// each destroyed through its vtable slot 0x10), one copy per element type: when the index is in range,
// the entry is destroyed, each later one is moved down a slot (the moved-from slot destroyed, now
// empty) and the count decremented; the loop is the one of fn_71005dae94. The first is in the
// querysystem::CEvaluator unit (tools/tu_map.py); the others are the same code in other units.
#include "game_s2/UnkOwnedList.h"
#include "types.h"

namespace {

// Moves the pointer in `src` into the empty slot `dst`, leaving `src` empty.
inline void MoveInto_b41a1(UnkOwnedPtr* dst, UnkOwnedPtr* src) {
    UnkOwned* t = src->ptr;
    src->ptr = nullptr;
    dst->ptr = t;
}

}  // namespace

struct UnkOwnedPtrVector_b41a1 : base::UnkVector<UnkOwnedPtr> {};

static inline __attribute__((always_inline)) void Erase_b41a1(UnkOwnedPtrVector_b41a1* self, const u32* index) {
    u32 at = *index;
    if (at < self->count) {
        u64 i = at;
        self->data[i].~UnkOwnedPtr();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwnedPtr* cur = &self->data[i];
            MoveInto_b41a1(cur, &cur[1]);
            (&self->data[i])[1].~UnkOwnedPtr();
        }
        self->count = self->count - 1;
    }
}

#define UNK_ERASE_B41A1(addr) \
    void fn_##addr(UnkOwnedPtrVector_b41a1* self, const u32* index) { Erase_b41a1(self, index); }

UNK_ERASE_B41A1(71002b9908)
UNK_ERASE_B41A1(71002c03c0)
UNK_ERASE_B41A1(71002de760)
UNK_ERASE_B41A1(71002f5634)
UNK_ERASE_B41A1(710036828c)
UNK_ERASE_B41A1(7100460a2c)
UNK_ERASE_B41A1(71004a7a94)
UNK_ERASE_B41A1(71004b1570)
UNK_ERASE_B41A1(71004bdce0)
UNK_ERASE_B41A1(7100561be8)
UNK_ERASE_B41A1(7100562fa8)
UNK_ERASE_B41A1(71005646bc)
UNK_ERASE_B41A1(7100578364)
UNK_ERASE_B41A1(71005b12ec)
