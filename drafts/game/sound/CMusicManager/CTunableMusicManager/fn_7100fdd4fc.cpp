// sound::CMusicManager::CTunableMusicManager's unit: copy-assignment of an array (base::global::CRntVector's
// layout) of {file path id, three u32} records: destroys its records, reserves the other's capacity and
// appends a copy of each with PushBack's growth. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkPathRecord_lfdd4fc {
    base::global::CFilePathStrId id;
    u32 values[3];  // 0x8

    UnkPathRecord_lfdd4fc(const UnkPathRecord_lfdd4fc& other) : id(other.id) {
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
    }
};

using UnkPathRecordVec_lfdd4fc = base::global::CRntVector<UnkPathRecord_lfdd4fc>;

// Releases a record's path id, leaving the empty id (the two-step release: the count is reread after the
// decrement).
static inline void ResetPath_lfdd4fc(base::global::CFilePathStrId& id) {
    using namespace base::global;
    if (UnkStringPool* pool = g_unk_7101d47200) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                fn_71000803a4(pool, id.data);
                pool = g_unk_7101d47200;
            }
        }
        id.data = &pool->empty;
    }
}

struct UnkPathVec_b0019;
void fn_7100456980(UnkPathVec_b0019* v, u32 n);

static inline void Reserve_lfdd4fc(UnkPathRecordVec_lfdd4fc* self, u32 n) {
    fn_7100456980(reinterpret_cast<UnkPathVec_b0019*>(self), n);
}

UnkPathRecordVec_lfdd4fc* fn_7100fdd4fc(UnkPathRecordVec_lfdd4fc* self, const UnkPathRecordVec_lfdd4fc* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            ResetPath_lfdd4fc(self->data[i].id);
        self->count = 0;
        Reserve_lfdd4fc(self, other->capacity);
        for (const UnkPathRecord_lfdd4fc *p = other->data, *e = p + other->count; p != e; ++p) {
            u32 n = self->count;
            u32 old_capacity = self->capacity;
            u32 new_capacity = old_capacity;
            if (old_capacity <= n && self->step != 0)
                new_capacity = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            Reserve_lfdd4fc(self, new_capacity);
            new (&self->data[self->count]) UnkPathRecord_lfdd4fc(*p);
            ++self->count;
        }
    }
    return self;
}
