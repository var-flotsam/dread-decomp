// Clears of arrays of string ids (destroying the ids, keeping the storage), as fn_710104f600
// (src/game/CFXPool/CDefinition/fn_710104f600.cpp) does for ids with two values: ids in CFilePathStrId's pool
// (fn_71000d0378) or the default one (fn_71000cdbbc), and ids at +0x8 of their owner, destroyed
// only when the array has storage (fn_71000deb68, fn_7100251758). Also move-assignments of arrays
// of ids, as fn_710104ee34 for ids with two values (CFilePathStrId's pool fn_71000d0860, the
// default one fn_71000ce0a4). Resets of cached iterator values (base::UnkIterValue) that own a
// file path id and a string id: keyed by the string id (fn_71000c56ac, fn_71000c5b88,
// fn_71000da2e8, fn_71000da974), or holding both, after 16 plain bytes or not (fn_710001c1f4,
// fn_710001c720; fn_710001d3cc, fn_710001da74). Move-assignments of the latter (fn_710001d658)
// and of values holding a word, a file path id and a counted resource (fn_710000da34).
#include "base/UnkCollectionIter.h"
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
template <UnkStringPool** Pool, bool Reread = Pool == &g_unk_7101d47108>
struct UnkReleasedStrIdT {
    UnkStringData* data;

    // The pointer is stored bytewise (char alias info), so the destructor of the moved-from id
    // reloads the pool.
    UnkReleasedStrIdT(UnkReleasedStrIdT&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &(*Pool)->empty;
    }
    ~UnkReleasedStrIdT() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
    // Releases the id, then takes the other's (a string id re-reads the pool for the other's empty
    // string, a file path id usually keeps the one read for the release).
    UnkReleasedStrIdT& operator=(UnkReleasedStrIdT&& other) {
        UnkStringPool* pool = *Pool;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        if constexpr (Reread)
            other.data = &(*Pool)->empty;
        else
            other.data = &pool->empty;
        return *this;
    }
};

using UnkReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47108>;
// The same id in CFilePathStrId's pool.
using UnkFilePathReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47200>;

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
    u64 field_0x0;
    UnkClearedArray<UnkReleasedStrId> ids;
};

void fn_71000d0378(UnkClearedArray<UnkFilePathReleasedStrId>* self) { self->Clear(); }

void fn_71000cdbbc(UnkClearedArray<UnkReleasedStrId>* self) { self->Clear(); }

void fn_71000deb68(UnkIdArrayOwner* self) {
    UnkClearedArray<UnkReleasedStrId>* ids = &self->ids;
    if (ids->data) {
        for (u64 i = 0; i < ids->count; ++i)
            ids->data[i].~UnkReleasedStrId();
    }
    ids->count = 0;
}

void fn_7100251758(UnkIdArrayOwner* self) {
    UnkClearedArray<UnkReleasedStrId>* ids = &self->ids;
    if (ids->data) {
        for (u64 i = 0; i < ids->count; ++i)
            ids->data[i].~UnkReleasedStrId();
    }
    ids->count = 0;
}

// Move-assignment of an array: the old ids are released (the storage is kept), the other's array
// is taken and left empty.
template <class T>
struct UnkMovedArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void MoveFrom(UnkMovedArray* src) {
        for (u64 i = 0; i < count; ++i)
            data[i].~T();
        count = 0;
        allocator = src->allocator;
        flags = src->flags;
        step = src->step;
        field_0x14 = src->field_0x14;
        count = src->count;
        capacity = src->capacity;
        data = src->data;
        src->data = nullptr;
        src->count = 0;
        src->capacity = 0;
    }
};

using UnkFilePathIdArray = UnkMovedArray<UnkFilePathReleasedStrId>;
using UnkIdArray = UnkMovedArray<UnkReleasedStrId>;

UnkFilePathIdArray* fn_71000d0860(UnkFilePathIdArray* self, UnkFilePathIdArray* other) {
    if (self != other)
        self->MoveFrom(other);
    return self;
}

UnkIdArray* fn_71000ce0a4(UnkIdArray* self, UnkIdArray* other) {
    if (self != other)
        self->MoveFrom(other);
    return self;
}

using base::UnkIterValue;

struct UnkReleasedIdPair {
    UnkReleasedStrId id;
    UnkFilePathReleasedStrId path;
};

struct UnkReleasedIdPair10 {
    u64 field_0x0;
    u64 field_0x8;
    UnkReleasedStrId id;
    UnkFilePathReleasedStrId path;
};

