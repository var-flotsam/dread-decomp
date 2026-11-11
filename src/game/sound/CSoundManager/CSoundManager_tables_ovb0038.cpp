// sound::CSoundManager unit (tools/tu_map.py, unit 2646): the sound manager's tables and the objects
// they own. A table of two parallel arrays sharing one count (0x18-byte records of a string id, a
// file-path id and their counted resource, released by fn_7100026f70, and string ids) with its growth,
// shrink, copy, append and swap-erase; an object owning one (destroyed by fn_710102796c) and an array of
// owning pointers to such objects; iterators and value pairs over them; the sound events definition's
// constructors and creation; the preset lookups of the sound manager's playback; and reading a reflected
// resource from its file. Names and layouts are placeholders.
#include <new>

#include "../../CMinimapData/UnkStrIdIterBeginEnd_A6_b75.h"
#include "base/UnkCountedResource.h"
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/core/CAsset.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU101.h"
#include "game/UnkTU25.h"
#include "game/UnkTU32.h"
#include "game/sound/CSoundManager.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkIdPair;
void fn_7100026f70(UnkIdPair* pair);

// CFilePathStrId's copy constructor.
struct UnkSoundPathId_b0038 {
    base::global::UnkStringData* data;
    UnkSoundPathId_b0038(const UnkSoundPathId_b0038& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
};

// A 0x18-byte record of a string id, a file-path id and their counted resource, as its copy
// constructor sees it (fn_7100026f70 destroys it).
struct UnkSoundIdResource_b0038 {
    CStrId id;
    UnkSoundPathId_b0038 path;
    UnkCountedResource* resource;

    UnkSoundIdResource_b0038(const UnkSoundIdResource_b0038& other)
        : id(other.id), path(other.path), resource(nullptr) {
        if (UnkCountedResource* r = other.resource) {
            ++r->ref_count;
            if (UnkCountedResource* old = resource) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            resource = r;
        }
    }
};

// Two parallel arrays sharing one count: the records and their string ids.
struct UnkSoundIdTable_b0038 {
    UnkSoundIdResource_b0038* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Moves both arrays into ones of exactly `count` entries (the records copied, then destroyed).
void fn_710102a344(UnkSoundIdTable_b0038* self) {
    if (self->count == self->capacity)
        return;
    auto* records = static_cast<UnkSoundIdResource_b0038*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkSoundIdResource_b0038), 8, self->flags));
    auto* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (UnkSoundIdResource_b0038* src = self->records) {
        UnkSoundIdResource_b0038* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkSoundIdResource_b0038(*src);
            fn_7100026f70(reinterpret_cast<UnkIdPair*>(src));
        }
        self->allocator->vf_0x48(self->records);
    }
    if (CStrId* old = self->ids) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&ids[j]) CStrId(*old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->records = records;
    self->ids = ids;
    self->capacity = self->count;
}

// A key as the growth moves it (as src/game/CMinimapDef_ShrinkToFit_b49.cpp's UnkKeyId_b49): the
// moved-from id is cleared through a union, then released in two steps.
union UnkSoundKeyIdSlot_b0038 {
    base::global::UnkStringData* p;
};

struct UnkSoundKeyId_b0038 {
    base::global::UnkStringData* data;

    UnkSoundKeyId_b0038(UnkSoundKeyId_b0038&& other) : data(other.data) {
        reinterpret_cast<UnkSoundKeyIdSlot_b0038*>(&other.data)->p = &g_unk_7101d47108->empty;
    }
    ~UnkSoundKeyId_b0038() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
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

// Grows both arrays to `capacity` entries (the records copied, then destroyed).
void fn_7101029ec8(UnkSoundIdTable_b0038* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    auto* records = static_cast<UnkSoundIdResource_b0038*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkSoundIdResource_b0038), 8, self->flags));
    auto* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
    if (UnkSoundIdResource_b0038* src = self->records) {
        UnkSoundIdResource_b0038* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkSoundIdResource_b0038(*src);
            fn_7100026f70(reinterpret_cast<UnkIdPair*>(src));
        }
        self->allocator->vf_0x48(self->records);
    }
    if (CStrId* old = self->ids) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&ids[j]) UnkSoundKeyId_b0038(static_cast<UnkSoundKeyId_b0038&&>(*reinterpret_cast<UnkSoundKeyId_b0038*>(old)));
            reinterpret_cast<UnkSoundKeyId_b0038*>(old)->~UnkSoundKeyId_b0038();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->records = records;
    self->ids = ids;
    self->capacity = capacity;
}

