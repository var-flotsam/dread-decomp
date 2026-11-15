// SLogicPathNode unit: erases the 0x50-byte element (UnkItem, include/game_s2/UnkItem50.h) of an array
// at an index: the later elements move down one. Names are placeholders.
#include <new>

#include "game_s2/UnkItem50.h"

void fn_710105edb8(base::global::CRntVector<UnkItem>* self, const u32* index) {
    u32 idx = *index;
    if (idx < self->count) {
        self->data[idx].~UnkItem();
        for (u64 i = idx; i < self->count - 1; ++i) {
            new (&self->data[i]) UnkItem(static_cast<UnkItem&&>(self->data[i + 1]));
            self->data[i + 1].~UnkItem();
        }
        --self->count;
    }
}
