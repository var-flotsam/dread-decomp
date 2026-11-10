// Unit 1115 (tools/tu_map.py, no class name): a registry of objects. It keeps its objects in an array
// (+0x68), files them by id in parallel arrays (+0x40: the objects and their u32 ids), and keeps a keyed
// table of records (+0x18), each an array of arrays. This file holds the id index append
// (fn_71006fb514), an object refresh from the registry graph (fn_71006facbc), the registry move
// constructor (fn_71006f8ca0), an object constructor (fn_71006f84a8) and the count of entries over
// copies of the records (fn_71006fa894). Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

extern void* g_unk_7101a6b820[];  // the registry vtable
extern void* g_unk_7101a6b7f8[];  // the object vtable

struct UnkWorldGraphEntries_b47a3;
s32 fn_7100acca04(UnkWorldGraphEntries_b47a3* self, const u64* key);

// The registry keyed tables: records (or u32 keys) with a bucket array beside them.
struct UnkTable_b0255 {
    void* data;     // 0x0
    void* buckets;  // 0x8
    u32 count;      // 0x10
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;  // 0x20
};

struct UnkRegistry_b0255;

// An object of the registry: an id, a name, the graph ids it knows and the names it answers to.
struct UnkRegObj_b0255 {
    void* vtable;                        // 0x0
    u32 id;                              // 0x8
    u32 field_0xc;                       // 0xc
    base::global::CStrId name;           // 0x10
    base::UnkVector<u32> ids;            // 0x18
    base::UnkVector<u64> names_storage;  // 0x40, a base::UnkStrIdVec
    UnkRegistry_b0255* registry;         // 0x68

    base::UnkStrIdVec& names() { return *reinterpret_cast<base::UnkStrIdVec*>(&names_storage); }
};
static_assert(__builtin_offsetof(UnkRegObj_b0255, registry) == 0x68, "layout");

// Parallel arrays of objects (as words) and their u32 ids.
struct UnkPairVec_b0255 {
    u64* values;                    // 0x0
    u32* keys;                      // 0x8
    u32 count;                      // 0x10
    u32 capacity;                   // 0x14
    u32 step;                       // 0x18
    u32 flags;                      // 0x1c
    base::UnkAllocator* allocator;  // 0x20
};

struct UnkRegistry_b0255 {
    void* vtable;                               // 0x0
    base::global::CStrId name;                  // 0x8
    u8 field_0x10;                              // 0x10
    UnkTable_b0255 records;                     // 0x18
    UnkPairVec_b0255 index;                     // 0x40
    base::UnkVector<UnkRegObj_b0255*> objects;  // 0x68
    u64 field_0x90;                             // 0x90
    base::UnkVector<u32> field_0x98;            // 0x98
    u32 field_0xc0;                             // 0xc0
    u32 field_0xc4;                             // 0xc4
    void* graph;                                // 0xc8
};
static_assert(__builtin_offsetof(UnkRegistry_b0255, graph) == 0xc8, "layout");

// The query entries, as the other files declare these functions.
struct UnkQueryEntry_b50;

// Appends an object under its id and returns the object slot.
__attribute__((noinline)) u64* fn_71006fb514(void* map, const u32* key, UnkQueryEntry_b50* const* entry) {
    auto* self = static_cast<UnkPairVec_b0255*>(map);
    const u64* value = reinterpret_cast<const u64*>(entry);
    u32 n = self->count;
    if (n == self->capacity) {
        u32 new_capacity = self->step + n;
        if (n < new_capacity) {
            u64* new_values = static_cast<u64*>(self->allocator->vf_0x40((u64)new_capacity * 8, 8, self->flags));
            u32* new_keys = static_cast<u32*>(self->allocator->vf_0x40((u64)new_capacity * 4, 8, self->flags));
            if (self->values) {
                __builtin_memcpy(new_values, self->values, (u64)self->count * 8);
                self->allocator->vf_0x48(self->values);
            }
            if (self->keys) {
                __builtin_memcpy(new_keys, self->keys, (u64)self->count * 4);
                self->allocator->vf_0x48(self->keys);
            }
            n = self->count;
            self->values = new_values;
            self->keys = new_keys;
            self->capacity = new_capacity;
        }
    }
    __builtin_memcpy(&self->keys[n], key, 4);
    __builtin_memcpy(&self->values[self->count], value, 8);
    n = self->count + 1;
    self->count = n;
    u64* end = &self->values[n];
    return n ? end - 1 : self->values;
}

// Refreshes the object graph index and the indices of the graph ids it holds.
struct UnkIdMapOwner_b0255 {
    u8 unk_0x0[0xc8];
    UnkWorldGraphEntries_b47a3* graph;  // 0xc8
};

struct UnkIdMap_b0255 {
    u8 unk_0x0[8];
    u32 index;                     // 0x8
    u64 key;                       // 0x10
    base::UnkVector<u32> indices;  // 0x18
    base::UnkVector<u64> ids;      // 0x40
    UnkIdMapOwner_b0255* owner;    // 0x68
};
static_assert(__builtin_offsetof(UnkIdMap_b0255, owner) == 0x68, "layout");