// CStrId's copy constructor, with the pointer copied untyped (the count is reloaded after it).
__attribute__((always_inline)) static inline void CopyId(CStrId* dst, const CStrId* src) {
    base::global::UnkStringData* data = src->data;
    __builtin_memcpy(&dst->data, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

// Appends a key and a copy of a record (growing by the step when full); returns the new record.
__attribute__((always_inline)) static inline UnkSoundIdResource_b0038* Add(UnkSoundIdTable_b0038* self, const CStrId* key,
                                            const UnkSoundIdResource_b0038* record) {
    if (self->count == self->capacity)
        fn_7101029ec8(self, self->count + self->step);
    CopyId(&self->ids[self->count], key);
    new (&self->records[self->count]) UnkSoundIdResource_b0038(*record);
    u32 n = ++self->count;
    return n ? self->records + n - 1 : self->records;
}

UnkSoundIdResource_b0038* fn_710102a578(UnkSoundIdTable_b0038* self, const CStrId* key,
                                        const UnkSoundIdResource_b0038* record) {
    return Add(self, key, record);
}

UnkSoundIdResource_b0038* fn_710102a6bc(UnkSoundIdTable_b0038* self, const CStrId* key,
                                        const UnkSoundIdResource_b0038* record) {
    return Add(self, key, record);
}

// The copy constructor: the growth parameters copied, room for the other's count reserved
// (fn_7101029ec8), then the records and the ids copied.
void fn_7101029d40(UnkSoundIdTable_b0038* self, const UnkSoundIdTable_b0038* other) {
    self->records = nullptr;
    self->ids = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7101029ec8(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkSoundIdResource_b0038* dst = &self->records[i];
        const UnkSoundIdResource_b0038* src = &other->records[i];
        new (&dst->id) CStrId(src->id);
        dst->path.data = src->path.data;
        if (&g_unk_7101d47200->empty != dst->path.data)
            ++dst->path.data->ref_count;
        dst->resource = nullptr;
        if (UnkCountedResource* r = src->resource) {
            ++r->ref_count;
            if (UnkCountedResource* old = dst->resource) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            dst->resource = r;
        }
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyId(&self->ids[i], &other->ids[i]);
}

// A string id taken over, the moved-from one left empty.
struct UnkSoundMovedStrId_b0038 {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkSoundMovedStrId_b0038(UnkSoundMovedStrId_b0038&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

// A pair of values held in place (state 1) or by reference (state 2).
template <class X, class Y>
struct UnkSoundPairRef_b0038 {
    union {
        struct {
            X x;
            Y y;
        } values;
        struct {
            X* x;
            Y* y;
        } refs;
    };
    s32 state;

    X& GetX() { return state == 2 ? *refs.x : values.x; }
    Y& GetY() { return state == 2 ? *refs.y : values.y; }
};

// Move construction that always holds the values in place: from in-place values (state 1) or
// from the referents (state 2).
template <class X, class Y>
__attribute__((always_inline)) static inline void Emplace(UnkSoundPairRef_b0038<X, Y>* self, X& x, Y& y) {
    self->state = 1;
    new (&self->values.x) X(static_cast<X&&>(x));
    new (&self->values.y) Y(static_cast<Y&&>(y));
}

template <class X, class Y>
__attribute__((always_inline)) static inline void MoveConstruct(UnkSoundPairRef_b0038<X, Y>* self,
                                                                UnkSoundPairRef_b0038<X, Y>* other) {
    self->state = 0;
    switch (other->state) {
    case 1:
        Emplace(self, other->values.x, other->values.y);
        break;
    case 2: {
        self->state = 1;
        X* x = other->refs.x;
        Y* y = other->refs.y;
        new (&self->values.x) X(static_cast<X&&>(*x));
        new (&self->values.y) Y(static_cast<Y&&>(*y));
        break;
    }
    }
}

using UnkSoundIdResourcePair_b0038 = UnkSoundPairRef_b0038<UnkSoundMovedStrId_b0038, UnkSoundIdResource_b0038>;

void fn_710102918c(UnkSoundIdResourcePair_b0038* self, UnkSoundIdResourcePair_b0038* other) {
    MoveConstruct(self, other);
}
void fn_71010295f0(UnkSoundIdResourcePair_b0038* self, UnkSoundIdResourcePair_b0038* other) {
    MoveConstruct(self, other);
}

// The ids as the moves see them.
static inline UnkSoundKeyId_b0038* KeyIds(UnkSoundIdTable_b0038* self) {
    return reinterpret_cast<UnkSoundKeyId_b0038*>(self->ids);
}

// Removes the record at `pos` and its id: the last record and id are moved into its place.
// Returns the record now at that place.
UnkSoundIdResource_b0038* fn_710102a800(UnkSoundIdTable_b0038* self, UnkSoundIdResource_b0038* pos) {
    u32 i = pos - self->records;
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->records[i]));
    self->ids[i].Reset();
    if (i != self->count - 1) {
        new (&self->records[i]) UnkSoundIdResource_b0038(self->records[self->count - 1]);
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->records[self->count - 1]));
        new (&KeyIds(self)[i]) UnkSoundKeyId_b0038(static_cast<UnkSoundKeyId_b0038&&>(KeyIds(self)[self->count - 1]));
        KeyIds(self)[self->count - 1].~UnkSoundKeyId_b0038();
    }
    --self->count;
    return &self->records[i];
}

// An object owning a table, with an owned polymorphic object first: destroyed by fn_710102796c.
struct UnkSoundOwnerBase_b0038 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // deleting destructor
};

