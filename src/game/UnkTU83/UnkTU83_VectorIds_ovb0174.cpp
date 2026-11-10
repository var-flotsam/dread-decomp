// Unit 83 of tools/tu_map.py (base::reflection by its names), batch b0174: the engine's vector, hash
// set and id helpers: inserts, grows and copies of base::UnkVector instantiations (ids, reflected
// values, strings, words), the erase of parallel id arrays, the move and copy assignment of id
// pairs, the attribute arrays' copy assignment and shrink, and the registry
// (UnkVersionRegistry, src/engine/UnkEngineStart.cpp) constructor and set-up. Names and layouts are
// placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include <new>

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCloneRecords.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkResPathVec.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileCacheSet.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/UnkXformNode.h"
#include "types.h"

// ---- copy-inserts of id vectors (fn_71000ce300, fn_71000d0abc) ----
namespace {

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkIdRef_b0174 {
    UnkStringData* data;
};

template <class Vec, UnkStringPool** Pool, bool AtCount>
__attribute__((always_inline)) inline void Finish(Vec* self, const UnkIdRef_b0174* value, u32 index) {
    auto** data = reinterpret_cast<UnkStringData***>(&self->data);
    UnkStringData** slot = *data;
    slot += AtCount ? self->count : index;
    __builtin_memcpy(slot, &value->data, sizeof(UnkStringData*));
    UnkStringData* d = *slot;
    if (&(*Pool)->empty != d)
        ++d->ref_count;
    ++self->count;
}

template <class Vec, UnkStringPool** Pool, void (*ReserveFn)(Vec*, u32)>
__attribute__((always_inline)) inline void InsertCopy(Vec* self, const UnkIdRef_b0174* value, u32 index) {
    auto** data = reinterpret_cast<UnkStringData***>(&self->data);
    u32 n = self->count;
    u32 cap = self->capacity;
    if (n == index) {
        if (cap <= n && self->step != 0)
            cap = (index / self->step + 1) * self->step;
        if ((s32)cap <= (s32)index)
            for (;;) {
            }
        ReserveFn(self, cap);
        Finish<Vec, Pool, true>(self, value, 0);
        return;
    } else {
        if (cap <= n && self->step != 0)
            cap = (n / self->step + 1) * self->step;
        ReserveFn(self, cap);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkStringData** q = &(*data)[i];
            q[1] = q[0];
            q[0] = &(*Pool)->empty;
            if (UnkStringPool* pool = *Pool) {
                UnkStringData** slot = &(*data)[i];
                if (*slot && *slot != &pool->empty) {
                    --(*slot)->ref_count;
                    if ((*slot)->ref_count == 0) {
                        fn_71000803a4(pool, *slot);
                        pool = *Pool;
                    }
                }
                *slot = &pool->empty;
            }
            if (i == 0)
                break;
        }
    }
    Finish<Vec, Pool, false>(self, value, index);
}

}  // namespace

void fn_71000ce300(base::UnkStrIdVec* self, const base::global::CStrId* value, u32 index) {
    InsertCopy<base::UnkStrIdVec, &g_unk_7101d47108, fn_710000542c>(
        self, reinterpret_cast<const UnkIdRef_b0174*>(value), index);
}

void fn_71000d0abc(UnkResPathVec* self, const base::global::CFilePathStrId* value, u32 index) {
    InsertCopy<UnkResPathVec, &g_unk_7101d47200, fn_7100018450>(
        self, reinterpret_cast<const UnkIdRef_b0174*>(value), index);
}

// ---- shrink of the attribute arrays (fn_71000d8290) ----
using base::global::CStrId;
using base::reflection::UnkReflValue;

void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);  // copy constructor

void fn_71000d8290(UnkPropertyArrays* self) {
    if (self->count != self->capacity) {
        UnkReflValue* values = static_cast<UnkReflValue*>(
            self->allocator->vf_0x40((u64)self->count * sizeof(UnkReflValue), 8, self->flags));
        CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
        if (self->values) {
            UnkReflValue* src = static_cast<UnkReflValue*>(self->values);
            UnkReflValue* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                fn_71000a179c(dst, src);
                fn_71000a1c60(src);
            }
            self->allocator->vf_0x48(self->values);
        }
        if (CStrId* old = self->ids) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
                new (&ids[j]) CStrId(*old);
                old->Reset();
            }
            self->allocator->vf_0x48(self->ids);
        }
        u32 n = self->count;
        self->values = values;
        self->ids = ids;
        self->capacity = n;
    }
}