using UnkPathByIdValue = UnkIterValue<UnkFilePathReleasedStrId, UnkReleasedStrId>;

__attribute__((flatten)) void fn_71000c56ac(UnkPathByIdValue* self) { self->Reset(); }

__attribute__((flatten)) void fn_71000c5b88(UnkPathByIdValue* self) { self->Reset(); }

__attribute__((flatten)) void fn_71000da2e8(UnkPathByIdValue* self) { self->Reset(); }

__attribute__((flatten)) void fn_71000da974(UnkPathByIdValue* self) { self->Reset(); }

// Assignment of the reference another iterator value holds (its slot 0x48): fn_71000c5778,
// fn_71000c5c54.
template <class T, class K>
__attribute__((always_inline)) static inline UnkIterValue<T, K>* AssignRef(UnkIterValue<T, K>* self,
                                                                         const UnkIterValue<T, K>* other) {
    if (self != other) {
        // base::UnkIterValue::SetRef, inlined.
        using Pair = typename UnkIterValue<T, K>::Value;
        typename UnkIterValue<T, K>::Ref r = other->ref;
        if (self->state == 2) {
            self->state = 0;
        } else if (self->state == 1) {
            self->value.~Pair();
            self->state = 0;
        }
        self->state = 2;
        self->ref.index = r.index;
        self->ref.element = r.element;
    }
    return self;
}

__attribute__((flatten)) UnkPathByIdValue* fn_71000c5778(UnkPathByIdValue* self, const UnkPathByIdValue* other) {
    return AssignRef(self, other);
}

__attribute__((flatten)) UnkPathByIdValue* fn_71000c5c54(UnkPathByIdValue* self, const UnkPathByIdValue* other) {
    return AssignRef(self, other);
}

__attribute__((flatten)) void fn_710001c1f4(UnkIterValue<UnkReleasedIdPair10>* self) { self->Reset(); }

__attribute__((flatten)) void fn_710001c720(UnkIterValue<UnkReleasedIdPair10>* self) { self->Reset(); }

__attribute__((flatten)) void fn_710001d3cc(UnkIterValue<UnkReleasedIdPair>* self) { self->Reset(); }

__attribute__((flatten)) void fn_710001da74(UnkIterValue<UnkReleasedIdPair>* self) { self->Reset(); }

// A counted resource (base/UnkCountedResource.h), referenced and assigned by pointer.
struct UnkAssignedRef54 {
    UnkCountedResource* ptr;

    UnkAssignedRef54& operator=(const UnkAssignedRef54& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkCountedResource* old = ptr) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            ptr = p;
        }
        return *this;
    }
    ~UnkAssignedRef54() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

// 8 plain bytes, a file path id and a counted resource (the resource is copied on a move).
struct UnkWordPathRes {
    u64 field_0x0;
    UnkFilePathReleasedStrId path;
    UnkAssignedRef54 shared;
};

// Move-assignment of the pair another value holds or refers to, without a self check.
template <class T, class K>
static inline UnkIterValue<T, K>* MoveAssign(UnkIterValue<T, K>* self, UnkIterValue<T, K>* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = static_cast<T&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = static_cast<T&&>(*other->ref.element);
    }
    return self;
}

__attribute__((flatten)) UnkIterValue<UnkReleasedIdPair>* fn_710001d658(UnkIterValue<UnkReleasedIdPair>* self,
                                                                        UnkIterValue<UnkReleasedIdPair>* other) {
    return MoveAssign(self, other);
}

__attribute__((flatten)) UnkIterValue<UnkWordPathRes>* fn_710000da34(UnkIterValue<UnkWordPathRes>* self,
                                                                     UnkIterValue<UnkWordPathRes>* other) {
    return MoveAssign(self, other);
}

// Move-assignment of an id and a file path id, as fn_71002db2a0 (src/game/animtree/CAnimTreeDef/fn_71002db2a0.cpp)
// does for two ids.
UnkReleasedIdPair* fn_710001d15c(UnkReleasedIdPair* self, UnkReleasedIdPair* other) {
    if (self != other)
        *self = static_cast<UnkReleasedIdPair&&>(*other);
    return self;
}

UnkReleasedIdPair* fn_710001cd80(UnkReleasedIdPair* self, UnkReleasedIdPair* other) {
    if (self != other)
        *self = static_cast<UnkReleasedIdPair&&>(*other);
    return self;
}