struct UnkSoundOwned_b0038 {
    UnkSoundOwnerBase_b0038* owner;
    UnkSoundIdTable_b0038 table;
};

void fn_710102796c(UnkSoundOwned_b0038* self) {
    UnkSoundIdTable_b0038* t = &self->table;
    if (t->ids && t->count) {
        for (u64 i = 0; i < t->count; ++i)
            t->ids[i].Reset();
    }
    if (t->records && t->count) {
        for (u64 i = 0; i < t->count; ++i)
            fn_7100026f70(reinterpret_cast<UnkIdPair*>(&t->records[i]));
    }
    t->count = 0;
    if (t->ids)
        t->allocator->vf_0x48(t->ids);
    if (t->records)
        t->allocator->vf_0x48(t->records);
    UnkSoundOwnerBase_b0038* p = self->owner;
    self->owner = nullptr;
    if (p)
        p->vf_0x10();
}

// An owning pointer: moved by taking the other's object (an assignment destroys the old one);
// cleared, then its object destroyed (fn_710102796c) and freed.
struct UnkSoundOwnedPtr_b0038 {
    union {
        UnkSoundOwned_b0038* ptr;
    };

    UnkSoundOwnedPtr_b0038(UnkSoundOwnedPtr_b0038&& other) {
        UnkSoundOwned_b0038* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    UnkSoundOwnedPtr_b0038& operator=(UnkSoundOwnedPtr_b0038&& other) {
        UnkSoundOwned_b0038* p = other.ptr;
        other.ptr = nullptr;
        UnkSoundOwned_b0038* old = ptr;
        ptr = p;
        if (old) {
            fn_710102796c(old);
            fn_7100083c00(old);
        }
        return *this;
    }
    ~UnkSoundOwnedPtr_b0038() {
        UnkSoundOwned_b0038* p = ptr;
        ptr = nullptr;
        if (p) {
            fn_710102796c(p);
            fn_7100083c00(p);
        }
    }
};

using UnkSoundOwnedArray_b0038 = base::UnkVector<UnkSoundOwnedPtr_b0038>;

// The array of owning pointers: its constructor with room for `capacity`, and its Free.
void fn_7101028988(UnkSoundOwnedArray_b0038* self, u32 capacity, u32 flags) {
    new (self) UnkSoundOwnedArray_b0038(flags);
    self->Reserve(capacity);
}

// sound::CSoundEventsDef (its constructor is the reflected class's fn_0x28): the owning pointers at
// +0x58 and a table at +0x80.
struct UnkSoundOwnerObject_b0038 {
    void** vtable;
    u8 unk_0x8[0x50];
    UnkSoundOwnedArray_b0038 objects;
    UnkSoundIdTable_b0038 table;
};
static_assert(__builtin_offsetof(UnkSoundOwnerObject_b0038, table) == 0x80, "");

__attribute__((always_inline)) static inline void Construct(UnkSoundOwnerObject_b0038* self) {
    fn_7100016ad8(self);
    self->vtable = g_unk_7101cac688 + 2;
    fn_7101028988(&self->objects, 0x40, 0xb);
    self->table.records = nullptr;
    self->table.ids = nullptr;
    self->table.count = 0;
    self->table.capacity = 0;
    self->table.step = 10;
    self->table.flags = 0xe;
    self->table.allocator = fn_7100083990();
}

static inline void CallSlot(UnkSoundOwnerObject_b0038* self, u32 slot) {
    reinterpret_cast<void (*)(UnkSoundOwnerObject_b0038*)>(self->vtable[slot])(self);
}

void fn_710102890c(UnkSoundOwnerObject_b0038* self) {
    Construct(self);
    CallSlot(self, 3);
}

void fn_710102b608(UnkSoundOwnedArray_b0038* self) { self->Free(); }

extern void* g_unk_7101cac6c8[];
void fn_710102849c(void* self, void* alloc, u32 flags);

// An object with two vtables, a bucket table at +0x10 (fn_710102849c, flags 0xe) and a table at +0x68
// (its destructors are fn_71010233ec and fn_7101023544).
struct UnkSoundTableObject_b0038 {
    void** vtable;
    void** vtable2;
    u8 buckets[0x58];  // fn_710102849c
    UnkSoundIdTable_b0038 table;
    u32 field_0x90;
};
static_assert(__builtin_offsetof(UnkSoundTableObject_b0038, table) == 0x68, "");

void fn_7101023378(UnkSoundTableObject_b0038* self) {
    self->vtable = g_unk_7101cac6c8 + 2;
    self->vtable2 = g_unk_7101cac6e8 + 2;
    fn_710102849c(self->buckets, nullptr, 0xe);
    self->vtable = g_unk_7101cac5d8 + 2;
    self->vtable2 = g_unk_7101cac5d8 + 12;
    self->table.records = nullptr;
    self->table.ids = nullptr;
    self->table.count = 0;
    self->table.capacity = 0;
    self->table.step = 10;
    self->table.flags = 0xe;
    self->table.allocator = fn_7100083990();
    self->field_0x90 = 0;
}

// Sync, begin and end of the iterators whose
// current element is kept in a two-state variant, the shape of src/base/UnkUnit2609/
// UnkUnit2609_OwnedIterSync_b55.cpp and UnkUnit2609_OwnedIterBegin_b56.cpp: state 1 owns an object at
// +0x18 (fn_710102796c destroys it, fn_7100083c00 frees it), state 2 holds the pair {iterator,
// entry}.
struct UnkSoundOwnedObject_b0038 {
    u64 field_0x0;
    UnkSoundOwned_b0038* owned;

