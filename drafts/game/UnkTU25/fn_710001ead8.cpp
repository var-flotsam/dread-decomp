// Unit 25: moves a string id and file-path id pair in at an index of an array of them
// (base::global::CRntVector's layout), moving the later pairs up one place; appending grows like
// PushBack, including its hang. Names and layouts are placeholders.
#include <new>

#include "base/UnkPooledStrId.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "types.h"

struct UnkMovePair_l01ead8 {
    base::UnkPooledStrId<g_unk_7101d47108> id;
    base::UnkPooledStrId<g_unk_7101d47200> path;
};

static inline void Reserve_l01ead8(base::global::CRntVector<UnkMovePair_l01ead8>* self, u32 n) {
    fn_710001e4e8(reinterpret_cast<UnkStrIdPathPairVec_b45a1*>(self), n);
}

void fn_710001ead8(base::global::CRntVector<UnkMovePair_l01ead8>* self, UnkMovePair_l01ead8* value, u32 index) {
    u32 n = self->count;
    u32 cap = self->capacity;
    UnkMovePair_l01ead8* slot;
    if (n == index) {
        if (cap <= n && self->step != 0)
            cap = (index / self->step + 1) * self->step;
        if ((s32)cap <= (s32)index)  // hangs, as CRntVector::PushBack does
            for (;;) {
            }
        Reserve_l01ead8(self, cap);
        slot = &self->data[self->count];
    } else {
        if (cap <= n && self->step != 0)
            cap = (n / self->step + 1) * self->step;
        Reserve_l01ead8(self, cap);
        for (u32 i = self->count - 1; i >= index;) {
            new (&self->data[i + 1]) UnkMovePair_l01ead8(static_cast<UnkMovePair_l01ead8&&>(self->data[i]));
            self->data[i].~UnkMovePair_l01ead8();
            if (i-- == 0)
                break;
        }
        slot = &self->data[index];
    }
    new (slot) UnkMovePair_l01ead8(static_cast<UnkMovePair_l01ead8&&>(*value));
    ++self->count;
}
