// Unit 2640's unit: the 0x50-byte record (a word, an array of 0x18-byte path records, a plain tail)
// moved out of an array and in an entry holder, and a named map copied. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A file-path id released in two steps (decrement, then reload and test), as CStrId::Reset.
struct UnkPathId_ovb0214 {
    UnkStringData* data;

    ~UnkPathId_ovb0214() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkPathRecord_ovb0214 {
    UnkPathId_ovb0214 id;
    u64 field_0x8;
    u32 field_0x10;
};

// The array of path records (base::UnkVector's layout; flags 0xf when created here). The tag
// constructor leaves the fields for a move constructor to fill.
struct UnkNoInit_ovb0214 {};

struct UnkPathRecords_ovb0214 {
    UnkPathRecord_ovb0214* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkPathRecords_ovb0214(UnkNoInit_ovb0214) {}

    base::UnkVector<UnkPathRecord_ovb0214>& vec() {
        return *reinterpret_cast<base::UnkVector<UnkPathRecord_ovb0214>*>(this);
    }
};

struct __attribute__((packed)) UnkRecordTail_ovb0214 {
    u64 field_0x0;
    f32 field_0x8;
    f32 field_0xc;
    u64 field_0x10;
    u32 field_0x18;
    u8 field_0x1c;
};

struct UnkRecord_ovb0214 {
    u32 field_0x0;
    UnkPathRecords_ovb0214 records;
    UnkRecordTail_ovb0214 tail;

    UnkRecord_ovb0214(UnkRecord_ovb0214&& other) : records(UnkNoInit_ovb0214{}) {
        field_0x0 = other.field_0x0;
        __builtin_memcpy(&records, &other.records, 0x10);  // array, count and capacity
        records.step = other.records.step;
        records.field_0x14 = other.records.field_0x14;
        records.flags = other.records.flags;
        records.allocator = other.records.allocator;
        __builtin_memset(&other.records, 0, 0x10);
        __builtin_memcpy(&tail, &other.tail, sizeof(tail));
    }
    UnkRecord_ovb0214& operator=(UnkRecord_ovb0214&& other) {
        field_0x0 = other.field_0x0;
        records.vec() = static_cast<base::UnkVector<UnkPathRecord_ovb0214>&&>(other.records.vec());
        __builtin_memcpy(&tail, &other.tail, sizeof(tail));
        return *this;
    }
    ~UnkRecord_ovb0214() { records.vec().Free(); }
};
static_assert(sizeof(UnkRecord_ovb0214) == 0x50, "UnkRecord layout");

struct UnkCollection_U2640x50_A6_b67;

void fn_7100ffdac8(UnkCollection_U2640x50_A6_b67* self, u32 index) {
    auto* v = reinterpret_cast<base::UnkVector<UnkRecord_ovb0214>*>(self);
    if (index >= v->count)
        return;
    v->data[index].~UnkRecord_ovb0214();
    for (u32 i = index; i < v->count - 1; ++i) {
        UnkRecord_ovb0214* src = &v->data[i] + 1;
        new (src - 1) UnkRecord_ovb0214(static_cast<UnkRecord_ovb0214&&>(*src));
        (&v->data[i] + 1)->~UnkRecord_ovb0214();
    }
    v->count = v->count - 1;
}

// An entry holder (kind at +0x58: 1 an owned key and record, 2 references to a key and a record
// elsewhere), move-assigned from another: the key and record of the other are moved over the
// holder's own (or referenced) ones.
struct UnkEntryPair_ovb0214 {
    u32 key;
    UnkRecord_ovb0214 value;
};

struct UnkEntryRefs_ovb0214 {
    u32* key;
    UnkRecord_ovb0214* value;
};

struct UnkEntryHolder_ovb0214 {
    union {
        UnkEntryPair_ovb0214 pair;
        UnkEntryRefs_ovb0214 ref;
    };
    s32 kind;

    UnkEntryHolder_ovb0214() {}
    ~UnkEntryHolder_ovb0214() {}
};
static_assert(__builtin_offsetof(UnkEntryHolder_ovb0214, kind) == 0x58, "UnkEntryHolder layout");

UnkEntryHolder_ovb0214* fn_7100ffcaf8(UnkEntryHolder_ovb0214* self, UnkEntryHolder_ovb0214* other) {
    switch (other->kind) {
    case 1: {
        u32 key = other->pair.key;
        (self->kind == 2 ? *self->ref.key : self->pair.key) = key;
        UnkRecord_ovb0214* src = &other->pair.value;
        UnkRecord_ovb0214* dst = self->kind == 2 ? self->ref.value : &self->pair.value;
        *dst = static_cast<UnkRecord_ovb0214&&>(*src);
        break;
    }
    case 2: {
        u32 key = *other->ref.key;
        (self->kind == 2 ? *self->ref.key : self->pair.key) = key;
        UnkRecord_ovb0214* src = other->ref.value;
        UnkRecord_ovb0214* dst = self->kind == 2 ? self->ref.value : &self->pair.value;
        *dst = static_cast<UnkRecord_ovb0214&&>(*src);
        break;
    }
    }
    return self;
}

// Copy constructor of a named map: a string id (the empty one at first), a chained hash map of 11
// buckets and, in a list of all nodes, each of the other's nodes inserted anew (fn_7101005fcc), then
// the name copied. Names and layouts are placeholders.
using base::global::CStrId;

struct UnkMapNode_ovb0214 {
    u8 unk_0x0[0x38];
    UnkMapNode_ovb0214* next;
};

// The pointer is cleared before the pool is made sure of.
struct UnkEmptyId_ovb0214 {
    UnkStringData* data;

    __attribute__((always_inline)) UnkEmptyId_ovb0214() : data(nullptr) {
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkNamedMap_ovb0214 {
    UnkEmptyId_ovb0214 name;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    base::UnkVector<void*> buckets;
    UnkMapNode_ovb0214* tail;
    UnkMapNode_ovb0214* head;

    __attribute__((always_inline)) UnkNamedMap_ovb0214()
        : allocator(fn_7100083990()), flags(0xe), count(0), buckets(0xe), tail(nullptr), head(nullptr) {
        if (buckets.count < 11) {
            buckets.Reserve(11);
            __builtin_memset(&buckets.data[buckets.count], 0, (11 - buckets.count) * sizeof(void*));
            buckets.count = 11;
        } else if (buckets.count > 11) {
            buckets.count = 11;
        }
    }
};

void fn_7101005fcc(void* map, const void* key, const void* value);

void fn_7100fed9dc(void* dst, const void* src) {
    auto* self = new (dst) UnkNamedMap_ovb0214();
    auto* other = static_cast<const UnkNamedMap_ovb0214*>(src);
    *reinterpret_cast<CStrId*>(&self->name) = *reinterpret_cast<const CStrId*>(&other->name);
    for (UnkMapNode_ovb0214* n = other->head; n; n = n->next)
        fn_7101005fcc(&self->allocator, n, n->unk_0x0 + 8);
}