// ---- grow of a word vector (fn_71000cf92c) ----
struct UnkNegKey_b0174 {
    u64 v;
    UnkNegKey_b0174() { __builtin_memset(&v, 0xff, 8); }
    UnkNegKey_b0174(UnkNegKey_b0174&& o) { __builtin_memcpy(&v, &o.v, 8); }
};
UnkNegKey_b0174* fn_71000cf92c(base::UnkVector<UnkNegKey_b0174>* self, u32 index) {
    return &self->GrowAt(index);
}

// ---- grow of a string vector (fn_71000e0bb8) ----
void fn_710007e6c4(void* self);

struct UnkStringElem_b0174 {
    base::UnkNamedEntry value;

    base::global::CRntString* Str() { return reinterpret_cast<base::global::CRntString*>(&value); }

    UnkStringElem_b0174() { fn_710007e6c4(&value); }
    UnkStringElem_b0174(UnkStringElem_b0174&& other) { fn_710007eab4(Str(), other.Str()); }
    UnkStringElem_b0174(const UnkStringElem_b0174& other) { fn_710007ea60(&value, &other.value); }
    ~UnkStringElem_b0174() { fn_710007ecd4(Str()); }
};

UnkStringElem_b0174* fn_71000e0bb8(base::UnkVector<UnkStringElem_b0174>* self, u32 index) {
    return &self->GrowAt(index);
}

// ---- copies of word and string vectors (fn_71000cf1fc, fn_71000e0320, fn_71000e0524) ----
struct UnkWord_b0174 {
    u64 v;
    UnkWord_b0174(const UnkWord_b0174& o) { __builtin_memcpy(&v, &o.v, 8); }
    UnkWord_b0174(UnkWord_b0174&& o) { __builtin_memcpy(&v, &o.v, 8); }
};

__attribute__((flatten)) void fn_71000cf1fc(base::UnkVector<UnkWord_b0174>* self, const base::UnkVector<UnkWord_b0174>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    if (other->count != 0) {
        const UnkWord_b0174* end = other->data + other->count;
        for (const UnkWord_b0174* p = other->data; p != end; ++p)
            self->PushBackCopy(*p);
    }
}


__attribute__((flatten)) void fn_71000e0320(base::UnkVector<UnkStringElem_b0174>* self, const base::UnkVector<UnkStringElem_b0174>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    if (other->count != 0) {
        const UnkStringElem_b0174* end = other->data + other->count;
        for (const UnkStringElem_b0174* p = other->data; p != end; ++p)
            self->PushBackCopy(*p);
    }
}

__attribute__((flatten)) base::UnkVector<UnkStringElem_b0174>* fn_71000e0524(base::UnkVector<UnkStringElem_b0174>* self, const base::UnkVector<UnkStringElem_b0174>* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        if (other->count != 0) {
            const UnkStringElem_b0174* end = other->data + other->count;
            for (const UnkStringElem_b0174* p = other->data; p != end; ++p)
                self->PushBackCopy(*p);
        }
    }
    return self;
}

// ---- copy-insert of reflected values (fn_71000d5a68) ----
using base::reflection::UnkReflValue;

void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);  // copy constructor

struct UnkReflValueElem_b0174 {
    UnkReflValue value;

    UnkReflValueElem_b0174(UnkReflValueElem_b0174&& other) { fn_71000a179c(&value, &other.value); }
    // fn_71000a179c's body, inlined: the kind, then the value rebuilt from the other's type and storage.
    UnkReflValueElem_b0174(const UnkReflValueElem_b0174& other) {
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = other.value.kind;
        const void* src = nullptr;
        if (other.value.type) {
            if (other.value.field_0x14 == 1 || *reinterpret_cast<const u64*>(other.value.type + 0x18) >= 9)
                src = other.value.field_0x0;
            else
                src = &other.value;
        }
        fn_71000a18d4(&value, reinterpret_cast<const base::reflection::CType*>(other.value.type), src, false);
    }
    ~UnkReflValueElem_b0174() { fn_71000a1c60(&value); }
};

typedef base::UnkVector<UnkReflValueElem_b0174> ReflValueVector_b0174;

// Appends a copy (src/base/reflection/UnkReflValueVectorAssign_b52.cpp declares it with its own names).
struct UnkReflValueVector_7100095acc;
struct UnkReflValueElem_7100095acc;
void fn_7100095e98(UnkReflValueVector_7100095acc* self, const UnkReflValueElem_7100095acc* value);

