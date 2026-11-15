// Unit 25: copy constructor of an array (base::global::CRntVector's layout) of string id and file-path id
// pairs: takes the other's settings, reserves its capacity and appends a copy of each pair with PushBack's
// growth. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "types.h"

struct UnkCopyPair_l01e3b4 {
    base::global::CStrId id;
    base::global::CFilePathStrId path;
};

struct UnkCopyPairVec_l01e3b4 {
    UnkCopyPair_l01e3b4* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Signature as src/base/UnkUnit25/UnkStrIdPathPairVecReserve_b45.cpp defines it.
struct UnkStrIdPathPairVec_b45a1;

static inline void Reserve_l01e3b4(UnkCopyPairVec_l01e3b4* self, u32 n) {
    fn_710001e4e8(reinterpret_cast<UnkStrIdPathPairVec_b45a1*>(self), n);
}

void fn_710001e3b4(UnkCopyPairVec_l01e3b4* self, const UnkCopyPairVec_l01e3b4* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    Reserve_l01e3b4(self, other->capacity);
    for (const UnkCopyPair_l01e3b4 *p = other->data, *e = p + other->count; p != e; ++p) {
        u32 n = self->count;
        u32 old_capacity = self->capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve_l01e3b4(self, new_capacity);
        new (&self->data[self->count]) UnkCopyPair_l01e3b4(*p);
        ++self->count;
    }
}
