// Free of an array (base::global::CRntVector's layout) of file path ids paired with two words, as
// fn_71011df574 (src/game/GUI/CHUD/fn_71011df574.cpp) with the ids in CFilePathStrId's pool:
// fn_7100ffb8d4. Each id is released (and left as the empty id) before the array is freed.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkFilePathIdWordPair {
    UnkPathIdTwoStep id;
    u64 field_0x8;
    u64 field_0x10;
};

void fn_7100ffb8d4(base::global::CRntVector<UnkFilePathIdWordPair>* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkFilePathIdWordPair();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}
