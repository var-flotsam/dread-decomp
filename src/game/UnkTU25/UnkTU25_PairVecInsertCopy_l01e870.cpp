// Unit 25: copies a string id and file-path id pair in at an index of an array of them
// (base::global::CRntVector's layout), moving the later pairs up one place; appending grows like
// PushBack, including its hang. Names and layouts are placeholders.
#include <new>

#include "base/UnkPooledStrId.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "types.h"

struct UnkCopyPair_l01e870 {
    base::UnkPooledStrId<g_unk_7101d47108> id;
    base::UnkPooledStrId<g_unk_7101d47200> path;
};

struct UnkCopyPairVec_l01e870 {
    UnkCopyPair_l01e870* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

static inline void Reserve_l01e870(UnkCopyPairVec_l01e870* self, u32 n) {
    fn_710001e4e8(reinterpret_cast<UnkStrIdPathPairVec_b45a1*>(self), n);
}

void fn_710001e870(UnkCopyPairVec_l01e870* self, const UnkCopyPair_l01e870* value, u32 index) {
    if (index == self->count) {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)  // hangs, as CRntVector::PushBack does
            for (;;) {
            }
        Reserve_l01e870(self, new_capacity);
        new (&self->data[self->count]) UnkCopyPair_l01e870(*value);
    } else {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        Reserve_l01e870(self, new_capacity);
        for (u32 i = self->count - 1; i >= index;) {
            UnkCopyPair_l01e870* e = &self->data[i];
            new (&e[1]) UnkCopyPair_l01e870(static_cast<UnkCopyPair_l01e870&&>(*e));
            self->data[i].~UnkCopyPair_l01e870();
            if (i-- == 0)
                break;
        }
        new (&self->data[index]) UnkCopyPair_l01e870(*value);
    }
    ++self->count;
}
