// Unit 2569: an array of 32-byte entries (two string ids, a u64 and a u32): its reserve, copy-assign and the append of
// another array's entries to a record's array at +0x18; and the index of an id among nine globals.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A counted string id. `kSwapped` selects the operand order of its destructor's compare, which differs
// between the vector reallocation and the other users.
template <bool kSwapped>
struct UnkRawId_ovb0245 {
    UnkStringData* data;
    UnkRawId_ovb0245(const UnkRawId_ovb0245& o) : data(o.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkRawId_ovb0245(UnkRawId_ovb0245&& o) : data(o.data) { o.data = &g_unk_7101d47108->empty; }
    ~UnkRawId_ovb0245() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (kSwapped ? (data && &pool->empty != data) : (data && data != &pool->empty)) {
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

template <bool kSwapped>
struct UnkIdEntryT_ovb0245 {
    UnkRawId_ovb0245<kSwapped> a;
    UnkRawId_ovb0245<kSwapped> b;
    u64 c;
    u32 d;
};

using UnkIdEntry_ovb0245 = UnkIdEntryT_ovb0245<true>;
using UnkIdEntryMove_ovb0245 = UnkIdEntryT_ovb0245<false>;

using UnkIdVec_ovb0245 = base::UnkVector<UnkIdEntry_ovb0245>;

void fn_7100ec4fd4(UnkIdVec_ovb0245* v, u32 n);

// As src/game/UnkPlayReport.cpp declares it (its record type is the entry above).
struct UnkRecord_7101147868;
__attribute__((noinline, flatten)) void fn_7100ec51e8(base::UnkVector<UnkRecord_7101147868>* vec_, u32 n) {
    using Entry = UnkIdEntryMove_ovb0245;
    auto* vec = reinterpret_cast<UnkIdVec_ovb0245*>(vec_);
    auto* v = reinterpret_cast<base::UnkVector<Entry>*>(vec);
    if (v->capacity < n) {
        Entry* p = static_cast<Entry*>(v->allocator->vf_0x40((u64)n * sizeof(Entry), 8, v->flags));
        if (v->data) {
            Entry* src = v->data;
            Entry* dst = p;
            for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
                new (dst) Entry(static_cast<Entry&&>(*src));
                src->~Entry();
            }
            v->allocator->vf_0x48(v->data);
        }
        v->data = p;
        v->capacity = n;
    }
}

static inline void AppendCopy(UnkIdVec_ovb0245* v, const UnkIdEntry_ovb0245& value) {
    u32 n = v->count;
    u32 old_capacity = v->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_7100ec51e8(reinterpret_cast<base::UnkVector<UnkRecord_7101147868>*>(v), new_capacity);
    new (&v->data[v->count]) UnkIdEntry_ovb0245(value);
    ++v->count;
}

// As src/game/UnkPlayReport_Copy_b49.cpp declares it (its record type is the entry above). The original returns
// `self`.
struct UnkPlayReportRecord_b49;
base::UnkVector<UnkPlayReportRecord_b49>* fn_7100ec55ac(base::UnkVector<UnkPlayReportRecord_b49>* self_,
                                                         const base::UnkVector<UnkPlayReportRecord_b49>* other_) {
    auto* self = reinterpret_cast<UnkIdVec_ovb0245*>(self_);
    auto* other = reinterpret_cast<const UnkIdVec_ovb0245*>(other_);
    if (self != other) {
        fn_7100ec4fd4(self, 0);
        fn_7100ec51e8(reinterpret_cast<base::UnkVector<UnkRecord_7101147868>*>(self), other->capacity);
        for (UnkIdEntry_ovb0245 *it = other->data, *end = it + other->count; it != end; ++it)
            AppendCopy(self, *it);
    }
    return self_;
}

static inline void AppendByValue(UnkIdVec_ovb0245* v, UnkIdEntry_ovb0245 value) { AppendCopy(v, value); }

struct UnkIdOwner_ovb0245 {
    u32 kind;
    u8 pad_0x4[0x14];
    UnkIdVec_ovb0245 entries;  // 0x18
};

void fn_7100ec4ad8(void* unused, UnkIdOwner_ovb0245* owner, UnkIdVec_ovb0245* src) {
    for (UnkIdEntry_ovb0245 *it = src->data, *end = it + src->count; it != end; ++it) {
        UnkIdEntry_ovb0245 e(*it);
        AppendByValue(&owner->entries, e);
    }
}

#define UNK_ID_GLOBAL(n) extern u64 g_unk_71729ce##n __attribute__((visibility("hidden")))
UNK_ID_GLOBAL(0d0);
UNK_ID_GLOBAL(0d8);
UNK_ID_GLOBAL(0e0);
UNK_ID_GLOBAL(0e8);
UNK_ID_GLOBAL(0f0);
UNK_ID_GLOBAL(0f8);
UNK_ID_GLOBAL(100);
UNK_ID_GLOBAL(108);
UNK_ID_GLOBAL(110);

struct UnkIdHolder_ovb0245 {
    u64 id;
};

s64 fn_7100ec4ef8(void* self, UnkIdHolder_ovb0245* key) {
    u64 id = key->id;
    if (__builtin_expect(id == g_unk_71729ce0d0, 0))
        return 0;
    if (__builtin_expect(id == g_unk_71729ce0d8, 0))
        return 1;
    if (__builtin_expect(id == g_unk_71729ce0e0, 0))
        return 2;
    if (__builtin_expect(id == g_unk_71729ce0e8, 0))
        return 3;
    if (__builtin_expect(id == g_unk_71729ce0f0, 0))
        return 4;
    if (__builtin_expect(id == g_unk_71729ce0f8, 0))
        return 5;
    if (__builtin_expect(id == g_unk_71729ce100, 0))
        return 6;
    if (__builtin_expect(id == g_unk_71729ce108, 0))
        return 7;
    return id == g_unk_71729ce110 ? 8 : -1;
}
