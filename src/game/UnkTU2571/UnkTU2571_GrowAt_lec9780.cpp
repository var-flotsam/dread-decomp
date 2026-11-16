// Unit 2571: the element at an index of an array of (word, string id) records, growing the array up
// to it with empty records (CRntVector::GrowAt written out). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The record's string id: new ones hold the pool's empty string, a move leaves it behind.
struct UnkTU2571Id_ec9780 {
    UnkStringData* data;

    UnkTU2571Id_ec9780() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    UnkTU2571Id_ec9780(UnkTU2571Id_ec9780&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkTU2571Id_ec9780() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// Two words (the music mode first) and a string id.
struct UnkTU2571Entry_ec9780 {
    u32 mode = 0;
    u32 field_0x4 = 0;
    UnkTU2571Id_ec9780 name;

    UnkTU2571Entry_ec9780() = default;
    UnkTU2571Entry_ec9780(UnkTU2571Entry_ec9780&& other) = default;
};

using UnkTU2571Entries_ec9780 = base::global::CRntVector<UnkTU2571Entry_ec9780>;

// Moves the records into a new array of `n` and frees the old one.
static inline void Reallocate(UnkTU2571Entries_ec9780* self, u32 n) {
    auto* p = static_cast<UnkTU2571Entry_ec9780*>(self->allocator->vf_0x40((u64)n * sizeof(UnkTU2571Entry_ec9780), 8, self->flags));
    if (self->data) {
        UnkTU2571Entry_ec9780* src = self->data;
        UnkTU2571Entry_ec9780* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkTU2571Entry_ec9780(static_cast<UnkTU2571Entry_ec9780&&>(*src));
            src->~UnkTU2571Entry_ec9780();
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = p;
    self->capacity = n;
}

// The music list of UnkTU2571_Music_ovb0166.cpp, which declares this function: its array of records
// starts at +0x0.
struct UnkMusicList_b0166;
struct UnkMusicEntry_b0166;

UnkMusicEntry_b0166* fn_7100ec9780(UnkMusicList_b0166* list, u32 index) {
    auto* self = reinterpret_cast<UnkTU2571Entries_ec9780*>(list);
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 records.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                Reallocate(self, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            Reallocate(self, n);
        // New records hold zero and the empty id.
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkTU2571Entry_ec9780();
        self->count = n;
    }
    return reinterpret_cast<UnkMusicEntry_b0166*>(&self->data[index]);
}
