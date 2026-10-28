// Erases the list at the given index from an array of owned-object lists (fn_71004a9570 and its
// copies are the same): destroys it and moves the later lists down by one (UnkVector::EraseAt,
// inlined; the moved-from list is destroyed again, its storage reloaded through the array).
#include "game_s2/UnkOwnedList.h"

void fn_71004bf588(base::UnkVector<UnkOwnedList>* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkOwnedList();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwnedList* cur = &self->data[i];
            new (cur) UnkOwnedList(static_cast<UnkOwnedList&&>(cur[1]));
            (&self->data[i])[1].~UnkOwnedList();
        }
        self->count = self->count - 1;
    }
}
