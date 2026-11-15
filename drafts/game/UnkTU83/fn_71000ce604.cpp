// Unit 83 (base::reflection): erases the string id at an iterator from a vector of ids, the later ids
// moved down one, and returns the iterator to the id that followed. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;

// Takes the id and leaves the empty one behind.
static inline void MoveConstruct(CStrId* dst, CStrId* src) {
    new (dst) UnkStrIdBase(*src);
    src->data = &g_unk_7101d47108->empty;
}

CStrId* fn_71000ce604(base::global::CRntVector<CStrId>* self, CStrId* it) {
    u32 index = ((u64)it - (u64)self->data) / sizeof(CStrId);
    if (index < self->count) {
        self->data[index].Reset();
        if (self->count - 1 > index) {
            u64 i = index;
            do {
                MoveConstruct(&self->data[i], &self->data[i + 1]);
                self->data[i + 1].Reset();
                ++i;
            } while (i < self->count - 1);
        }
        self->count = self->count - 1;
    }
    return self->data + index;
}
