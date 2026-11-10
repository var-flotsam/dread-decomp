// Unit 2601 of tools/tu_map.py (0x7100f83f14-0x7100f87368; no reliable class name): the chained hash map
// (an allocator at +0, the entry count at +0xc, the buckets at +0x10 and the node list's ends at
// +0x38/+0x40) whose nodes hold a string id and an entry of two string ids, an array of 0x50-byte records
// (UnkUnit2601_VecCopy_b56.h) and an array of polymorphic elements, and the operations on that entry:
// move-insert, copy-assignment, move-assignment, the iterator value's move construction and copy
// assignment, and the record array's growth. Names and layouts are placeholders.
#include <new>

#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkPartF84868_b56;
void fn_7100f84868(UnkPartF84868_b56* self, const UnkPartF84868_b56* other);

struct UnkWords16_ovb0224 {
    u64 a, b;
};

// A polymorphic element of a string id and two words, destroyed through its vtable (slot 1).
struct UnkElem_ovb0224 {
    char* vtable;
    CStrId id;
    UnkWords16_ovb0224 words;

    UnkElem_ovb0224& operator=(const UnkElem_ovb0224& other) {
        id = other.id;
        words = other.words;
        return *this;
    }
    void Destroy() { reinterpret_cast<void (**)(UnkElem_ovb0224*)>(vtable)[1](this); }
};

