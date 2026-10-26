// Resets and assignments of small holders of string ids: a two-state variant (state 1 holds a
// string id and a reference; state 2 holds nothing to destroy) reset to state 0, a counted
// array of ids stored after its count, and a move-assignment of an id with two values that
// destroys the old id first.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    // The pointer is stored bytewise (char alias info), so the destructor of the moved-from id
    // reloads the pool.
    UnkReleasedStrId(UnkReleasedStrId&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkReleasedStrId() {
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

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkRefPtr {
    UnkRefObject* ptr;

    ~UnkRefPtr() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkIdRef {
    UnkReleasedStrId id;
    UnkRefPtr ref;
};

struct UnkIdRefVariant {
    union {
        UnkIdRef id_ref;
    };
    s32 state;

    void Reset() {
        switch (state) {
        case 1:
            id_ref.~UnkIdRef();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
};

struct UnkVariantHolder {
    u64 field_0x0;
    UnkIdRefVariant variant;
};

void fn_7100d3d424(UnkVariantHolder* self) { self->variant.Reset(); }
void fn_7100d3d8ac(UnkVariantHolder* self) { self->variant.Reset(); }
void fn_7100d40ed8(UnkIdRefVariant* self) { self->Reset(); }
void fn_7100d414b0(UnkIdRefVariant* self) { self->Reset(); }

// A count followed directly (at +4, packed) by the ids.
#pragma pack(push, 4)
struct UnkCountedIds {
    u32 count;
    UnkReleasedStrId ids[1];
};
#pragma pack(pop)

void fn_7100de4928(UnkCountedIds* self) {
    for (u64 i = 0; i < self->count; ++i)
        self->ids[i].~UnkReleasedStrId();
    self->count = 0;
}

struct UnkIdValues {
    UnkReleasedStrId id;
    u32 field_0x8;
    u32 field_0xc;

    UnkIdValues(UnkIdValues&& other)
        : id(static_cast<UnkReleasedStrId&&>(other.id)), field_0x8(other.field_0x8), field_0xc(other.field_0xc) {}
};

UnkIdValues* fn_7100ead030(UnkIdValues* self, UnkIdValues* other) {
    if (self != other) {
        self->~UnkIdValues();
        new (self) UnkIdValues(static_cast<UnkIdValues&&>(*other));
    }
    return self;
}

struct UnkIdValues4 {
    UnkReleasedStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;

    UnkIdValues4(UnkIdValues4&& other)
        : id(static_cast<UnkReleasedStrId&&>(other.id)), field_0x8(other.field_0x8), field_0xc(other.field_0xc),
          field_0x10(other.field_0x10), field_0x14(other.field_0x14) {}
};

UnkIdValues4* fn_7100e6e704(UnkIdValues4* self, UnkIdValues4* other) {
    if (self != other) {
        self->~UnkIdValues4();
        new (self) UnkIdValues4(static_cast<UnkIdValues4&&>(*other));
    }
    return self;
}

// Copy-assignment of a polymorphic record: a value, a string id and 0x1c plain bytes (copied as
// one block).
struct UnkIdBlockRecord {
    void* vtable;
    u32 field_0x8;
    CStrId id;
    u8 data[0x1c];

    UnkIdBlockRecord& operator=(const UnkIdBlockRecord& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            id = other.id;
            __builtin_memcpy(data, other.data, sizeof(data));
        }
        return *this;
    }
};

UnkIdBlockRecord* fn_7100cd4af8(UnkIdBlockRecord* self, const UnkIdBlockRecord* other) { return &(*self = *other); }
UnkIdBlockRecord* fn_7100cd4bac(UnkIdBlockRecord* self, const UnkIdBlockRecord* other) { return &(*self = *other); }

// Clears (destroying the elements, keeping the storage) of arrays of ids (at 0xd642f0 of an
// owner) and of ids with two values.
template <class T>
struct UnkClearedArray {
    T* data;
    u32 count;

    void Clear() {
        for (u64 i = 0; i < count; ++i)
            data[i].~T();
        count = 0;
    }
};

struct UnkIdArrayOwner {
    u8 unk_0x0[0xd642f0];
    UnkClearedArray<UnkReleasedStrId> ids;
};

void fn_7100e9b5d0(UnkIdArrayOwner* self) { self->ids.Clear(); }
void fn_7100eaf4e0(UnkClearedArray<UnkIdValues>* self) { self->Clear(); }

struct UnkIdPairValue {
    UnkReleasedStrId first;
    UnkReleasedStrId second;
    u32 field_0x10;
};

// Releases the old id, then takes the other's (leaving it empty).
__attribute__((always_inline)) static inline void MoveAssign(UnkReleasedStrId* self, UnkReleasedStrId* other) {
    self->~UnkReleasedStrId();
    self->data = other->data;
    other->data = &g_unk_7101d47108->empty;
}

UnkIdPairValue* fn_7100c8f970(UnkIdPairValue* self, UnkIdPairValue* other) {
    if (self != other) {
        MoveAssign(&self->first, &other->first);
        MoveAssign(&self->second, &other->second);
        self->field_0x10 = other->field_0x10;
    }
    return self;
}

// Move-assignment of six values, an id and a half-word.
struct UnkValuesIdRecord {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    UnkReleasedStrId id;
    u16 field_0x20;
};

UnkValuesIdRecord* fn_7100e4f980(UnkValuesIdRecord* self, UnkValuesIdRecord* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        self->field_0x4 = other->field_0x4;
        self->field_0x8 = other->field_0x8;
        self->field_0xc = other->field_0xc;
        self->field_0x10 = other->field_0x10;
        self->field_0x14 = other->field_0x14;
        MoveAssign(&self->id, &other->id);
        self->field_0x20 = other->field_0x20;
    }
    return self;
}

struct UnkValuesIdArrayRecord {
    u32 field_0x0;
    u32 field_0x4;
    UnkMovedArray<UnkReleasedStrId> ids;
};

UnkValuesIdArrayRecord* fn_7100e504b4(UnkValuesIdArrayRecord* self, UnkValuesIdArrayRecord* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        self->field_0x4 = other->field_0x4;
        self->ids.MoveFrom(&other->ids);
    }
    return self;
}

UnkMovedArray<UnkIdValues>* fn_7100eaf960(UnkMovedArray<UnkIdValues>* self, UnkMovedArray<UnkIdValues>* other) {
    if (self != other)
        self->MoveFrom(other);
    return self;
}

// Copy-assignment of two ids and a value.
struct UnkIdPairCopy {
    CStrId first;
    CStrId second;
    u32 field_0x10;
};

UnkIdPairCopy* fn_7100c8f87c(UnkIdPairCopy* self, const UnkIdPairCopy* other) {
    if (self != other) {
        self->first = other->first;
        self->second = other->second;
        self->field_0x10 = other->field_0x10;
    }
    return self;
}

// Move-assignment of a value, an id and an array of ids.
struct UnkValueIdIdArrayRecord {
    u32 field_0x0;
    UnkReleasedStrId id;
    UnkMovedArray<UnkReleasedStrId> ids;
};

UnkValueIdIdArrayRecord* fn_7100e077d0(UnkValueIdIdArrayRecord* self, UnkValueIdIdArrayRecord* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        MoveAssign(&self->id, &other->id);
        self->ids.MoveFrom(&other->ids);
    }
    return self;
}

// A reference whose object's count is a union member (no type-based alias info: the count of
// the array is reloaded after each release).
struct UnkUnionRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};

