#include <new>

#include "../../../src/game/UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkPartF84868_b56;
void fn_7100f84868(UnkPartF84868_b56* self, const UnkPartF84868_b56* other);

extern char g_unk_7101cab1f8[];  // the elements' base vtable
extern char g_unk_7101cab400[];  // the elements' vtable

struct UnkWords16_a {
    u64 a, b;
};

// A polymorphic element of a string id and two words, destroyed through its vtable (slot 1).
struct UnkElem_a {
    char* vtable;
    CStrId id;
    UnkWords16_a words;

    UnkElem_a(const UnkElem_a& other) : vtable(g_unk_7101cab1f8 + 0x10), id(other.id) {
        vtable = g_unk_7101cab400 + 0x10;
        words = other.words;
    }
    UnkElem_a& operator=(const UnkElem_a& other) {
        id = other.id;
        words = other.words;
        return *this;
    }
    void Destroy() { reinterpret_cast<void (**)(UnkElem_a*)>(vtable)[1](this); }
};

// An array of elements that is copied by fn_7100f84868.
struct UnkPartF84868_b56 {
    UnkElem_a* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
    UnkPartF84868_b56(const UnkPartF84868_b56& other) { fn_7100f84868(this, &other); }

    UnkPartF84868_b56& operator=(UnkPartF84868_b56&& other) {
        if (this != &other) {
            for (u64 i = 0; i < count; ++i)
                data[i].Destroy();
            count = 0;
            allocator = other.allocator;
            flags = other.flags;
            step = other.step;
            field_0x14 = other.field_0x14;
            count = other.count;
            capacity = other.capacity;
            data = other.data;
            other.data = nullptr;
            other.count = 0;
            other.capacity = 0;
        }
        return *this;
    }
};

template <class V>
static inline __attribute__((always_inline)) void TakeVec_a(V* dst, V* src) {
    dst->data = src->data;
    dst->count = src->count;
    dst->capacity = src->capacity;
    dst->step = src->step;
    dst->field_0x14 = src->field_0x14;
    dst->flags = src->flags;
    dst->allocator = src->allocator;
    src->data = nullptr;
    src->count = 0;
    src->capacity = 0;
}

struct UnkEntry_a {
    CStrId id;
    UnkRecord50VecMember_b56 records;
    CStrId id2;
    u32 field_0x38;
    UnkPartF84868_b56 part;
};

struct UnkEntryNode_a {
    CStrId key;
    UnkEntry_a value;
    UnkEntryNode_a* next;
    UnkEntryNode_a* prev;
    UnkEntryNode_a* chain;
};
static_assert(sizeof(UnkEntryNode_a) == 0x88, "node");

struct UnkEntryMap_a {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkEntryNode_a** buckets;
    u32 bucket_used;
    u32 bucket_count;
    u8 unk_0x20[0x10];
    base::UnkAllocator* bucket_allocator;
    UnkEntryNode_a* tail;
    UnkEntryNode_a* head;
};

struct UnkStrDataHash_a {
    u8 unk_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

UnkEntryNode_a* fn_7100f87368(UnkEntryMap_a* self, const CStrId* key, UnkEntry_a* value) {
    UnkEntryNode_a* node = (UnkEntryNode_a*)self->allocator->vf_0x40(sizeof(UnkEntryNode_a), 8, self->flags);
    if (node) {
        UnkStringData* key_data = key->data;
        node->key.data = key_data;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        if (empty != key_data)
            ++key_data->ref_count;
        node->value.id.data = value->id.data;
        value->id.data = empty;
        TakeVec_a(&node->value.records.vec, &value->records.vec);
        node->value.id2.data = value->id2.data;
        value->id2.data = &g_unk_7101d47108->empty;
        node->value.field_0x38 = value->field_0x38;
        TakeVec_a(&node->value.part, &value->part);
        node->next = nullptr;
        node->prev = nullptr;
        node->chain = nullptr;
    }
    return node;
}

static inline __attribute__((always_inline)) UnkEntryNode_a** FindSlot_a(UnkEntryMap_a* self, const CStrId* key) {
    UnkEntryNode_a** slot = nullptr;
    if (self->bucket_used) {
        const UnkStrDataHash_a* d = reinterpret_cast<const UnkStrDataHash_a*>(key->data);
        u32 hash = d->hash_lo;
        hash ^= d->hash_hi;
        u32 index = hash % self->bucket_count;
        slot = &self->buckets[index];
    }
    // keeps the null bucket slot (empty table) from being assumed away.
    asm("" : "+r"(slot));
    return slot;
}

static inline __attribute__((always_inline)) void Link_a(UnkEntryMap_a* self, UnkEntryNode_a* node) {
    UnkEntryNode_a** link = &self->head;
    if (self->head) {
        if (!self->tail) goto done;
        UnkEntryNode_a* last = self->tail;
        __builtin_memcpy(&node->prev, &last, sizeof(last));
        link = &self->tail->next;
    }
    *link = node;
done:
    self->tail = node;
    ++self->count;
}

UnkEntryNode_a* fn_7100f87140(UnkEntryMap_a* self, const CStrId* key, UnkEntry_a* value) {
    UnkEntryNode_a** slot = FindSlot_a(self, key);
    UnkEntryNode_a* old = *slot;
    UnkEntryNode_a* node = fn_7100f87368(self, key, value);
    node->chain = old;
    *slot = node;
    Link_a(self, node);
    return node;
}

void fn_7100f86fec(UnkEntryMap_a* self, const CStrId* key, const UnkEntry_a* value) {
    UnkEntryNode_a** slot = FindSlot_a(self, key);
    base::UnkAllocator* allocator = self->allocator;
    u32 flags = self->flags;
    void* (base::UnkAllocator::*alloc)(u64, u64, u32) = &base::UnkAllocator::vf_0x40;
    UnkEntryNode_a* old = *slot;
    UnkEntryNode_a* node = (UnkEntryNode_a*)(allocator->*alloc)(sizeof(UnkEntryNode_a), 8, flags);
    new (&node->key) CStrId(*key);
    new (&node->value) UnkEntry_a(*value);
    node->next = nullptr;
    node->prev = nullptr;
    node->chain = old;
    *slot = node;
    Link_a(self, node);
}

void fn_7100d90460(UnkRecord50Vec_b56* self, u32 n);

static inline __attribute__((always_inline)) void MoveRecords_a(UnkRecord50Vec_b56& dst, UnkRecord50Vec_b56& src) {
    if (&dst != &src) {
        fn_7100d90460(&dst, 0);
        dst.allocator = src.allocator;
        dst.flags = src.flags;
        dst.step = src.step;
        dst.field_0x14 = src.field_0x14;
        dst.count = src.count;
        dst.capacity = src.capacity;
        dst.data = src.data;
        src.data = nullptr;
        src.count = 0;
        src.capacity = 0;
    }
}

UnkEntry_a* fn_7100f86348(UnkEntry_a* self, UnkEntry_a* other) {
    self->id.ReleaseAndTake(other->id);
    MoveRecords_a(self->records.vec, other->records.vec);
    self->id2.ReleaseAndTake(other->id2);
    self->field_0x38 = other->field_0x38;
    self->part = static_cast<UnkPartF84868_b56&&>(other->part);
    return self;
}

// Erases the element at the index and moves the later ones down by one.
void fn_7100f843e8(UnkPartF84868_b56* self, const u32* index) {
    u32 at = *index;
    if (self->count > at) {
        u64 i = at;
        self->data[i].Destroy();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkElem_a* cur = &self->data[i];
            new (cur) UnkElem_a(cur[1]);
            (&self->data[i])[1].Destroy();
        }
        self->count = self->count - 1;
    }
}