// An array of elements that is copied by fn_7100f84868.
struct UnkPartF84868_b56 {
    UnkElem_ovb0224* data;
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
static inline __attribute__((always_inline)) void TakeVec_ovb0224(V* dst, V* src) {
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

struct UnkEntry_ovb0224 {
    CStrId id;
    UnkRecord50VecMember_b56 records;
    CStrId id2;
    u32 field_0x38;
    UnkPartF84868_b56 part;
};

struct UnkEntryNode_ovb0224 {
    CStrId key;
    UnkEntry_ovb0224 value;
    UnkEntryNode_ovb0224* next;
    UnkEntryNode_ovb0224* prev;
    UnkEntryNode_ovb0224* chain;
};
static_assert(sizeof(UnkEntryNode_ovb0224) == 0x88, "node");

struct UnkEntryMap_ovb0224 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkEntryNode_ovb0224** buckets;
    u32 bucket_used;
    u32 bucket_count;
    u8 unk_0x20[0x10];
    base::UnkAllocator* bucket_allocator;
    UnkEntryNode_ovb0224* tail;
    UnkEntryNode_ovb0224* head;
};

struct UnkStrDataHash_ovb0224 {
    u8 unk_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

UnkEntryNode_ovb0224* fn_7100f87368(UnkEntryMap_ovb0224* self, const CStrId* key, UnkEntry_ovb0224* value) {
    UnkEntryNode_ovb0224* node = (UnkEntryNode_ovb0224*)self->allocator->vf_0x40(sizeof(UnkEntryNode_ovb0224), 8, self->flags);
    if (node) {
        UnkStringData* key_data = key->data;
        node->key.data = key_data;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        if (empty != key_data)
            ++key_data->ref_count;
        node->value.id.data = value->id.data;
        value->id.data = empty;
        TakeVec_ovb0224(&node->value.records.vec, &value->records.vec);
        node->value.id2.data = value->id2.data;
        value->id2.data = &g_unk_7101d47108->empty;
        node->value.field_0x38 = value->field_0x38;
        TakeVec_ovb0224(&node->value.part, &value->part);
        node->next = nullptr;
        node->prev = nullptr;
        node->chain = nullptr;
    }
    return node;
}

static inline __attribute__((always_inline)) UnkEntryNode_ovb0224** FindSlot_ovb0224(UnkEntryMap_ovb0224* self, const CStrId* key) {
    UnkEntryNode_ovb0224** slot = nullptr;
    if (self->bucket_used) {
        const UnkStrDataHash_ovb0224* d = reinterpret_cast<const UnkStrDataHash_ovb0224*>(key->data);
        u32 hash = d->hash_lo;
        hash ^= d->hash_hi;
        u32 index = hash % self->bucket_count;
        slot = &self->buckets[index];
    }
    // keeps the null bucket slot (empty table) from being assumed away.
    asm("" : "+r"(slot));
    return slot;
}

static inline __attribute__((always_inline)) void Link_ovb0224(UnkEntryMap_ovb0224* self, UnkEntryNode_ovb0224* node) {
    UnkEntryNode_ovb0224** link = &self->head;
    if (self->head) {
        if (!self->tail) goto done;
        UnkEntryNode_ovb0224* last = self->tail;
        __builtin_memcpy(&node->prev, &last, sizeof(last));
        link = &self->tail->next;
    }
    *link = node;
done:
    self->tail = node;
    ++self->count;
}

UnkEntryNode_ovb0224* fn_7100f87140(UnkEntryMap_ovb0224* self, const CStrId* key, UnkEntry_ovb0224* value) {
    UnkEntryNode_ovb0224** slot = FindSlot_ovb0224(self, key);
    UnkEntryNode_ovb0224* old = *slot;
    UnkEntryNode_ovb0224* node = fn_7100f87368(self, key, value);
    node->chain = old;
    *slot = node;
    Link_ovb0224(self, node);
    return node;
}

static inline __attribute__((always_inline)) void MoveRecords_ovb0224(UnkRecord50Vec_b56& dst, UnkRecord50Vec_b56& src) {
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

// Declared with this record type in src/base/reflection/fn_7100adae44.cpp.
struct UnkAssignedRecord_f86348;

UnkAssignedRecord_f86348* fn_7100f86348(UnkAssignedRecord_f86348* record, const UnkAssignedRecord_f86348* other_record) {
    UnkEntry_ovb0224* self = reinterpret_cast<UnkEntry_ovb0224*>(record);
    UnkEntry_ovb0224* other = const_cast<UnkEntry_ovb0224*>(reinterpret_cast<const UnkEntry_ovb0224*>(other_record));
    self->id.ReleaseAndTake(other->id);
    MoveRecords_ovb0224(self->records.vec, other->records.vec);
    self->id2.ReleaseAndTake(other->id2);
    self->field_0x38 = other->field_0x38;
    self->part = static_cast<UnkPartF84868_b56&&>(other->part);
    return record;
}

// The iterator value's element with move semantics: ids and arrays are taken over.
template <class E>
struct UnkMoveVec_ovb0224 {
    E* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
    UnkMoveVec_ovb0224(UnkMoveVec_ovb0224&& o)
        : data(o.data), count(o.count), capacity(o.capacity), step(o.step), field_0x14(o.field_0x14),
          flags(o.flags), allocator(o.allocator) {
        o.data = nullptr;
        o.count = 0;
        o.capacity = 0;
    }
};

struct UnkMoveEntry_ovb0224 {
    base::UnkStrIdMember id;
    UnkMoveVec_ovb0224<UnkRecord50b_b56> records;
    base::UnkStrIdMember id2;
    u32 field_0x38;
    UnkMoveVec_ovb0224<UnkElem_ovb0224> part;
};

using UnkEntryIter_ovb0224 = base::UnkIterValue<UnkMoveEntry_ovb0224, base::UnkStrIdMember>;

void fn_7100f868bc(UnkEntryIter_ovb0224* self, UnkEntryIter_ovb0224* other) {
    new (self) UnkEntryIter_ovb0224(static_cast<UnkEntryIter_ovb0224&&>(*other));
}

using UnkElemIter_ovb0224 = base::UnkIterValue<UnkElem_ovb0224, u32>;

UnkElemIter_ovb0224* fn_7100f83f14(UnkElemIter_ovb0224* self, const UnkElemIter_ovb0224* other) {
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

UnkEntry_ovb0224* fn_7100f86150(UnkEntry_ovb0224* self, const UnkEntry_ovb0224* other) {
    self->id = other->id;
    fn_7100f86254(&self->records.vec, &other->records.vec);
    self->id2 = other->id2;
    self->field_0x38 = other->field_0x38;
    fn_7100f84b94(&self->part, &other->part);
    return self;
}

struct UnkPathEntry_ovb0224 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    u8 unk_0x8[0x10];
};

// A 0x50-byte record holding an array of path entries (UnkRecord50b_b56's layout, movable).
struct UnkRec50_ovb0224 {
    u32 field_0x0;
    UnkPathEntry_ovb0224* data;  // 0x8
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

    UnkRec50_ovb0224(UnkRec50_ovb0224&& o)
        : field_0x0(o.field_0x0), data(o.data), count(o.count), capacity(o.capacity), step(o.step),
          field_0x1c(o.field_0x1c), flags(o.flags), allocator(o.allocator) {
        o.data = nullptr;
        o.count = 0;
        o.capacity = 0;
        __builtin_memcpy(&field_0x30, &o.field_0x30, 0x1d);
    }
    ~UnkRec50_ovb0224() {
        if (data) {
            for (u64 j = 0; j < count; ++j)
                data[j].path.~UnkStrIdMemberOf();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
static_assert(sizeof(UnkRec50_ovb0224) == 0x50, "layout");

struct UnkRec50Vec_ovb0224 {
    UnkRec50_ovb0224* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100f85d24(UnkRecord50Vec_b56* vec, u32 n) {
    UnkRec50Vec_ovb0224* self = reinterpret_cast<UnkRec50Vec_ovb0224*>(vec);
    if (self->capacity < n) {
        UnkRec50_ovb0224* mem = static_cast<UnkRec50_ovb0224*>(self->allocator->vf_0x40((u64)n * sizeof(UnkRec50_ovb0224), 8, self->flags));
        if (self->data) {
            UnkRec50_ovb0224* src = self->data;
            UnkRec50_ovb0224* dst = mem;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkRec50_ovb0224(static_cast<UnkRec50_ovb0224&&>(*src));
                src->~UnkRec50_ovb0224();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = mem;
        self->capacity = n;
    }
}
