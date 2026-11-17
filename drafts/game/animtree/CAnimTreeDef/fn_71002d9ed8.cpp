// animtree::CAnimTreeDef's unit (tools/tu_map.py unit 401): the reflected erase of an array of string-id
// pairs: the pair at the index is destroyed and the later ones move down by one. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdPair_l2d9ed8 {
    CStrId first;
    CStrId second;

    // Takes over the other's ids into storage that holds none; the other is left with empty ones.
    void MoveFrom(UnkIdPair_l2d9ed8& other) {
        CStrId::MoveConstructReloading(&first, other.first);
        CStrId::MoveConstructReloading(&second, other.second);
    }
};

struct UnkIdPairs_l2d9ed8 {
    UnkIdPair_l2d9ed8* data;
    u32 count;
};

// Destroys the pair at `*index` and moves the later ones down by one.
void fn_71002d9ed8(UnkIdPairs_l2d9ed8* self, const u32* index) {
    u32 at = *index;
    if (self->count > at) {
        self->data[at].~UnkIdPair_l2d9ed8();
        for (u64 i = at; i < self->count - 1; ++i) {
            self->data[i].MoveFrom(self->data[i + 1]);
            self->data[i + 1].~UnkIdPair_l2d9ed8();
        }
        self->count = self->count - 1;
    }
}