static inline void Move_b0174(ReflValueVector_b0174* self, u32 new_capacity) {
    auto* p = static_cast<UnkReflValueElem_b0174*>(
        self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkReflValueElem_b0174), 8, self->flags));
    if (self->data) {
        UnkReflValueElem_b0174* src = self->data;
        UnkReflValueElem_b0174* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkReflValueElem_b0174(static_cast<UnkReflValueElem_b0174&&>(*src));
            src->~UnkReflValueElem_b0174();
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = p;
    self->capacity = new_capacity;
}

void fn_71000d5a68(ReflValueVector_b0174* self, const UnkReflValueElem_b0174* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_7100095e98(reinterpret_cast<UnkReflValueVector_7100095acc*>(self),
                      reinterpret_cast<const UnkReflValueElem_7100095acc*>(value));
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0) {
        u32 grown = (n / self->step + 1) * self->step;
        if (cap < grown) {
            Move_b0174(self, grown);
            n = self->count;
        }
    }
    for (u32 i = n - 1; i >= index; --i) {
        UnkReflValueElem_b0174* q = &self->data[i];
        new (q + 1) UnkReflValueElem_b0174(static_cast<UnkReflValueElem_b0174&&>(*q));
        self->data[i].~UnkReflValueElem_b0174();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkReflValueElem_b0174(*value);
    ++self->count;
}

// ---- copy assignment of the attribute arrays (fn_71000d8088) ----
using base::global::CStrId;
using base::reflection::UnkReflValue;

void fn_71000d8290(UnkPropertyArrays* self);

UnkPropertyArrays* fn_71000d8088(UnkPropertyArrays* self, const UnkPropertyArrays* other) {
    if (self != other) {
        if (self->ids && self->count) {
            for (u64 i = 0; i < self->count; ++i)
                self->ids[i].Reset();
        }
        UnkReflValue* values = static_cast<UnkReflValue*>(self->values);
        if (values && self->count) {
            for (u64 i = 0; i < self->count; ++i)
                fn_71000a1c60(&static_cast<UnkReflValue*>(self->values)[i]);
        }
        self->count = 0;
        if (other->count != self->capacity) {
            fn_71000d8290(self);
            fn_7100093dd0(self, other->count);
        }
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i) {
            UnkReflValue* dst = &static_cast<UnkReflValue*>(self->values)[i];
            const UnkReflValue* src = &static_cast<const UnkReflValue*>(other->values)[i];
            dst->field_0x0 = nullptr;
            dst->type = nullptr;
            dst->kind = src->kind;
            const void* p = nullptr;
            if (src->type) {
                if (src->field_0x14 == 1 || *reinterpret_cast<const u64*>(src->type + 0x18) >= 9)
                    p = src->field_0x0;
                else
                    p = src;
            }
            fn_71000a18d4(dst, reinterpret_cast<const base::reflection::CType*>(src->type), p, false);
        }
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i) {
            base::global::UnkStringData* d = other->ids[i].data;
            __builtin_memcpy(&self->ids[i], &d, sizeof(d));
            if (&g_unk_7101d47108->empty != d)
                ++d->ref_count;
        }
    }
    return self;
}