    __attribute__((always_inline)) void Destroy() {
        UnkSoundOwned_b0038* p = owned;
        owned = nullptr;
        if (p) {
            fn_710102796c(p);
            fn_7100083c00(p);
        }
    }
};

struct UnkSoundVariant_b0038 {
    union {
        UnkSoundOwnedObject_b0038 object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            object.Destroy();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    __attribute__((always_inline)) void SetPair(void* first, void* second) {
        s32* p = &state;
        switch (*p) {
        case 1:
            object.Destroy();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = 2;
        pair.first = first;
        pair.second = second;
    }
};

template <int Tag>
struct UnkSoundIter_b0038 {
    void* field_0x0;
    void* current;
    UnkSoundVariant_b0038 variant;

    __attribute__((always_inline)) void Sync() {
        void* c = current;
        if (c)
            variant.SetPair(this, c);
        else
            variant.Reset();
    }
};

void fn_7101027dbc(UnkSoundIter_b0038<0>* self) { self->Sync(); }
void fn_7101028174(UnkSoundIter_b0038<1>* self) { self->Sync(); }

// A temporary (index, element) iterator built and synced inline, copied to the output unless it is
// the output, the output synced out of line, and the temporary's variant reset.
struct UnkSoundIdxIter_b0038 {
    u32 index;
    void** current;
    UnkSoundVariant_b0038 variant;

