// Unit 2640 (tools/tu_map.py; no reflected class name): destructors, moves, copies and map inserts for
// records of string ids, growable arrays of 0x50- and 0x78-byte elements
// (src/game/UnkUnit2601/UnkUnit2601_VecCopy_b56.h) and chained hash maps keyed by string ids. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "game/UnkTU2640.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

// ---- 0x78-byte records (two arrays) ----

struct UnkElem78 {
    u8 unk_0x0[0x78];
};

template <class Elem>
struct UnkElemArray {
    Elem* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710100147c(UnkElemArray<UnkElem78>* self, u32 capacity);
void fn_71010017f4(UnkElemArray<UnkElem78>* self);  // clear

struct UnkRec78Init_b0212 {
    u64 field_0x0;
    base::UnkVector<u8> items_0x8{0xf};
    u64 field_0x30 = 0;
    f32 scale_x = 1.0f;
    f32 scale_y = 1.0f;
    u64 field_0x40 = 0;
    u32 field_0x48;
    u8 field_0x4c = 0;
    base::UnkVector<u8> items_0x50{0xf};
};

// The array's resize: reserve, then value-initialise the new records.
void fn_7101002358(UnkElemArray<UnkRec78Init_b0212>* self, u32 n) {
    fn_710100147c(reinterpret_cast<UnkElemArray<UnkElem78>*>(self), n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkRec78Init_b0212();
    self->count = n;
}

// The record's value-initialising constructor.
void fn_7101000300(UnkRec78Init_b0212* self) { new (self) UnkRec78Init_b0212(); }

struct UnkMoveRec78_b0212 {
    u32 field_0x0;
    void* data;     // 0x8
    u32 size;       // 0x10
    u32 capacity;   // 0x14
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    u64 field_0x28;
    u8 tail[0x1d];  // 0x30
    void* data2;    // 0x50
    u32 size2;      // 0x58
    u32 capacity2;  // 0x5c
    u32 field_0x60;
    bool field_0x64;
    u32 field_0x68;
    u64 field_0x70;
};

// The signature src/reflection/class_descriptors_02.cpp declares (a type's move slot); `src` is emptied.
void fn_710100047c(void* dst, const void* src) {
    UnkMoveRec78_b0212* self = static_cast<UnkMoveRec78_b0212*>(dst);
    UnkMoveRec78_b0212* other = static_cast<UnkMoveRec78_b0212*>(const_cast<void*>(src));
    self->field_0x0 = other->field_0x0;
    self->data = other->data;
    self->size = other->size;
    self->capacity = other->capacity;
    self->field_0x18 = other->field_0x18;
    self->field_0x1c = other->field_0x1c;
    self->field_0x20 = other->field_0x20;
    self->field_0x28 = other->field_0x28;
    other->data = nullptr;
    other->size = 0;
    other->capacity = 0;
    __builtin_memcpy(self->tail, other->tail, sizeof(self->tail));
    self->data2 = other->data2;
    self->size2 = other->size2;
    self->capacity2 = other->capacity2;
    self->field_0x60 = other->field_0x60;
    self->field_0x64 = other->field_0x64;
    self->field_0x68 = other->field_0x68;
    self->field_0x70 = other->field_0x70;
    other->data2 = nullptr;
    other->size2 = 0;
    other->capacity2 = 0;
}

struct UnkWordVec_b0212 {
    u64* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Grows the array to n words, moving the old ones over.
    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity < n) {
            u64* p = static_cast<u64*>(allocator->vf_0x40((u64)n * sizeof(u64), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(u64));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
};

struct UnkCopyRec78_b0212 {
    u32 field_0x0;
    UnkPathRecordsVec_b56 records_0x8;
    u8 tail[0x1d];  // 0x30
    UnkWordVec_b0212 words_0x50;

    UnkCopyRec78_b0212(const UnkCopyRec78_b0212& o) : field_0x0(o.field_0x0), records_0x8(o.records_0x8) {
        __builtin_memcpy(tail, o.tail, sizeof(tail));
        words_0x50.data = nullptr;
        words_0x50.count = 0;
        words_0x50.capacity = 0;
        words_0x50.step = o.words_0x50.step;
        words_0x50.field_0x14 = o.words_0x50.field_0x14;
        words_0x50.flags = o.words_0x50.flags;
        words_0x50.allocator = o.words_0x50.allocator;
        words_0x50.Reserve(o.words_0x50.capacity);
        __builtin_memcpy(words_0x50.data, o.words_0x50.data, (u64)o.words_0x50.count * sizeof(u64));
        words_0x50.count = o.words_0x50.count;
    }
};

// The copy constructor (slot 0x30 in src/reflection/class_descriptors_02.cpp).
void fn_7101000384(void* dst, const void* src) {
    new (dst) UnkCopyRec78_b0212(*static_cast<const UnkCopyRec78_b0212*>(src));
}

struct UnkPartFfca00;

struct UnkAssignRec78_b0212 {
    u32 field_0x0;
    u64 part_0x8[5];
    u8 tail[0x1d];  // 0x30
    UnkWordVec_b0212 words_0x50;
};

// The reflected copy assignment (slot 0x48 in src/reflection/class_descriptors_02.cpp).
void* fn_710100060c(void* dst, const void* src) {
    UnkAssignRec78_b0212* self = static_cast<UnkAssignRec78_b0212*>(dst);
    const UnkAssignRec78_b0212* other = static_cast<const UnkAssignRec78_b0212*>(src);
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        fn_7100ffbad8(reinterpret_cast<UnkPartFfca00*>(self->part_0x8),
                      reinterpret_cast<const UnkPartFfca00*>(other->part_0x8));
        __builtin_memcpy(self->tail, other->tail, sizeof(self->tail));
        self->words_0x50.count = 0;
        self->words_0x50.Reserve(other->words_0x50.capacity);
        __builtin_memcpy(self->words_0x50.data, other->words_0x50.data,
                         (u64)other->words_0x50.count * sizeof(u64));
        self->words_0x50.count = other->words_0x50.count;
    }
    return self;
}

// ---- destructors of records of ids and arrays ----

template <int Tag>
struct UnkIdVecId_b0212 {
    base::UnkStrIdMember id_0x0;
    UnkRecord50Vec_b56 vec_0x8;
    base::UnkStrIdMember id_0x30;
};

template <int Tag>
static inline void DestroyIdVecId_b0212(UnkIdVecId_b0212<Tag>* self) {
    UnkRecord50Vec_b56& v = self->vec_0x8;
    self->id_0x30.~UnkStrIdMemberOf();
    if (v.data) {
        fn_7100d90460(&v, 0);
        v.allocator->vf_0x48(v.data);
        v.capacity = 0;
    }
    self->id_0x0.~UnkStrIdMemberOf();
}

void fn_7100ff4350(char* self) { DestroyIdVecId_b0212(reinterpret_cast<UnkIdVecId_b0212<0>*>(self)); }
void fn_7100ff4844(char* self) { DestroyIdVecId_b0212(reinterpret_cast<UnkIdVecId_b0212<1>*>(self)); }
void fn_7100fffd7c(char* self) { DestroyIdVecId_b0212(reinterpret_cast<UnkIdVecId_b0212<2>*>(self)); }

struct UnkIdMap_b0212 {
    base::UnkStrIdMember id_0x0;
    UnkChainedMap map_0x8;
};

void fn_7100fed6cc(char* self) {
    UnkIdMap_b0212* m = reinterpret_cast<UnkIdMap_b0212*>(self);
    if (m->map_0x8.count) {
        char* n = reinterpret_cast<char*>(m->map_0x8.tail);
        while (n) {
            char* next = *reinterpret_cast<char**>(n + 0x38);
            fn_7101005d8c(n);
            m->map_0x8.allocator->vf_0x48(n);
            n = next;
        }
        __builtin_memset(m->map_0x8.buckets.data, 0, (u64)m->map_0x8.buckets.capacity * sizeof(char*));
        m->map_0x8.count = 0;
        m->map_0x8.head = nullptr;
        m->map_0x8.tail = nullptr;
    }
    m->map_0x8.buckets.Free();
    m->id_0x0.~UnkStrIdMemberOf();
}

void fn_7100fee158(char*);

struct UnkIdMapNode_b0212 {
    base::UnkStrIdMember key;
    u8 value[0x50];
    UnkIdMapNode_b0212* next;
};

void fn_7100fee034(char* self) {
    UnkIdMap_b0212* m = reinterpret_cast<UnkIdMap_b0212*>(self);
    if (m->map_0x8.count) {
        UnkIdMapNode_b0212* n = reinterpret_cast<UnkIdMapNode_b0212*>(m->map_0x8.tail);
        while (n) {
            UnkIdMapNode_b0212* next = n->next;
            fn_7100fed6cc(reinterpret_cast<char*>(n->value));
            n->key.~UnkStrIdMemberOf();
            m->map_0x8.allocator->vf_0x48(n);
            n = next;
        }
        __builtin_memset(m->map_0x8.buckets.data, 0, (u64)m->map_0x8.buckets.capacity * sizeof(char*));
        m->map_0x8.count = 0;
        m->map_0x8.head = nullptr;
        m->map_0x8.tail = nullptr;
    }
    fn_7100fee158(reinterpret_cast<char*>(&m->map_0x8));
    m->id_0x0.~UnkStrIdMemberOf();
}

struct UnkIdRecord18_b0212 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;  // CFilePathStrId's pool
    u8 unk_0x8[0x10];
};

template <int Tag, u32 Pre>
struct UnkTwoArrays_b0212 {
    u8 unk_0x0[Pre];
    base::UnkVector<UnkIdRecord18_b0212> records;
    u8 unk_gap[0x20];
    base::UnkVector<u64> words;
};

template <class T>
static inline void DestroyTwoArrays_b0212(T* self) {
    self->words.Free();
    self->records.Free();
}

void fn_7100ff5b70(char* self) { DestroyTwoArrays_b0212(reinterpret_cast<UnkTwoArrays_b0212<0, 0x10>*>(self)); }
void fn_7100ff6054(char* self) { DestroyTwoArrays_b0212(reinterpret_cast<UnkTwoArrays_b0212<1, 0x10>*>(self)); }
void fn_7101000518(void* self) { DestroyTwoArrays_b0212(reinterpret_cast<UnkTwoArrays_b0212<2, 0x8>*>(self)); }

// ---- a 0x50-byte-record array with an id ----

struct UnkVecId_b0212 {
    UnkRecord50Vec_b56 vec_0x0;
    base::UnkStrIdMember id_0x28;
};

void fn_7100ffee68(void* obj) {
    UnkVecId_b0212* self = static_cast<UnkVecId_b0212*>(obj);
    self->id_0x28.~UnkStrIdMemberOf();
    UnkRecord50Vec_b56& v = self->vec_0x0;
    if (v.data) {
        fn_7100d90460(&v, 0);
        v.allocator->vf_0x48(v.data);
        v.capacity = 0;
    }
}

// The reflected move assignment (slot 0x50 in src/reflection/class_descriptors_02.cpp).
void* fn_7100ffef90(void* dst, const void* src) {
    UnkVecId_b0212* self = static_cast<UnkVecId_b0212*>(dst);
    UnkVecId_b0212* other = static_cast<UnkVecId_b0212*>(const_cast<void*>(src));
    if (self != other) {
        fn_7100d90460(&self->vec_0x0, 0);
        self->vec_0x0.allocator = other->vec_0x0.allocator;
        self->vec_0x0.flags = other->vec_0x0.flags;
        self->vec_0x0.step = other->vec_0x0.step;
        self->vec_0x0.field_0x14 = other->vec_0x0.field_0x14;
        self->vec_0x0.count = other->vec_0x0.count;
        self->vec_0x0.capacity = other->vec_0x0.capacity;
        self->vec_0x0.data = other->vec_0x0.data;
        other->vec_0x0.data = nullptr;
        other->vec_0x0.count = 0;
        other->vec_0x0.capacity = 0;
        self->id_0x28.~UnkStrIdMemberOf();
        new (&self->id_0x28) base::UnkStrIdMember(static_cast<base::UnkStrIdMember&&>(other->id_0x28));
    }
    return self;
}

struct UnkIdArray78_b0212 {
    base::UnkStrIdMember id_0x0;
    UnkElemArray<UnkElem78> items_0x8;
};

// The reflected move assignment (slot 0x50 in src/reflection/class_descriptors_02.cpp).
void* fn_7101002ae0(void* dst, const void* src) {
    UnkIdArray78_b0212* self = static_cast<UnkIdArray78_b0212*>(dst);
    UnkIdArray78_b0212* other = static_cast<UnkIdArray78_b0212*>(const_cast<void*>(src));
    if (self != other) {
        self->id_0x0.~UnkStrIdMemberOf();
        new (&self->id_0x0) base::UnkStrIdMember(static_cast<base::UnkStrIdMember&&>(other->id_0x0));
        fn_71010017f4(&self->items_0x8);
        self->items_0x8.allocator = other->items_0x8.allocator;
        self->items_0x8.flags = other->items_0x8.flags;
        self->items_0x8.step = other->items_0x8.step;
        self->items_0x8.field_0x14 = other->items_0x8.field_0x14;
        self->items_0x8.count = other->items_0x8.count;
        self->items_0x8.capacity = other->items_0x8.capacity;
        self->items_0x8.data = other->items_0x8.data;
        other->items_0x8.data = nullptr;
        other->items_0x8.count = 0;
        other->items_0x8.capacity = 0;
    }
    return self;
}

struct UnkIdRecords_b0212 {
    u32 field_0x0;
    base::UnkVector<UnkIdRecord18_b0212> items_0x8;
    u8 tail[0x1d];  // 0x30
};

// The reflected move assignment (slot 0x50 in src/reflection/class_descriptors_02.cpp).
void* fn_7100ffc6c0(void* dst, const void* src) {
    UnkIdRecords_b0212* self = static_cast<UnkIdRecords_b0212*>(dst);
    UnkIdRecords_b0212* other = static_cast<UnkIdRecords_b0212*>(const_cast<void*>(src));
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        self->items_0x8 = static_cast<base::UnkVector<UnkIdRecord18_b0212>&&>(other->items_0x8);
        __builtin_memcpy(self->tail, other->tail, sizeof(self->tail));
    }
    return self;
}