// ---- erase of parallel id arrays (fn_71000d9de8, fn_71000db82c) ----
// A string id over `Pool` taken over on a move (the moved-from id left empty through an untyped
// store: the original reloads the count after it), released in two steps.
template <base::global::UnkStringPool*& Pool>
struct UnkMovedId_b0174 : base::global::UnkStrIdBase {
    __attribute__((always_inline)) UnkMovedId_b0174(UnkMovedId_b0174&& other) : UnkStrIdBase(other) {
        base::global::UnkStringData* empty = &Pool->empty;
        __builtin_memcpy(&other.data, &empty, sizeof(empty));
    }
    ~UnkMovedId_b0174() {
        if (base::global::UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

// Two parallel arrays of string ids (values and the keys that name them) with a shared count.
template <class V, class K>
struct UnkIdPair_b0174 {
    V* values;
    K* keys;
    u32 count;

    // Releases the ids at the position, then moves the last pair into the hole.
    __attribute__((always_inline)) V* Erase(V* pos) {
        u64 offset = pos - values;
        u32 index = offset;
        values[index].~V();
        keys[index].~K();
        if (count - 1 != (u32)offset) {
            new (&values[index]) V(static_cast<V&&>(values[count - 1]));
            values[count - 1].~V();
            new (&keys[index]) K(static_cast<K&&>(keys[count - 1]));
            keys[count - 1].~K();
        }
        count = count - 1;
        return &values[index];
    }
};

using UnkStrPair_b0174 = UnkIdPair_b0174<UnkMovedId_b0174<g_unk_7101d47108>, UnkMovedId_b0174<g_unk_7101d47108>>;
using UnkPathPair_b0174 = UnkIdPair_b0174<UnkMovedId_b0174<g_unk_7101d47200>, UnkMovedId_b0174<g_unk_7101d47108>>;

UnkMovedId_b0174<g_unk_7101d47108>* fn_71000d9de8(UnkStrPair_b0174* self, UnkMovedId_b0174<g_unk_7101d47108>* pos) {
    return self->Erase(pos);
}
UnkMovedId_b0174<g_unk_7101d47200>* fn_71000db82c(UnkPathPair_b0174* self, UnkMovedId_b0174<g_unk_7101d47200>* pos) {
    return self->Erase(pos);
}

// ---- assignments of id pairs (fn_71000da414, fn_71000d8b90, fn_71000da59c) ----
using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

typedef u32 __attribute__((may_alias)) a_u32_b0174;

// Two ids that are either stored inline in the two words, or (kind 2) pointed to by them.
struct UnkIdRefPair_b0174 {
    u64 slot0;
    u64 slot1;
    u32 kind;  // 0x10
    u32 Kind() { return *reinterpret_cast<a_u32_b0174*>(&kind); }

    template <class T>
    T* First() {
        return Kind() == 2 ? *reinterpret_cast<T**>(&slot0) : reinterpret_cast<T*>(&slot0);
    }
    template <class T>
    T* Second() {
        return Kind() == 2 ? *reinterpret_cast<T**>(&slot1) : reinterpret_cast<T*>(&slot1);
    }
};

// Copy-assignment of an id over `Pool` (retain the new one, release the old one).
template <UnkStringPool*& Pool>
__attribute__((always_inline)) inline void AssignId(UnkStringData** dst, UnkStringData** src) {
    if (*dst != *src) {
        UnkStringData* d = *src;
        UnkStringPool* pool = Pool;
        if (d != &pool->empty)
            ++d->ref_count;
        if (*dst != &pool->empty) {
            --(*dst)->ref_count;
            if ((*dst)->ref_count == 0)
                fn_71000803a4(pool, *dst);
        }
        *dst = d;
    }
}

UnkIdRefPair_b0174* fn_71000da414(UnkIdRefPair_b0174* self, const UnkIdRefPair_b0174* other_) {
    UnkIdRefPair_b0174* other = const_cast<UnkIdRefPair_b0174*>(other_);
    if (other != self) {
        UnkStringData** src = other->First<UnkStringData*>();
        AssignId<g_unk_7101d47108>(self->First<UnkStringData*>(), src);
        src = other->Second<UnkStringData*>();
        AssignId<g_unk_7101d47200>(self->Second<UnkStringData*>(), src);
    }
    return self;
}

UnkIdRefPair_b0174* fn_71000d8b90(UnkIdRefPair_b0174* self, UnkIdRefPair_b0174* other) {
    switch (other->kind) {
    case 1: {
        CStrId* src = reinterpret_cast<CStrId*>(&other->slot0);
        self->First<CStrId>()->ReleaseAndTake(*src);
        src = reinterpret_cast<CStrId*>(&other->slot1);
        self->Second<CStrId>()->ReleaseAndTake(*src);
        break;
    }
    case 2: {
        CStrId* src = *reinterpret_cast<CStrId**>(&other->slot0);
        self->First<CStrId>()->ReleaseAndTake(*src);
        src = *reinterpret_cast<CStrId**>(&other->slot1);
        self->Second<CStrId>()->ReleaseAndTake(*src);
        break;
    }
    }
    return self;
}

UnkIdRefPair_b0174* fn_71000da59c(UnkIdRefPair_b0174* self, UnkIdRefPair_b0174* other) {
    switch (other->kind) {
    case 1: {
        CStrId* src = reinterpret_cast<CStrId*>(&other->slot0);
        self->First<CStrId>()->ReleaseAndTake(*src);
        CFilePathStrId* psrc = reinterpret_cast<CFilePathStrId*>(&other->slot1);
        *self->Second<CFilePathStrId>() = static_cast<CFilePathStrId&&>(*psrc);
        break;
    }
    case 2: {
        CStrId* src = *reinterpret_cast<CStrId**>(&other->slot0);
        self->First<CStrId>()->ReleaseAndTake(*src);
        CFilePathStrId* psrc = *reinterpret_cast<CFilePathStrId**>(&other->slot1);
        *self->Second<CFilePathStrId>() = static_cast<CFilePathStrId&&>(*psrc);
        break;
    }
    }
    return self;
}

// ---- grow of a 3D vector array (fn_71000cc374) ----
// A vector element zeroed on creation, copied word by word.
struct UnkZeroVec3f_b0174 : UnkVec3f {
    UnkZeroVec3f_b0174() {
        a_u32* w = reinterpret_cast<a_u32*>(this);
        w[0] = 0;
        w[1] = 0;
        w[2] = 0;
    }
    UnkZeroVec3f_b0174(UnkZeroVec3f_b0174&& o) : UnkVec3f(o) {}
};

UnkZeroVec3f_b0174* fn_71000cc374(base::UnkVector<UnkZeroVec3f_b0174>* self, u32 index) {
    return &self->GrowAt(index);
}

// ---- insert into a 3D vector array (fn_71000cbef4, fn_71000cc134) ----
namespace {

typedef base::UnkVector<UnkVec3f> Vec3Vector_b0174;

__attribute__((always_inline)) inline UnkVec3f* MoveTo_b0174(Vec3Vector_b0174* self, u32 new_capacity) {
    UnkVec3f* p = static_cast<UnkVec3f*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkVec3f), 8, self->flags));
    if (self->data) {
        UnkVec3f* src = self->data;
        UnkVec3f* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src)
            new (dst) UnkVec3f(*src);
        self->allocator->vf_0x48(self->data);
    }
    return p;
}

