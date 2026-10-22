// Resets and assignments of small holders of string ids: a two-state variant (state 1 holds a
// string id and a reference; state 2 holds nothing to destroy) reset to state 0, a counted
// array of ids stored after its count, and a move-assignment of an id with two values that
// destroys the old id first.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    UnkReleasedStrId(UnkReleasedStrId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
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

// Move-assignment of two values and an array of ids: the old ids are released (the storage is
// kept), the other's array is taken and left empty.
template <class T>
struct UnkMovedArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x1c;
    u32 flags;
    void* allocator;

    template <class Index = u64>
    __attribute__((always_inline)) void MoveFrom(UnkMovedArray* src) {
        for (Index i = 0; i < count; ++i)
            data[i].~T();
        count = 0;
        allocator = src->allocator;
        flags = src->flags;
        step = src->step;
        field_0x1c = src->field_0x1c;
        count = src->count;
        capacity = src->capacity;
        data = src->data;
        src->data = nullptr;
        src->count = 0;
        src->capacity = 0;
    }
};

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

// Move-assignments of arrays of arrays (each element frees its storage), and of arrays of
// pairs of arrays.
template <class T>
struct UnkFreedVector : base::UnkVector<T> {
    ~UnkFreedVector() { this->Free(); }
};

struct UnkFreedVectorPair {
    UnkFreedVector<u32> first;
    UnkFreedVector<u32> second;
    u8 unk_0x50[0x10];
};

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