// ---- hash maps keyed by string ids ----

struct UnkVecIdMapNode_b0212 {
    CStrId key;
    UnkVecId_b0212 value;
    u8 unk_0x38[0x10];
    UnkVecIdMapNode_b0212* chain;
};

struct UnkVecIdLookup_b0212 {
    u8 unk_0x0[0x10];
    UnkVecIdMapNode_b0212** buckets;
    u32 count;
    u32 bucket_count;
};

static inline UnkVecIdMapNode_b0212* Find_b0212(UnkVecIdLookup_b0212* self, const CStrId* key) {
    UnkVecIdMapNode_b0212** slot = nullptr;
    if (self->count) {
        const char* raw = reinterpret_cast<const char*>(key->data);
        u32 h = *reinterpret_cast<const u32*>(raw + 0x30) ^ *reinterpret_cast<const u32*>(raw + 0x34);
        u32 i = h % self->bucket_count;
        slot = self->buckets + i;
    }
    if (!slot)
        return nullptr;
    UnkVecIdMapNode_b0212* n = *slot;
    if (!n)
        return nullptr;
    while (n->key.data != key->data) {
        n = n->chain;
        if (!n)
            return nullptr;
    }
    return n;
}

// The setter of the reflected map: moves the given value into the key's node when there is one.
void fn_7100fffb98(UnkVecIdLookup_b0212* self, const CStrId* key, UnkVecId_b0212* value) {
    UnkVecIdMapNode_b0212* n = Find_b0212(self, key);
    if (!n)
        return;
    UnkVecId_b0212* dst = &n->value;
    if (&dst->vec_0x0 != &value->vec_0x0) {
        fn_7100d90460(&dst->vec_0x0, 0);
        dst->vec_0x0.allocator = value->vec_0x0.allocator;
        dst->vec_0x0.flags = value->vec_0x0.flags;
        dst->vec_0x0.step = value->vec_0x0.step;
        dst->vec_0x0.field_0x14 = value->vec_0x0.field_0x14;
        dst->vec_0x0.count = value->vec_0x0.count;
        dst->vec_0x0.capacity = value->vec_0x0.capacity;
        dst->vec_0x0.data = value->vec_0x0.data;
        value->vec_0x0.data = nullptr;
        value->vec_0x0.count = 0;
        value->vec_0x0.capacity = 0;
    }
    dst->id_0x28.~UnkStrIdMemberOf();
    new (&dst->id_0x28) base::UnkStrIdMember(static_cast<base::UnkStrIdMember&&>(value->id_0x28));
}