__attribute__((always_inline)) inline void Insert_b0174(Vec3Vector_b0174* self, UnkVec3f* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        UnkVec3f* p;
        if (cap < new_capacity) {
            p = MoveTo_b0174(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        new (&p[index]) UnkVec3f(*value);
        ++self->count;
        return;
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkVec3f* p = MoveTo_b0174(self, grown);
                self->data = p;
                self->capacity = grown;
                n = self->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkVec3f* q = &self->data[i];
            new (q + 1) UnkVec3f(*q);
            if (i == 0)
                break;
        }
    }
    new (&self->data[index]) UnkVec3f(*value);
    ++self->count;
}

}  // namespace

void fn_71000cbef4(Vec3Vector_b0174* self, UnkVec3f* value, u32 index) { Insert_b0174(self, value, index); }
void fn_71000cc134(Vec3Vector_b0174* self, UnkVec3f* value, u32 index) { Insert_b0174(self, value, index); }

// ---- registry constructor (fn_71000a4598) ----
extern char g_unk_7101a1b828[];  // the registry's vtable
void fn_71000a5460(UnkFileCacheSet* self, u32 buckets, u32 nodes);
void fn_71000a562c(UnkFileCacheSet* self, u32 buckets, u32 nodes);

struct UnkKeyedArray_b0174 {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkVersionRegistry;

struct UnkVersionRegistryLayout_b0174 {
    char* vtable;                        // 0x0
    UnkFileCacheSet set_a;               // 0x8
    base::UnkVector<void*> list_a;       // 0x98
    UnkKeyedArray_b0174 keyed;           // 0xc0
    UnkFileCacheSet set_b;               // 0xe8
    u64 field_0x178;
    base::UnkVector<void*> list_b;       // 0x180
};
static_assert(sizeof(UnkVersionRegistryLayout_b0174) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkVersionRegistryLayout_b0174, set_b) == 0xe8, "layout");

// A file cache set as the registry's constructor builds it.
__attribute__((always_inline)) static inline void InitSet(UnkFileCacheSet* s) {
    s->buckets.count = 0;
    s->buckets.capacity = 0;
    s->buckets.data = nullptr;
    s->buckets.step = 10;
    s->buckets.field_0x14 = true;
    s->buckets.flags = 0xe;
    s->buckets.allocator = fn_7100083990();
    s->field_0x28 = -1;
    s->field_0x30 = nullptr;
    s->allocator = fn_7100083990();
    s->flags = 0xe;
    s->count = 0;
    s->field_0x48 = 1;
    s->pool.Init();
    s->pool.field_0x38 = 0;
    s->pool.field_0x3c = false;
}