struct UnkUnionRefPtr {
    UnkUnionRefObject* ptr;

    ~UnkUnionRefPtr() {
        if (UnkUnionRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

UnkMovedArray<UnkUnionRefPtr>* fn_7100d40474(UnkMovedArray<UnkUnionRefPtr>* self, UnkMovedArray<UnkUnionRefPtr>* other) {
    if (self != other)
        self->MoveFrom<u32>(other);
    return self;
}
UnkMovedArray<UnkUnionRefPtr>* fn_7100d844e0(UnkMovedArray<UnkUnionRefPtr>* self, UnkMovedArray<UnkUnionRefPtr>* other) {
    if (self != other)
        self->MoveFrom<u32>(other);
    return self;
}

UnkMovedArray<UnkFreedVector<u32>>* fn_7100d43c6c(UnkMovedArray<UnkFreedVector<u32>>* self,
                                                   UnkMovedArray<UnkFreedVector<u32>>* other) {
    if (self != other)
        self->MoveFrom(other);
    return self;
}

UnkMovedArray<UnkFreedVectorPair>* fn_7100e52a58(UnkMovedArray<UnkFreedVectorPair>* self,
                                                  UnkMovedArray<UnkFreedVectorPair>* other) {
    if (self != other)
        self->MoveFrom<u32>(other);
    return self;
}

// Move-assignment of an array of records holding a value, an id and an array of ids (which
// frees its storage).
struct UnkFreedIdArray {
    UnkReleasedStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkFreedIdArray() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkReleasedStrId();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkIdArrayRecord {
    u64 field_0x0;
    UnkReleasedStrId id;
    UnkFreedIdArray ids;
};

UnkMovedArray<UnkIdArrayRecord>* fn_7100b5c0d0(UnkMovedArray<UnkIdArrayRecord>* self, UnkMovedArray<UnkIdArrayRecord>* other) {
    if (self != other)
        self->MoveFrom<u32>(other);
    return self;
}

// Move-assignment of an id, four values and a large plain block.
struct UnkIdValuesBlock {
    UnkReleasedStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u64 unk_0x18;
    u8 data[0x5f54];
};

UnkIdValuesBlock* fn_7100e7c638(UnkIdValuesBlock* self, UnkIdValuesBlock* other) {
    if (self != other) {
        MoveAssign(&self->id, &other->id);
        self->field_0x8 = other->field_0x8;
        self->field_0xc = other->field_0xc;
        self->field_0x10 = other->field_0x10;
        self->field_0x14 = other->field_0x14;
        __builtin_memcpy(self->data, other->data, sizeof(self->data));
    }
    return self;
}

// Move-assignment of an array with a second pointer (destroying the elements through
// fn_7100ea7034, keeping the storage).
struct UnkRecord118 {
    u64 unk_0x0[0x23];

    ~UnkRecord118();
};
void fn_7100ea7034(UnkRecord118* self);
inline UnkRecord118::~UnkRecord118() { fn_7100ea7034(this); }

template <class T, class Index>
static inline UnkSecondPtrArray<T>* MoveAssign(UnkSecondPtrArray<T>* self, UnkSecondPtrArray<T>* other) {
    if (self != other) {
        if (self->data)
            for (Index i = 0; i < self->count; ++i)
                self->data[i].~T();
        self->count = 0;
        self->allocator = other->allocator;
        self->field_0x8 = other->field_0x8;
        self->data = other->data;
        self->capacity = other->capacity;
        self->count = other->count;
        self->field_0x1c = other->field_0x1c;
        self->field_0x18 = other->field_0x18;
        other->count = 0;
        other->data = nullptr;
        other->field_0x8 = nullptr;
    }
    return self;
}

UnkSecondPtrArray<UnkRecord118>* fn_7100eae08c(UnkSecondPtrArray<UnkRecord118>* self, UnkSecondPtrArray<UnkRecord118>* other) {
    return MoveAssign<UnkRecord118, u32>(self, other);
}
UnkSecondPtrArray<UnkFreedVectorPair>* fn_7100e54838(UnkSecondPtrArray<UnkFreedVectorPair>* self,
                                                      UnkSecondPtrArray<UnkFreedVectorPair>* other) {
    return MoveAssign<UnkFreedVectorPair, u32>(self, other);
}

// Copy-assignment of an array of array pairs through its out-of-line Reserve (fn_7100e52b58) and
// PushBack.
void fn_7100e52cf4(UnkPairArray* self, const UnkFreedVectorPair* value);  // PushBack

// The array's Reserve: moves the pairs into a new array.
__attribute__((noinline)) void fn_7100e52b58(UnkPairArray* self, u32 capacity) {
    self->Reserve(capacity);
}

UnkPairArray* fn_7100e52e64(UnkPairArray* self, const UnkPairArray* other) {
    if (self != other) {
        for (u32 i = 0; i < self->count; ++i)
            self->data[i].~UnkFreedVectorPair();
        self->count = 0;
        fn_7100e52b58(self, other->capacity);
        for (const UnkFreedVectorPair *it = other->data, *end = it + other->count; it != end; ++it)
            fn_7100e52cf4(self, it);
    }
    return self;
}

void fn_7100e493f4(UnkSecondPtrArray<UnkFreedVectorPair>* self) { self->Destroy(); }

// Copy-assignments of maps of ids to values (values at 0, keys at 8, sharing one count): both
// sides are destroyed, the storage is reallocated when the other's count differs from the
// capacity, then the values are assigned and the keys copied, counting a use of each.
template <class V>
struct UnkIdMap {
    V* values;
    UnkReleasedStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Grows both arrays, moving the values (destroying the old ones with Destroy) and the keys.
template <class V, void (*Destroy)(V*)>
static inline void Reserve(UnkIdMap<V>* self, u32 capacity) {
    if (self->capacity < capacity) {
        V* values = static_cast<V*>(self->allocator->vf_0x40((u64)capacity * sizeof(V), 8, self->flags));
        auto* keys = static_cast<UnkReleasedStrId*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkReleasedStrId), 8, self->flags));
        if (V* src = self->values) {
            V* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) V(static_cast<V&&>(*src));
                Destroy(src);
            }
            self->allocator->vf_0x48(self->values);
        }
        if (UnkReleasedStrId* src = self->keys) {
            UnkReleasedStrId* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkReleasedStrId(static_cast<UnkReleasedStrId&&>(*src));
                src->~UnkReleasedStrId();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}

// Shrinks both arrays to the count, moving the values (destroying the old ones with Destroy) and
// copying the keys (counting a use) before releasing the old ones.
template <class V, void (*Destroy)(V*)>
static inline void ShrinkToFit(UnkIdMap<V>* self) {
    if (self->count == self->capacity)
        return;
    V* values = static_cast<V*>(self->allocator->vf_0x40((u64)self->count * sizeof(V), 8, self->flags));
    auto* keys = static_cast<UnkReleasedStrId*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkReleasedStrId), 8, self->flags));
    if (V* src = self->values) {
        V* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) V(static_cast<V&&>(*src));
            Destroy(src);
        }
        self->allocator->vf_0x48(self->values);
    }
    if (UnkReleasedStrId* src = self->keys) {
        for (u32 i = 0; i < self->count; ++i, ++src) {
            UnkStringData* data = src->data;
            __builtin_memcpy(&keys[i], &data, sizeof(data));
            if (&g_unk_7101d47108->empty != data)
                ++data->ref_count;
            src->~UnkReleasedStrId();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = self->count;
}

template <class V, void (*Destroy)(V*), void (*Free)(UnkIdMap<V>*), void (*Allocate)(UnkIdMap<V>*, u32),
          V* (*AssignValue)(V*, const V*)>
static inline UnkIdMap<V>* Assign(UnkIdMap<V>* self, const UnkIdMap<V>* other) {
    if (self != other) {
        if (self->keys)
            for (u64 i = 0; i < self->count; ++i)
                self->keys[i].~UnkReleasedStrId();
        if (self->values)
            for (u32 i = 0; i < self->count; ++i)
                Destroy(&self->values[i]);
        self->count = 0;
        if (other->count != self->capacity) {
            Free(self);
            Allocate(self, other->count);
        }
        self->count = other->count;
        for (u32 i = 0; i < self->count; ++i)
            AssignValue(&self->values[i], &other->values[i]);
        self->count = other->count;
        for (u32 i = 0; i < self->count; ++i) {
            UnkStringData* data = other->keys[i].data;
            __builtin_memcpy(&self->keys[i], &data, sizeof(data));
            if (&g_unk_7101d47108->empty != data)
                ++data->ref_count;
        }
    }
    return self;
}

// Ids to arrays of array pairs.
using UnkPairArrayValue = UnkSecondPtrArray<UnkFreedVectorPair>;
using UnkIdPairArrayMap = UnkIdMap<UnkPairArrayValue>;
UnkPairArrayValue* fn_7100e5493c(UnkPairArrayValue* self, const UnkPairArrayValue* other);

__attribute__((noinline)) void fn_7100e5854c(UnkIdPairArrayMap* self, u32 capacity);
__attribute__((noinline)) void fn_7100e588d0(UnkIdPairArrayMap* self);

// NON_MATCHING(minor): in the key loop the original steps the old key with its own pointer;
// ours addresses it from the shared index.
void fn_7100e588d0(UnkIdPairArrayMap* self) { ShrinkToFit<UnkPairArrayValue, fn_7100e493f4>(self); }

void fn_7100e5854c(UnkIdPairArrayMap* self, u32 capacity) {
    Reserve<UnkPairArrayValue, fn_7100e493f4>(self, capacity);
}

UnkIdPairArrayMap* fn_7100e5870c(UnkIdPairArrayMap* self, const UnkIdPairArrayMap* other) {
    return Assign<UnkPairArrayValue, fn_7100e493f4, fn_7100e588d0, fn_7100e5854c, fn_7100e5493c>(self, other);
}

// Ids to 0x60-byte records: plain words, two string ids and an array of string ids (destroyed by
// fn_7100e4a1d0 and copied by fn_7100e4fd1c, both in fn_7100e4fd1c.cpp).
struct UnkRecord60 {
    u32 field_0x0[10];
    UnkStringData* id_0x28;
    UnkStringData* id_0x30;
    UnkStringData** ids;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x4c;
    u32 flags;
    base::UnkAllocator* allocator;

    // Takes over the ids (leaving the pool's empty string) and the array (leaving it empty).
    __attribute__((always_inline)) UnkRecord60(UnkRecord60&& other) {
        for (u32 i = 0; i < 10; ++i)
            field_0x0[i] = other.field_0x0[i];
        id_0x28 = other.id_0x28;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        other.id_0x28 = empty;
        id_0x30 = other.id_0x30;
        other.id_0x30 = empty;
        ids = other.ids;
        count = other.count;
        capacity = other.capacity;
        step = other.step;
        field_0x4c = other.field_0x4c;
        flags = other.flags;
        allocator = other.allocator;
        other.ids = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};
static_assert(sizeof(UnkRecord60) == 0x60, "UnkRecord60 layout");
using UnkIdRecordMap = UnkIdMap<UnkRecord60>;
void fn_7100e4a1d0(UnkRecord60* record);                // destructor
void fn_7100e5a114(UnkIdRecordMap* self);              // frees the storage
UnkRecord60* fn_7100e4fd1c(UnkRecord60* self, const UnkRecord60* other);
__attribute__((noinline)) void fn_7100e59d18(UnkIdRecordMap* self, u32 capacity);

void fn_7100e59d18(UnkIdRecordMap* self, u32 capacity) { Reserve<UnkRecord60, fn_7100e4a1d0>(self, capacity); }

UnkIdRecordMap* fn_7100e59f50(UnkIdRecordMap* self, const UnkIdRecordMap* other) {
    return Assign<UnkRecord60, fn_7100e4a1d0, fn_7100e5a114, fn_7100e59d18, fn_7100e4fd1c>(self, other);
}