// Insert: a node with the key (retained) and the value copied or moved in, at the head of its bucket and
// the tail of the node list (as UnkValueMap::Insert in base/UnkCloneHelpers.h), returning the node.
struct UnkVecIdValue_b0212 {
    UnkRecord50Vec_b56 vec_0x0;
    union {
        base::UnkStrIdMember id_0x28;
    };

    ~UnkVecIdValue_b0212() {}
    UnkVecIdValue_b0212(const UnkVecIdValue_b0212& other) {
        fn_7100f85c28(&vec_0x0, &other.vec_0x0);
        UnkStringData* v = other.id_0x28.data;
        id_0x28.data = v;
        if (&g_unk_7101d47108->empty != v)
            ++v->ref_count;
    }
};

struct UnkHashedData_b0212 {
    char unk_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkVecIdNode_b0212 {
    UnkStringData* key;
    alignas(8) u8 value[0x30];
    UnkVecIdNode_b0212* next;   // 0x38
    UnkVecIdNode_b0212* prev;   // 0x40
    UnkVecIdNode_b0212* chain;  // 0x48

    UnkVecIdNode_b0212(UnkStringData* const* k, const UnkVecIdValue_b0212& v) {
        UnkStringData* d = *k;
        key = d;
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
        new (value) UnkVecIdValue_b0212(v);
        prev = nullptr;
        chain = nullptr;
        next = nullptr;
    }

    UnkVecIdNode_b0212(UnkStringData* const* k, UnkVecIdValue_b0212&& v) {
        UnkStringData* d = *k;
        key = d;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        if (empty != d)
            ++d->ref_count;
        UnkVecIdValue_b0212* dst = reinterpret_cast<UnkVecIdValue_b0212*>(value);
        dst->vec_0x0.data = v.vec_0x0.data;
        dst->vec_0x0.count = v.vec_0x0.count;
        dst->vec_0x0.capacity = v.vec_0x0.capacity;
        dst->vec_0x0.step = v.vec_0x0.step;
        dst->vec_0x0.field_0x14 = v.vec_0x0.field_0x14;
        dst->vec_0x0.flags = v.vec_0x0.flags;
        dst->vec_0x0.allocator = v.vec_0x0.allocator;
        v.vec_0x0.data = nullptr;
        v.vec_0x0.count = 0;
        v.vec_0x0.capacity = 0;
        dst->id_0x28.data = v.id_0x28.data;
        v.id_0x28.data = empty;
        prev = nullptr;
        chain = nullptr;
        next = nullptr;
    }
};

struct UnkVecIdMap_b0212 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkVecIdNode_b0212** buckets;
    u32 bucket_used;
    u32 bucket_count;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    UnkVecIdNode_b0212* tail;
    UnkVecIdNode_b0212* head;