__attribute__((noinline)) void fn_71006facbc(UnkQueryEntry_b50* entry) {
    auto* self = reinterpret_cast<UnkIdMap_b0255*>(entry);
    self->index = fn_7100acca04(self->owner->graph, &self->key);
    self->indices.count = 0;
    for (u64 *id = self->ids.data, *end = id + self->ids.count; id != end; ++id) {
        u32 index = fn_7100acca04(self->owner->graph, id);
        self->indices.PushBack(index);
    }
}

// The registry move constructor: it takes the other tables and arrays and files its objects again.
void fn_71006f8ca0(UnkRegistry_b0255* self, UnkRegistry_b0255* other) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a6b820) + 0x10;
    self->name.data = other->name.data;
    other->name.data = &g_unk_7101d47108->empty;
    self->field_0x10 = other->field_0x10;
    self->records.data = other->records.data;
    self->records.buckets = other->records.buckets;
    self->records.count = other->records.count;
    self->records.field_0x14 = other->records.field_0x14;
    self->records.field_0x18 = other->records.field_0x18;
    self->records.field_0x1c = other->records.field_0x1c;
    self->records.allocator = other->records.allocator;
    other->records.count = 0;
    other->records.data = nullptr;
    other->records.buckets = nullptr;

    self->index.values = nullptr;
    self->index.keys = nullptr;
    self->index.count = 0;
    self->index.capacity = 0;
    self->index.step = 10;
    self->index.flags = 0xe;
    self->index.allocator = fn_7100083990();

    self->objects.data = other->objects.data;
    self->objects.count = other->objects.count;
    self->objects.capacity = other->objects.capacity;
    self->objects.step = other->objects.step;
    self->objects.field_0x14 = other->objects.field_0x14;
    self->objects.flags = other->objects.flags;
    self->objects.allocator = other->objects.allocator;
    other->objects.data = nullptr;
    other->objects.count = 0;
    other->objects.capacity = 0;
    self->field_0x90 = other->field_0x90;
    self->field_0x98.data = other->field_0x98.data;
    self->field_0x98.count = other->field_0x98.count;
    self->field_0x98.capacity = other->field_0x98.capacity;
    self->field_0x98.step = other->field_0x98.step;
    self->field_0x98.field_0x14 = other->field_0x98.field_0x14;
    self->field_0x98.flags = other->field_0x98.flags;
    self->field_0x98.allocator = other->field_0x98.allocator;
    other->field_0x98.data = nullptr;
    other->field_0x98.count = 0;
    other->field_0x98.capacity = 0;
    self->field_0xc0 = other->field_0xc0;
    self->field_0xc4 = other->field_0xc4;
    self->graph = other->graph;
    other->index.count = 0;
    other->objects.count = 0;

    for (UnkRegObj_b0255 **p = self->objects.data, **end = p + self->objects.count; p != end; ++p) {
        UnkRegObj_b0255* obj = *p;
        obj->registry = self;
        u32 id = obj->id;
        fn_71006fb514(&self->index, &id, reinterpret_cast<UnkQueryEntry_b50* const*>(&obj));
    }
}

// Constructs an object over copies of the given names and ids.
void fn_71006f84a8(UnkRegObj_b0255* self, UnkRegistry_b0255* registry, const base::global::CStrId* name, u32 id,
                   const base::UnkStrIdVec* names, const base::UnkVector<u32>* ids, u32 field_0xc) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a6b7f8) + 0x10;
    self->id = id;
    self->field_0xc = field_0xc;
    new (&self->name) base::global::CStrId(*name);
    self->ids.data = nullptr;
    self->ids.count = 0;
    self->ids.capacity = 0;
    self->ids.step = ids->step;
    self->ids.field_0x14 = ids->field_0x14;
    self->ids.flags = ids->flags;
    self->ids.allocator = ids->allocator;
    self->ids.Reserve(ids->capacity);
    __builtin_memcpy(self->ids.data, ids->data, (u64)ids->count * 4);
    self->ids.count = ids->count;
    new (&self->names()) base::UnkStrIdVec(*names);
    self->registry = registry;
}

// The number of entries in all the records, counted over copies of them (the records are arrays of arrays).
struct UnkInnerVec {
    base::UnkVector<u32> v;
    ~UnkInnerVec() { v.Free(); }
};
typedef base::UnkVector<UnkInnerVec> UnkOuterVec;
void fn_71006fb3f4(UnkOuterVec* self, const UnkOuterVec* other);

struct UnkRecord_b0255 {
    UnkOuterVec vec;  // 0x0
    u32 field_0x28;
};

struct UnkRecordCopy_b0255 {
    union {
        UnkOuterVec vec;
    };
    u32 field_0x28;
    UnkRecordCopy_b0255() {}
};

u32 fn_71006fa894(void* path) {
    auto* self = static_cast<UnkRegistry_b0255*>(path);
    u64 i = 0;
    u32 total = 0;
    for (; i < self->records.count; ++i) {
        const UnkRecord_b0255* record = &static_cast<UnkRecord_b0255*>(self->records.data)[i];
        UnkRecordCopy_b0255 copy;
        fn_71006fb3f4(&copy.vec, &record->vec);
        total += copy.vec.count;
        copy.field_0x28 = record->field_0x28;
        copy.vec.Free();
    }
    return total;
}
