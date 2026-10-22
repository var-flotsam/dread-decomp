// Equality of two arrays of owned-object lists (fn_71004bedf4 and its copies are the same): the lists
// must have the same lengths and hold the same object pointers.
#include "game_s2/UnkOwnedList.h"

bool fn_71004a8ddc(const base::UnkVector<UnkOwnedList>* self, const base::UnkVector<UnkOwnedList>* other) {
    if (self->count != other->count)
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        const UnkOwnedList& a = other->data[i];
        const UnkOwnedList& b = self->data[i];
        if (a.count != b.count)
            return false;
        const UnkOwnedPtr* pa = a.data;
        const UnkOwnedPtr* pb = b.data;
        u64 n = a.count;
        for (; n; --n, ++pa, ++pb)
            if (pb->ptr != pa->ptr)
                return false;
    }
    return true;
}