    template <class V>
    __attribute__((always_inline)) UnkVecIdNode_b0212* Insert(UnkStringData* const* key, V&& value) {
        UnkVecIdNode_b0212** b = nullptr;
        if (bucket_used) {
            const auto* d = reinterpret_cast<const UnkHashedData_b0212*>(*key);
            u32 h = d->hash_lo ^ d->hash_hi;
            u32 i = h % bucket_count;
            b = buckets + i;
        }
        UnkVecIdNode_b0212* old = *b;
        auto* n = static_cast<UnkVecIdNode_b0212*>(allocator->vf_0x40(sizeof(UnkVecIdNode_b0212), 8, flags));
        if (n)
            new (n) UnkVecIdNode_b0212(key, static_cast<V&&>(value));
        n->chain = old;
        *b = n;
        if (!head)
            head = n;
        else if (UnkVecIdNode_b0212* t = tail) {
            *reinterpret_cast<UnkVecIdNode_b0212**>(reinterpret_cast<char*>(n) +
                                                    __builtin_offsetof(UnkVecIdNode_b0212, prev)) = t;
            tail->next = n;
        }
        tail = n;
        ++count;
        return n;
    }
};

UnkVecIdNode_b0212* fn_7100fffe60(UnkVecIdMap_b0212* m, UnkStringData* const* key, const UnkVecIdValue_b0212* value) {
    return m->Insert(key, *value);
}

