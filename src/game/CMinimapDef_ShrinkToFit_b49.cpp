// Unit 2564 (CMinimapDef by reflection hint): shrinks keyed arrays of records. fn_7100e6fdf4: 0xc8-byte records (the record of
// src/game_aca57c/fn_7100e7b084.cpp: three string ids, a word, a string with 0x40 bytes of storage and
// plain values) to its count: the records are moved over and destroyed (fn_7100e6c960), the string-id
// keys copied and released, and the old arrays freed. fn_7100e71e04: 0x18-byte records (an id and four
// words) destroyed inline. Also the 0xc8-byte record's default constructor (fn_7100e6dfbc). Names and
// layouts are placeholders.
#include <new>

#include "../game_aca57c/UnkBufferString.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// As fn_7100e7b084.cpp's UnkMovedStrId: taken over, the moved-from id left empty.
struct UnkMovedStrId_b49 {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId_b49(UnkMovedStrId_b49&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

extern void* g_unk_7101a1b678[];
using UnkString40_b49 = UnkBufferString<0x40, g_unk_7101a1b678>;

// Copied as a block.
struct UnkWord_b49 {
    u32 value;

    __attribute__((always_inline)) UnkWord_b49(const UnkWord_b49& other) { __builtin_memcpy(this, &other, 4); }
};

// Copied as a 0x14-byte block.
struct UnkWords5_b49 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;

    __attribute__((always_inline)) UnkWords5_b49(const UnkWords5_b49& other) { __builtin_memcpy(this, &other, 0x14); }
};

struct UnkIdsString40_b49 {
    UnkMovedStrId_b49 field_0x0;
    u64 field_0x8;
    UnkMovedStrId_b49 field_0x10;
    UnkMovedStrId_b49 field_0x18;
    UnkString40_b49 field_0x20;
    UnkWord_b49 field_0xa8;
    UnkWord_b49 field_0xac;
    UnkWords5_b49 field_0xb0;
};
static_assert(sizeof(UnkIdsString40_b49) == 0xc8, "UnkIdsString40_b49 layout");

void fn_7100e6c960(char*);  // the record's destructor

// A 0x18-byte record: a string id taken over on a move (released in two steps when destroyed) and
// four words.
struct UnkIdWords4 {
    UnkMovedStrId_b49 id;
    UnkWord_b49 field_0x8;
    UnkWord_b49 field_0xc;
    UnkWord_b49 field_0x10;
    UnkWord_b49 field_0x14;

    UnkIdWords4(UnkIdWords4&& other) = default;
    ~UnkIdWords4() { reinterpret_cast<CStrId*>(&id)->Reset(); }
};
static_assert(sizeof(UnkIdWords4) == 0x18, "UnkIdWords4 layout");

template <class T>
struct UnkKeyedArray {
    T* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Moves the records into arrays of exactly `count` entries; Destroy runs on each moved-from record.
template <class T, void (*Destroy)(T*)>
__attribute__((always_inline)) static inline void ShrinkToFit(UnkKeyedArray<T>* self) {
    if (self->count == self->capacity)
        return;
    T* values = static_cast<T*>(self->allocator->vf_0x40((u64)self->count * sizeof(T), 8, self->flags));
    auto* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (T* src = self->values) {
        T* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) T(static_cast<T&&>(*src));
            Destroy(src);
        }
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&keys[j]) CStrId(*old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = self->count;
}

static inline void DestroyIdsString40(UnkIdsString40_b49* p) { fn_7100e6c960(reinterpret_cast<char*>(p)); }
static inline void DestroyIdWords4(UnkIdWords4* p) { p->~UnkIdWords4(); }

void fn_7100e6fdf4(UnkKeyedArray<UnkIdsString40_b49>* self) { ShrinkToFit<UnkIdsString40_b49, DestroyIdsString40>(self); }
void fn_7100e71e04(UnkKeyedArray<UnkIdWords4>* self) { ShrinkToFit<UnkIdWords4, DestroyIdWords4>(self); }

// The 0xc8-byte record's default constructor (fn_7100e6dfbc): the three ids empty, the string built
// on its storage, the words at +0xa8 and +0xac and the four at +0xb4 cleared; +0x8, +0xb0 and +0xc4
// are left as they are.
__attribute__((always_inline)) static inline void InitEmptyId(UnkMovedStrId_b49* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100e6dfbc(UnkIdsString40_b49* self) {
    InitEmptyId(&self->field_0x0);
    InitEmptyId(&self->field_0x10);
    InitEmptyId(&self->field_0x18);
    self->field_0x20.Init();
    self->field_0xa8.value = 0;
    self->field_0xac.value = 0;
    u32* words = reinterpret_cast<u32*>(&self->field_0xb0) + 1;
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
}

// A key as the Reserve moves it (as src/base/UnkResFactoryRegistry_Reserve_b49.cpp's
// UnkRegistryEntryId): the moved-from id is cleared through a union, then released in two steps.
union UnkKeyIdSlot_b49 {
    UnkStringData* p;
};

struct UnkKeyId_b49 {
    UnkStringData* data;

    UnkKeyId_b49(UnkKeyId_b49&& other) : data(other.data) {
        reinterpret_cast<UnkKeyIdSlot_b49*>(&other.data)->p = &g_unk_7101d47108->empty;
    }
    ~UnkKeyId_b49() {
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

// The Reserve of the keyed array of 0x18-byte records (fn_7100e71984): the records are moved over and
// destroyed inline, the string-id keys taken over (the moved-from ones released in two steps), and the
// old arrays freed.
void fn_7100e71984(UnkKeyedArray<UnkIdWords4>* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    auto* values = static_cast<UnkIdWords4*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkIdWords4), 8, self->flags));
    auto* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
    if (UnkIdWords4* src = self->values) {
        UnkIdWords4* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkIdWords4(static_cast<UnkIdWords4&&>(*src));
            src->~UnkIdWords4();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&keys[j]) UnkKeyId_b49(static_cast<UnkKeyId_b49&&>(*reinterpret_cast<UnkKeyId_b49*>(old)));
            reinterpret_cast<UnkKeyId_b49*>(old)->~UnkKeyId_b49();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = capacity;
}
