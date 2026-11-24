// Unit 25: copy-assignment of an array (base::global::CRntVector's layout) of string id and file-path id
// pairs: unless it is the array itself, empties it, reserves the other's capacity and appends a copy of each
// pair with PushBack's growth. Names and layouts are placeholders.
#include <new>

#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "types.h"

struct UnkCopyPair_l01e744 {
    base::global::CStrId id;
    base::global::CFilePathStrId path;
};

using UnkCopyPairVec_l01e744 = base::global::CRntVector<UnkCopyPair_l01e744>;

// Signatures as src/game/UnkTU25/UnkTU25_PairVecTruncate_ovb0186.cpp and
// src/base/UnkUnit25/UnkStrIdPathPairVecReserve_b45.cpp define them.
void TruncateStrIdPathPairVector(base::global::CRntVector<UnkStrIdPathPair>* self, u32 count);
struct UnkStrIdPathPairVec_b45a1;

static inline void Reserve_l01e744(UnkCopyPairVec_l01e744* self, u32 n) {
    ReserveStrIdPairVectorCapacity(reinterpret_cast<UnkStrIdPathPairVec_b45a1*>(self), n);
}

UnkCopyPairVec_l01e744* fn_710001e744(UnkCopyPairVec_l01e744* self, const UnkCopyPairVec_l01e744* other) {
    if (self != other) {
        TruncateStrIdPathPairVector(reinterpret_cast<base::global::CRntVector<UnkStrIdPathPair>*>(self), 0);
        Reserve_l01e744(self, other->capacity);
        for (const UnkCopyPair_l01e744 *p = other->data, *e = p + other->count; p != e; ++p) {
            u32 n = self->count;
            u32 old_capacity = self->capacity;
            u32 new_capacity = old_capacity;
            if (old_capacity <= n && self->step != 0)
                new_capacity = self->GrownCapacity(n);
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            Reserve_l01e744(self, new_capacity);
            new (&self->data[self->count]) UnkCopyPair_l01e744(*p);
            ++self->count;
        }
    }
    return self;
}