    __attribute__((always_inline)) UnkSoundIdxIter_b0038(u32 i, void** c) : index(i), current(c) {
        variant.state = 0;
        if (c) {
            variant.Reset();
            variant.state = 2;
            variant.pair.first = this;
            variant.pair.second = c;
        } else {
            variant.Reset();
        }
    }
    __attribute__((always_inline)) ~UnkSoundIdxIter_b0038() { variant.Reset(); }
};

struct UnkSoundPtrArray_b0038 {
    void** data;
    u32 count;
};

__attribute__((always_inline)) inline void UnkSoundIdxIterAt_b0038(u32 i, void** c,
                                                                    UnkSoundIter_b0038<0>* out) {
    UnkSoundIdxIter_b0038 it(i, c);
    UnkSoundIdxIter_b0038* o = reinterpret_cast<UnkSoundIdxIter_b0038*>(out);
    if (o != &it) {
        o->index = it.index;
        o->current = it.current;
        fn_7101027dbc(out);
    }
}

void fn_710102b780(UnkSoundPtrArray_b0038* self, UnkSoundIter_b0038<0>* out) {
    UnkSoundIdxIterAt_b0038(0, self->data, out);
}

void fn_710102b828(UnkSoundPtrArray_b0038* self, UnkSoundIter_b0038<0>* out) {
    UnkSoundIdxIterAt_b0038(self->count, self->data + self->count, out);
}

// Move-assignment of a pair of an id and a record, held in place (state 1) or by reference
// (state 2), the shape of fn_7100e59270 (src/base/reflection/fn_7100adae44.cpp): the id is moved
// (released, taken, the other's left empty) and the record assigned by fn_7100027118. Names and
// layouts are placeholders.

struct UnkSoundStrId_b0038 {
    base::global::UnkStringData* data;

