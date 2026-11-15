// UnkTU2523 (CEntity): removes the entry at an index from a vector of 0x40-byte polymorphic entries,
// moving the later ones down. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CDemolitionBlockComponent.h"
#include "types.h"

struct UnkEraseEntry_ld88030 {
    virtual ~UnkEraseEntry_ld88030();
    u8 pad_0x8[0x38];
};

void fn_7100d88030(base::global::CRntVector<UnkEraseEntry_ld88030>* self, const u32* index) {
    u32 idx = *index;
    if (self->count <= idx)
        return;
    UnkEraseEntry_ld88030* at = &self->data[idx];
    at->~UnkEraseEntry_ld88030();
    if (self->count - 1 > idx) {
        u64 i = idx;
        do {
            fn_71007b1928(&self->data[i], &self->data[i + 1]);
            self->data[i + 1].~UnkEraseEntry_ld88030();
            ++i;
        } while (i < self->count - 1);
    }
    self->count = self->count - 1;
}
