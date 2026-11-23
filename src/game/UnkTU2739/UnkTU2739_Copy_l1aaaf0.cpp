// Unit 2739: copy-constructs an array of 16-byte records (a word and a file-path id): CRntVector's
// copy constructor written out (take the source's settings and allocator, reserve its capacity,
// then append each record). Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::UnkStringPool;

// A file path whose move leaves the source empty.
struct UnkPath_l1aaaf0 : base::global::UnkFilePathStrIdBase {
    UnkPath_l1aaaf0(const UnkPath_l1aaaf0& other) : UnkFilePathStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkPath_l1aaaf0(UnkPath_l1aaaf0&& other) : UnkFilePathStrIdBase(other) {
        other.data = &g_unk_7101d47200->empty;
    }
    ~UnkPath_l1aaaf0() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A word and a file path. The word is copied bytewise (char alias info), so a move reloads the
// file-path pool.
struct UnkPathEntry_l1aaaf0 {
    u8 field_0x0[4];
    UnkPath_l1aaaf0 path;
};

using UnkPathEntryVector_l1aaaf0 = base::global::CRntVector<UnkPathEntry_l1aaaf0>;

void fn_71011aaaf0(UnkPathEntryVector_l1aaaf0* self, const UnkPathEntryVector_l1aaaf0* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkPathEntry_l1aaaf0 *it = other->data, *end = it + other->count; it != end; ++it) {
        // Append, growing to the next multiple of the step when full.
        u32 n = self->count;
        u32 cap = self->capacity;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        if (cap < new_capacity) {
            self->Reserve(new_capacity);
            n = self->count;
        }
        new (&self->data[n]) UnkPathEntry_l1aaaf0(*it);
        ++self->count;
    }
}