    ~UnkSoundStrId_b0038() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
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

// The id pointers are moved bitwise (the original reloads the pair's state after them).
__attribute__((always_inline)) static inline void MoveAssign(UnkSoundStrId_b0038* self,
                                                             UnkSoundStrId_b0038* other) {
    self->~UnkSoundStrId_b0038();
    __builtin_memcpy(&self->data, &other->data, sizeof(self->data));
    base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
    __builtin_memcpy(&other->data, &empty, sizeof(empty));
}

using UnkSoundIdRecordPair_b0038 = UnkSoundPairRef_b0038<UnkSoundStrId_b0038, UnkStrIdPathResource>;

template <class Y>
__attribute__((always_inline)) static inline UnkSoundPairRef_b0038<UnkSoundStrId_b0038, Y>* MoveAssignSet(
    UnkSoundPairRef_b0038<UnkSoundStrId_b0038, Y>* self, UnkSoundPairRef_b0038<UnkSoundStrId_b0038, Y>* other) {
    if (other->state == 1) {
        MoveAssign(&self->GetX(), &other->values.x);
        UnkStrIdPathResource* y = &other->values.y;
        fn_7100027118(&self->GetY(), y);
    } else if (other->state == 2) {
        UnkSoundStrId_b0038* x = other->refs.x;
        MoveAssign(&self->GetX(), x);
        UnkStrIdPathResource* y = other->refs.y;
        fn_7100027118(&self->GetY(), y);
    }
    return self;
}

UnkSoundIdRecordPair_b0038* fn_7101029308(UnkSoundIdRecordPair_b0038* self, UnkSoundIdRecordPair_b0038* other) {
    return MoveAssignSet(self, other);
}

// Move-assignment of a pair of a value and an owning pointer from an in-place or by-reference pair.
using UnkSoundU32OwnedPair_b0038 = UnkSoundPairRef_b0038<u32, UnkSoundOwnedPtr_b0038>;

UnkSoundU32OwnedPair_b0038* fn_710102ade4(UnkSoundU32OwnedPair_b0038* self, UnkSoundU32OwnedPair_b0038* other) {
    if (other->state == 1) {
        self->GetX() = other->values.x;
        self->GetY() = static_cast<UnkSoundOwnedPtr_b0038&&>(other->values.y);
    } else if (other->state == 2) {
        self->GetX() = *other->refs.x;
        self->GetY() = static_cast<UnkSoundOwnedPtr_b0038&&>(*other->refs.y);
    }
    return self;
}

// Begin and end slots of a CStrId-keyed flat map of 0x18-byte records (destroyed by fn_7100026f70),
// the shape of src/game/CMinimapData/UnkStrIdIterBeginEnd_A6_b75.h. Names and layouts are
// placeholders.


struct alignas(8) UnkSoundRecord18_b0038 {
    u8 data[0x18];
    ~UnkSoundRecord18_b0038() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(this)); }
};

void fn_710102aa2c(UnkStrIdMap_A6_b75<UnkSoundRecord18_b0038>* self,
                   UnkStrIdIterValue_A6_b75<UnkSoundRecord18_b0038>* out) {
    UnkStrIdMapBegin_A6_b75(self, out);
}

void fn_710102ab40(UnkStrIdMap_A6_b75<UnkSoundRecord18_b0038>* self,
                   UnkStrIdIterValue_A6_b75<UnkSoundRecord18_b0038>* out) {
    UnkStrIdMapEnd_A6_b75(self, out);
}

extern void* g_unk_7101a1b750[];

// A string with 0x80 bytes of inline storage (as in src/base/snd/CSoundSystemATK.cpp).
struct UnkSoundInlineString_b0038 {
    u8 str[0x30];
    void** vtable;
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;
};

struct UnkSoundNamed_b0038 {
    void** vtable;
    UnkSoundInlineString_b0038 name;
};

void fn_7101028b08(UnkSoundNamed_b0038* self) {
    __builtin_memset(&self->name, 0, sizeof(self->name));
    self->vtable = g_unk_7101cabad0 + 2;
    UnkSoundInlineString_b0038* s = &self->name;
    fn_710007e6e4(reinterpret_cast<CRntString*>(s), reinterpret_cast<base::UnkAllocator*>(&s->vtable));
    s->vtable = g_unk_7101a1b750 + 2;
    s->field_0xb8 = 0;
    s->field_0xc0 = 0;
}

// sound::CSoundEventsDef's class descriptor, the static its getter (fn_710101fb24) returns.
extern CClass g_unk_71729f9c40 __attribute__((visibility("hidden")));

void* fn_7101025f4c(void* self, void* source, u32 flags) {
    fn_710101fb24();
    const CClass* desc = &g_unk_71729f9c40;
    if (fn_710009d50c(desc, fn_7100016894()) && desc->m_fn_0x28) {
        void* obj = fn_7100083990()->vf_0x40(desc->m_size, 8, flags);
        if (obj)
            reinterpret_cast<void (*)(void*)>(desc->m_fn_0x28)(obj);
        return obj;
    }
    return nullptr;
}

struct UnkDropTarget;
struct UnkAudioPreset_b0038;
void fn_7101025628(UnkDropTarget* self, UnkAudioPreset_b0038* preset, f32 blend, f32 start);

// Stops the sounds of the preset an id/path resource resolves to, when both ids are set.
void fn_7101025560(UnkDropTarget* self, const UnkStrIdPathPair* ids, f32 blend, f32 start) {
    if (ids->id.data == &g_unk_7101d47108->empty || ids->path.data == &g_unk_7101d47200->empty)
        return;
    auto* obj = reinterpret_cast<UnkReflObject*>(reinterpret_cast<const UnkStrIdPathResource*>(ids)->resource);
    UnkReflObject* preset = obj && fn_710009d50c(obj->vf_0x0(), fn_710102df18()) ? obj : nullptr;
    fn_7101025628(self, reinterpret_cast<UnkAudioPreset_b0038*>(preset), blend, start);
}

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;

struct UnkSoundRef {
    void* object;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};

struct UnkSoundRequest_b50;  // src/game/CAudioComponent/CAudioComponent_b50.cpp

struct UnkAudioPreset_b0038 {
    u8 unk_0x0[0x58];
    u64 key;
    u8 unk_0x60[0x548];
    f32 field_0x5a8;
};

struct UnkSoundRequest_b0038 {
    u8 unk_0x0[0x18];
    u64 key;
    base::global::UnkStringData* path;
};

bool fn_710102423c(UnkDropTarget* self, UnkAudioPreset_b0038* preset);
void fn_71010244a8(UnkDropTarget* self, UnkSoundHandle handle);

UnkSoundHandle fn_710102540c(UnkDropTarget* self, const UnkStrIdPathPair* ids, const UnkSoundRequest_b0038* request) {
    UnkAudioPreset_b0038* preset = nullptr;
    if (ids->id.data != &g_unk_7101d47108->empty && ids->path.data != &g_unk_7101d47200->empty) {
        auto* obj = reinterpret_cast<UnkReflObject*>(reinterpret_cast<const UnkStrIdPathResource*>(ids)->resource);
        preset = reinterpret_cast<UnkAudioPreset_b0038*>(
            obj && fn_710009d50c(obj->vf_0x0(), fn_710102df18()) ? obj : nullptr);
    }
    if (fn_710102423c(self, preset) && request->key == preset->key &&
        (request->path != &g_unk_7101d47200->empty || !(preset->field_0x5a8 > 0.0f))) {
        UnkSoundHandle handle =
            fn_71001080b0(g_unk_7101d4d820, reinterpret_cast<const UnkSoundRequest_b50*>(request));
        if (handle.ref && handle.serial != -1 && handle.ref->serial == handle.serial && handle.ref->object)
            fn_71010244a8(self, handle);
        return handle;
    }
    return g_unk_7108b6c460;
}

// The id and path pair (base/UnkStrIdPathPair.h) the reader opens.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

// The id and path pair built from a resource's path, destroyed out of line.
struct UnkPathPairTemp_b45a1 {
    void* id;
    void* path;
    ~UnkPathPairTemp_b45a1() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710000f750(UnkParseContext* self, void* root);

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_71729f9bb0 __attribute__((visibility("hidden")));

// A reflected resource and its path.
struct UnkSoundResource_b0038 : UnkReflObject {
    u8 path[0x50];  // 0x8
};

struct UnkSoundLoadRequest_b0038 {
    u8 unk_0x0[0x10];
    u32 mode;
};

// Parses the file into the target, starting from its last entry's node; nonzero on success.
static inline s32 Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71729f9bb0, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    return reinterpret_cast<s32 (*)(UnkParseContext*, void*)>(&fn_710000f750)(
        &ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// Reads the resource's reflected object from the file its path names; whether it parsed.
bool fn_7101022d54(UnkSoundResource_b0038* self, const UnkSoundLoadRequest_b0038* request) {
    UnkFileReader reader;
    {
        UnkPathPairTemp_b45a1 s = fn_710001ab24(self->path);
        fn_710007be8c(&reader.file, 0x16, nullptr);
        reader.field_0x38 = 3;
        reader.field_0x3c = 2;
        bool opened;
        {
            base::global::CRntString mode("rb");
            opened = fn_710007c1ac(&reader, reinterpret_cast<UnkStrPair_0x10*>(&s), &mode);
        }
        if (opened)
            fn_710000ccd8(&reader);
    }
    UnkReadTarget target;
    {
        u32 mode = request->mode;
        const base::reflection::CClass* type = self->vf_0x0();
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, self, true);
        fn_71000f0568(&target, &value, mode);
        fn_71000a1c60(&value);
    }
    bool ok = false;
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
        ok = Parse(&reader, &target) != 0;
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
    return ok;
}