UnkVecIdNode_b0212* fn_7100ffff90(UnkVecIdMap_b0212* m, UnkStringData* const* key, UnkVecIdValue_b0212* value) {
    return m->Insert(key, static_cast<UnkVecIdValue_b0212&&>(*value));
}

// Erase of a chained hash map keyed by string ids (as src/game/UnkStrIdMapErase_A6_b73.cpp): the node's
// destructor, its allocator free, the next node returned.
template <u32 NextOffset>
struct UnkStrIdMap_b41a3;

namespace {

struct UnkHashView_b0212 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkEraseMap_b0212 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;     // 0xc
    char** buckets;  // 0x10
    u32 bucket_count;     // 0x18
    u32 bucket_capacity;  // 0x1c
    u8 pad_0x20[0x18];
    char* tail;  // 0x38
    char* head;  // 0x40
};

template <u32 Chain>
static inline __attribute__((always_inline)) char*& Link(char* n, u32 off) {
    return *reinterpret_cast<char**>(n + off);
}

template <u32 Chain, typename F>
static inline __attribute__((always_inline)) void* Erase(void* map, const CStrId* key, F destroy) {
    UnkEraseMap_b0212* self = static_cast<UnkEraseMap_b0212*>(map);
    auto* k = key->data;
    u64 h = reinterpret_cast<const UnkHashView_b0212*>(k)->hash;
    u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
    asm("" : "+r"(i));  // the original computes the index before loading the buckets
    char** bucket = &self->buckets[i];
    char* n = *bucket;
    if (!n)
        return nullptr;
    char* before = nullptr;
    while (*reinterpret_cast<decltype(k)*>(n) != k) {
        before = n;
        n = Link<Chain>(n, Chain);
        if (!n)
            return nullptr;
    }
    asm("" : "+r"(before));  // keeps &before->chain out of the search loop
    *(before ? &Link<Chain>(before, Chain) : bucket) = Link<Chain>(n, Chain);
    if (self->head == n)
        self->head = Link<Chain>(n, Chain - 0x10);
    if (self->tail == n)
        self->tail = Link<Chain>(n, Chain - 8);
    if (Link<Chain>(n, Chain - 8))
        Link<Chain>(Link<Chain>(n, Chain - 8), Chain - 0x10) = Link<Chain>(n, Chain - 0x10);
    char* r = nullptr;
    if (Link<Chain>(n, Chain - 0x10)) {
        Link<Chain>(Link<Chain>(n, Chain - 0x10), Chain - 8) = Link<Chain>(n, Chain - 8);
        r = Link<Chain>(n, Chain - 0x10);
    }
    destroy(n);
    self->allocator->vf_0x48(n);
    --self->count;
    return r;
}

}  // namespace

void* fn_71010000d0(UnkStrIdMap_b41a3<0x48>* map, const CStrId* key) {
    return Erase<0x48>(map, key, [](char* p) { fn_7100fffd7c(p); });
}