// Clear of a table of string ids (fn_71000d99d0) or file path ids (fn_71000db40c) keyed by string
// ids, as the tables' Clear in src/game/CActorComponent/fn_71002520fc.cpp does for records: the keys (at 8),
// then the values (at 0) are released, and the count is zeroed (the arrays are kept).
template <class Value>
struct UnkIdTable {
    Value* values;
    UnkReleasedStrId* keys;
    u32 count;

    __attribute__((always_inline)) void Clear() {
        if (keys && count) {
            for (u64 i = 0; i < count; ++i)
                keys[i].~UnkReleasedStrId();
        }
        if (values && count) {
            for (u64 i = 0; i < count; ++i)
                values[i].~Value();
        }
        count = 0;
    }
};

void fn_71000d99d0(UnkIdTable<UnkReleasedStrId>* self) { self->Clear(); }

void fn_71000db40c(UnkIdTable<UnkFilePathReleasedStrId>* self) { self->Clear(); }

// The same for a table of 0xb8-byte polymorphic values, destroyed through their vtable: fn_71000dd850.
struct UnkPolyValue_0xb8 {
    virtual ~UnkPolyValue_0xb8();
    u8 unk_0x8[0xb0];
};

void fn_71000dd850(UnkIdTable<UnkPolyValue_0xb8>* self) { self->Clear(); }

// The same for a table of counted resources paired with 8 plain bytes, whose value loop is
// written as a do-while (the count is already known to be nonzero): fn_71001d7444.
struct UnkResWord {
    UnkAssignedRef54 shared;
    u64 field_0x8;
};

void fn_71001d7444(UnkIdTable<UnkResWord>* self) {
    if (self->keys && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].~UnkReleasedStrId();
    }
    if (self->values && self->count) {
        u64 i = 0;
        do {
            self->values[i].~UnkResWord();
        } while (++i < self->count);
    }
    self->count = 0;
}

// Destructors of a file path id and a counted resource, after 8 plain bytes or not, as
// fn_710019e82c (src/game_lo/fn_710019e82c.cpp) does for a string id: fn_7100269170,
// fn_710000f514.
struct UnkPathRes {
    UnkFilePathReleasedStrId path;
    UnkAssignedRef54 shared;
};

void fn_7100269170(UnkPathRes* self) { self->~UnkPathRes(); }

void fn_710000f514(UnkWordPathRes* self) { self->~UnkWordPathRes(); }

// Its move-assignment, with a self check (fn_710000f674).
UnkWordPathRes* fn_710000f674(UnkWordPathRes* self, UnkWordPathRes* other) {
    if (self != other)
        *self = static_cast<UnkWordPathRes&&>(*other);
    return self;
}

// The move-assignment of iterator values over such pairs (as fn_710000da34 above), where the path
// id re-reads its pool for the other's empty string: fn_7100267684.
struct UnkRereadPathRes {
    UnkReleasedStrIdT<&g_unk_7101d47200, true> path;
    UnkAssignedRef54 shared;
};

__attribute__((flatten)) UnkIterValue<UnkRereadPathRes>* fn_7100267684(UnkIterValue<UnkRereadPathRes>* self,
                                                                       UnkIterValue<UnkRereadPathRes>* other) {
    return MoveAssign(self, other);
}

// End of a table of words keyed by string ids, as fn_7100aef43c (src/game_aca57c/fn_7100e4ee60.cpp)
// does for records: the iterator value is pointed one past the last key and value: fn_71000df938.
void fn_71000df938(const UnkIdTable<u32>* self, UnkIterValue<u32, UnkReleasedStrId>* out) {
    out->SetRef(self->keys + self->count, self->values + self->count);
}

// The same over 8-byte values: fn_7100252194.
void fn_7100252194(const UnkIdTable<u64>* self, UnkIterValue<u64, UnkReleasedStrId>* out) {
    out->SetRef(self->keys + self->count, self->values + self->count);
}

// Copy-assignment of a 0x38-byte record with a counted resource at 0x18 between plain fields
// (fn_71001ae8f0, fn_71001ae984).
struct UnkResRecord_71001ae8f0 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    UnkAssignedRef54 res;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
};

UnkResRecord_71001ae8f0* fn_71001ae8f0(UnkResRecord_71001ae8f0* self, const UnkResRecord_71001ae8f0* other) {
    if (self != other)
        *self = *other;
    return self;
}

UnkResRecord_71001ae8f0* fn_71001ae984(UnkResRecord_71001ae8f0* self, const UnkResRecord_71001ae8f0* other) {
    if (self != other)
        *self = *other;
    return self;
}