// The iterator value's element with move semantics: ids and arrays are taken over.
template <class E>
struct UnkMoveVec_a {
    E* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
    UnkMoveVec_a(UnkMoveVec_a&& o)
        : data(o.data), count(o.count), capacity(o.capacity), step(o.step), field_0x14(o.field_0x14),
          flags(o.flags), allocator(o.allocator) {
        o.data = nullptr;
        o.count = 0;
        o.capacity = 0;
    }
};

struct UnkMoveEntry_a {
    base::UnkStrIdMember id;
    UnkMoveVec_a<UnkRecord50b_b56> records;
    base::UnkStrIdMember id2;
    u32 field_0x38;
    UnkMoveVec_a<UnkElem_a> part;
};

using UnkEntryIter_a = base::UnkIterValue<UnkMoveEntry_a, base::UnkStrIdMember>;

void fn_7100f868bc(UnkEntryIter_a* self, UnkEntryIter_a* other) {
    new (self) UnkEntryIter_a(static_cast<UnkEntryIter_a&&>(*other));
}

using UnkElemIter_a = base::UnkIterValue<UnkElem_a, u32>;

UnkElemIter_a* fn_7100f83f14(UnkElemIter_a* self, const UnkElemIter_a* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = other->value.element;
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = *other->ref.element;
    }
    return self;
}

UnkPartF84868_b56* fn_7100f84b94(UnkPartF84868_b56* self, const UnkPartF84868_b56* other);

UnkEntry_a* fn_7100f86150(UnkEntry_a* self, const UnkEntry_a* other) {
    self->id = other->id;
    fn_7100f86254(&self->records.vec, &other->records.vec);
    self->id2 = other->id2;
    self->field_0x38 = other->field_0x38;
    fn_7100f84b94(&self->part, &other->part);
    return self;
}

struct UnkPathEntry_a {
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    u8 unk_0x8[0x10];
};

// A 0x50-byte record holding an array of path entries (UnkRecord50b_b56's layout, movable).
struct UnkRec50_a {
    u32 field_0x0;
    UnkPathEntry_a* data;  // 0x8
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x1c;
    u32 flags;
    base::UnkAllocator* allocator;  // 0x28
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;

    UnkRec50_a(UnkRec50_a&& o)
        : field_0x0(o.field_0x0), data(o.data), count(o.count), capacity(o.capacity), step(o.step),
          field_0x1c(o.field_0x1c), flags(o.flags), allocator(o.allocator) {
        o.data = nullptr;
        o.count = 0;
        o.capacity = 0;
        __builtin_memcpy(&field_0x30, &o.field_0x30, 0x1d);
    }
    ~UnkRec50_a() {
        if (data) {
            for (u64 j = 0; j < count; ++j)
                data[j].path.~UnkStrIdMemberOf();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
static_assert(sizeof(UnkRec50_a) == 0x50, "layout");

struct UnkRec50Vec_a {
    UnkRec50_a* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100f85d24(UnkRecord50Vec_b56* vec, u32 n) {
    UnkRec50Vec_a* self = reinterpret_cast<UnkRec50Vec_a*>(vec);
    if (self->capacity < n) {
        UnkRec50_a* mem = static_cast<UnkRec50_a*>(self->allocator->vf_0x40((u64)n * sizeof(UnkRec50_a), 8, self->flags));
        if (self->data) {
            UnkRec50_a* src = self->data;
            UnkRec50_a* dst = mem;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkRec50_a(static_cast<UnkRec50_a&&>(*src));
                src->~UnkRec50_a();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = mem;
        self->capacity = n;
    }
}

int end_marker_a;