void fn_71000a4598(UnkVersionRegistry* self_) {
    auto* self = reinterpret_cast<UnkVersionRegistryLayout_b0174*>(self_);
    self->vtable = g_unk_7101a1b828 + 0x10;
    InitSet(&self->set_a);
    fn_71000a5460(&self->set_a, 5000, 5000);
    self->list_a.data = nullptr;
    self->list_a.count = 0;
    self->list_a.capacity = 0;
    self->list_a.step = 10;
    self->list_a.field_0x14 = true;
    self->list_a.flags = 0x3e;
    self->list_a.allocator = fn_7100083990();
    self->list_a.Reserve(5000);
    self->keyed.values = nullptr;
    self->keyed.keys = nullptr;
    self->keyed.count = 0;
    self->keyed.capacity = 0;
    self->keyed.step = 10;
    self->keyed.flags = 0x3e;
    self->keyed.allocator = fn_7100083990();
    InitSet(&self->set_b);
    fn_71000a562c(&self->set_b, 5000, 5000);
    self->field_0x178 = 0;
    self->list_b.data = nullptr;
    self->list_b.count = 0;
    self->list_b.capacity = 0;
    self->list_b.step = 10;
    self->list_b.field_0x14 = true;
    self->list_b.flags = 0xf;
    self->list_b.allocator = fn_7100083990();
    self->list_b.Reserve(2);
}

// ---- registry set-up (fn_71000a497c) ----
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflAttrs;
using base::reflection::UnkReflValue;

struct UnkVersionRegistry;
struct UnkKeyedPtrsOwner_b52;

// The functions the registry runs at start-up: a list, newest first (nodes link at +0x8).
struct UnkStartHook_b0174 {
    void (*function)(UnkVersionRegistry* registry);
    UnkStartHook_b0174* next;
};
extern UnkStartHook_b0174* g_unk_7101d5bdd0;

void fn_71000a4b60(UnkVersionRegistry* self);
void fn_710009dba0(UnkKeyedPtrsOwner_b52* self, const u64* key, void* value);
UnkFileCacheSetNode** fn_71000a5c1c(base::UnkVector<UnkFileCacheSetNode*>* self, u32 index);
void fn_710009ea3c(CType* type);

// A registered type: the object a type's id (+0x30) names and the type that owns it.
struct UnkTypeKey_b0174 {
    u8 pad_0x0[0x30];
    u64 id;
};
struct UnkFileCacheSetNode {
    UnkTypeKey_b0174* key;
    UnkKeyedPtrsOwner_b52* owner;
    UnkFileCacheSetNode* next;
};

struct UnkRegisteredType_b0174 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
};

// Values and their keys' names side by side, as src/base/reflection/UnkReflAttrs.cpp has it.
struct UnkReflAttrsMap_b0174 {
    UnkReflValue* values;
    CStrId* keys;
    u32 count;

    UnkReflValue* End() { return &values[count]; }
    UnkReflValue* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return End();
    }
};

void fn_71000a497c(UnkVersionRegistry* self_) {
    auto* self = reinterpret_cast<UnkVersionRegistryLayout_b0174*>(self_);
    for (UnkStartHook_b0174* hook = g_unk_7101d5bdd0; hook; hook = hook->next)
        hook->function(self_);
    fn_71000a4b60(self_);
    for (UnkConverter* c = g_unk_7101d5bdc8; c; c = c->next) {
        if (self->set_a.buckets.count == 0)
            continue;
        const u32* halves = reinterpret_cast<const u32*>(&c->from);
        u32 index = (halves[0] ^ halves[1]) % self->set_a.buckets.capacity;
        if (index >= self->set_a.buckets.count)
            continue;
        UnkFileCacheSetNode* node = *fn_71000a5c1c(&self->set_a.buckets, index);
        for (; node; node = node->next) {
            if (node->key->id == reinterpret_cast<u64>(c->from)) {
                fn_710009dba0(node->owner, reinterpret_cast<const u64*>(&c->to), c);
                break;
            }
        }
    }
    for (void** p = self->list_a.data, **e = p + self->list_a.count; p != e; ++p)
        static_cast<UnkRegisteredType_b0174*>(*p)->vf_0x18();
    for (void** p = self->list_a.data, **e = p + self->list_a.count; p != e; ++p) {
        CType* type = static_cast<CType*>(*p);
        auto* attrs = reinterpret_cast<UnkReflAttrsMap_b0174*>(fn_710009dd6c(type));
        const CStrId& version = fn_7100005070().name;
        if (attrs->Find(version) != attrs->End())
            fn_710009ea3c(type);
    }
    for (void** p = self->list_a.data, **e = p + self->list_a.count; p != e; ++p)
        static_cast<UnkRegisteredType_b0174*>(*p)->vf_0x20();
}
