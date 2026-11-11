// Unit 2640 (tools/tu_map.py; no reliable name): arrays of file-path records and the groups that hold
// them. A path record is a file-path id, a word and a u32; a group is a word, an array of path records
// (at +0x8, assigned by fn_7100ffbad8), a run of plain fields, and an array of u64 ids (at +0x50). Names and layouts
// are placeholders.
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2640.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// ---- Path records

// A file-path id, released in two steps.
struct UnkRecPath_b0213 : base::global::UnkFilePathStrIdBase {
    // The empty path (as CFilePathStrId's default constructor).
    __attribute__((always_inline)) UnkRecPath_b0213() {
        data = nullptr;
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
    UnkRecPath_b0213(const UnkRecPath_b0213& other) : UnkFilePathStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkRecPath_b0213() {
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

struct UnkPathRecord_b0213 {
    UnkRecPath_b0213 id;
    u64 field_0x8 = -1;
    u32 field_0x10 = 1;
};

struct UnkPathRecordVec_b0213 {
    UnkPathRecord_b0213* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkPathRecordVec_b56;
void fn_7100f86020(UnkPathRecordVec_b56* self, u32 n);

__attribute__((always_inline)) static inline void Reserve(UnkPathRecordVec_b0213* self, u32 n) {
    fn_7100f86020(reinterpret_cast<UnkPathRecordVec_b56*>(self), n);
}

struct UnkPartFfca00;

// Copy assignment: the records are destroyed, then the other's appended.
UnkPartFfca00* fn_7100ffbad8(UnkPartFfca00* dst, const UnkPartFfca00* src) {
    auto* self = reinterpret_cast<UnkPathRecordVec_b0213*>(dst);
    auto* other = reinterpret_cast<const UnkPathRecordVec_b0213*>(src);
    if (self != other) {
        for (u32 i = 0; i < self->count; ++i)
            self->data[i].id.~UnkRecPath_b0213();
        self->count = 0;
        Reserve(self, other->capacity);
        for (const UnkPathRecord_b0213 *p = other->data, *e = p + other->count; p != e; ++p) {
            u32 n = self->count;
            u32 old_capacity = self->capacity;
            u32 new_capacity = old_capacity;
            if (old_capacity <= n && self->step != 0)
                new_capacity = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            Reserve(self, new_capacity);
            new (&self->data[self->count]) UnkPathRecord_b0213(*p);
            ++self->count;
        }
    }
    return dst;
}

// The record at `index`; past the end the array grows and the new records are empty paths (-1, 1).
UnkPathRecord_b0213* fn_7100ffc138(void* array, u32 index) {
    auto* self = static_cast<UnkPathRecordVec_b0213*>(array);
    if (self->growable && self->count <= index) {
        u32 grown = self->capacity;
        if (grown <= index && self->step != 0)
            grown = (index / self->step + 1) * self->step;
        Reserve(self, grown);
        u32 n = index + 1;
        Reserve(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkPathRecord_b0213();
        self->count = n;
    }
    return &self->data[index];
}

// ---- Groups

// The element and array types the other files declare these functions with (same names, same layouts).
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

void fn_7100f85f00(UnkPathRecordVec_b56* self, const UnkPathRecordVec_b56* other);

struct UnkPathEntry_b0213 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    u64 field_0x8[2];
};

struct UnkPathGroup_b0213 {
    u32 field_0x0;
    union {
        base::UnkVector<UnkPathEntry_b0213> entries;  // 0x8
    };
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
    union {
        base::UnkVector<u64> ids;  // 0x50
    };

    UnkPathGroup_b0213(UnkPathGroup_b0213&& o) : field_0x0(o.field_0x0) {
        entries.data = o.entries.data;
        entries.count = o.entries.count;
        entries.capacity = o.entries.capacity;
        entries.step = o.entries.step;
        entries.field_0x14 = o.entries.field_0x14;
        entries.flags = o.entries.flags;
        entries.allocator = o.entries.allocator;
        // An untyped clear: the destructor of the moved-from group reads the array pointer again.
        __builtin_memset(&o.entries, 0, 16);
        __builtin_memcpy(&field_0x30, &o.field_0x30, 0x1d);
        ids.data = o.ids.data;
        ids.count = o.ids.count;
        ids.capacity = o.ids.capacity;
        ids.step = o.ids.step;
        ids.field_0x14 = o.ids.field_0x14;
        ids.flags = o.ids.flags;
        ids.allocator = o.ids.allocator;
        __builtin_memset(&o.ids, 0, 16);
    }

    UnkPathGroup_b0213(const UnkPathGroup_b0213& o) : field_0x0(o.field_0x0) {
        fn_7100f85f00(reinterpret_cast<UnkPathRecordVec_b56*>(&entries),
                      reinterpret_cast<const UnkPathRecordVec_b56*>(&o.entries));
        __builtin_memcpy(&field_0x30, &o.field_0x30, 0x1d);
        ids.data = nullptr;
        __builtin_memset(&ids.count, 0, 8);  // count and capacity
        ids.step = o.ids.step;
        ids.field_0x14 = o.ids.field_0x14;
        ids.flags = o.ids.flags;
        ids.allocator = o.ids.allocator;
        u32 capacity = o.ids.capacity;
        u64* p = nullptr;
        if (capacity) {
            p = static_cast<u64*>(ids.allocator->vf_0x40((u64)capacity * sizeof(u64), 8, ids.flags));
            if (ids.data) {
                __builtin_memcpy(p, ids.data, (u64)ids.count * sizeof(u64));
                ids.allocator->vf_0x48(ids.data);
            }
            ids.data = p;
            ids.capacity = capacity;
        }
        __builtin_memcpy(p, o.ids.data, (u64)o.ids.count * sizeof(u64));
        ids.count = o.ids.count;
    }

    // The word, the entries (fn_7100ffbad8), the run of fields, and the ids copied over (the array
    // grows to the other's capacity first).
    UnkPathGroup_b0213& operator=(const UnkPathGroup_b0213& other) {
        field_0x0 = other.field_0x0;
        fn_7100ffbad8(reinterpret_cast<UnkPartFfca00*>(&entries), reinterpret_cast<const UnkPartFfca00*>(&other.entries));
        __builtin_memcpy(&field_0x30, &other.field_0x30, 0x1d);
        if (this != &other) {
            ids.count = 0;
            u32 n = other.ids.capacity;
            u64* p;
            if (ids.capacity < n) {
                p = static_cast<u64*>(ids.allocator->vf_0x40((u64)n * sizeof(u64), 8, ids.flags));
                if (ids.data) {
                    __builtin_memcpy(p, ids.data, (u64)ids.count * sizeof(u64));
                    ids.allocator->vf_0x48(ids.data);
                }
                ids.data = p;
                ids.capacity = n;
            } else {
                p = ids.data;
            }
            __builtin_memcpy(p, other.ids.data, (u64)other.ids.count * sizeof(u64));
            ids.count = other.ids.count;
        }
        return *this;
    }

    __attribute__((always_inline)) ~UnkPathGroup_b0213() {
        ids.Free();
        entries.Free();
    }
};
static_assert(sizeof(UnkPathGroup_b0213) == 0x78, "UnkPathGroup_b0213 layout");
static_assert(__builtin_offsetof(UnkPathGroup_b0213, ids) == 0x50, "UnkPathGroup_b0213 layout");

// Destroys every group, keeping the array.
void fn_71010017f4(UnkElemArray<UnkElem78>* array) {
    auto* self = reinterpret_cast<base::UnkVector<UnkPathGroup_b0213>*>(array);
    for (u32 i = 0; i < self->count; ++i)
        self->data[i].~UnkPathGroup_b0213();
    self->count = 0;
}

// Grows the array to n groups, moving the old ones over.
__attribute__((noinline)) void fn_710100147c(UnkElemArray<UnkElem78>* array, u32 n) {
    auto* self = reinterpret_cast<base::UnkVector<UnkPathGroup_b0213>*>(array);
    if (self->capacity < n) {
        auto* p = static_cast<UnkPathGroup_b0213*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkPathGroup_b0213), 8, self->flags));
        if (self->data) {
            UnkPathGroup_b0213* src = self->data;
            UnkPathGroup_b0213* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkPathGroup_b0213(static_cast<UnkPathGroup_b0213&&>(*src));
                src->~UnkPathGroup_b0213();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
}

// Copy assignment of a group iterator's pair.
void* fn_7101000928(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkPathGroup_b0213, u32>;
    V* self = static_cast<V*>(dst);
    const V* other = static_cast<const V*>(src);
    if (self != other) {
        self->Index() = other->Index();
        self->Element() = other->Element();
    }
    return self;
}

// ---- Path points

struct UnkCollection_U2640x18_A6_b67;

struct UnkPathPoint_b0213 {
    UnkStringData* name;
    f32 x, y;
    f32 z;
    u32 unk_0x14;

    UnkPathPoint_b0213(UnkPathPoint_b0213&& other) : name(other.name) {
        other.name = &g_unk_7101d47200->empty;
        __builtin_memcpy(&x, &other.x, 8);
        z = other.z;
    }
    ~UnkPathPoint_b0213() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (name && name != &pool->empty) {
                --name->ref_count;
                if (name->ref_count == 0) {
                    fn_71000803a4(pool, name);
                    pool = g_unk_7101d47200;
                }
            }
            name = &pool->empty;
        }
    }
};

void fn_7100ffbff0(UnkCollection_U2640x18_A6_b67* array, u32 index) {
    auto* self = reinterpret_cast<base::UnkVector<UnkPathPoint_b0213>*>(array);
    if (index < self->count) {
        self->data[index].~UnkPathPoint_b0213();
        for (u32 i = index; i < self->count - 1; ++i) {
            UnkPathPoint_b0213* e = &self->data[i];
            new (e) UnkPathPoint_b0213(static_cast<UnkPathPoint_b0213&&>(e[1]));
            (&self->data[i])[1].~UnkPathPoint_b0213();
        }
        self->count = self->count - 1;
    }
}

// ---- Id vector entries

using base::global::CStrId;

struct UnkRecord50Vec_b56;
UnkRecord50Vec_b56* fn_7100f86254(UnkRecord50Vec_b56* self, const UnkRecord50Vec_b56* other);

struct UnkIdVecEntry_b0213 {
    u8 vec_0x0[0x28];
    CStrId id;  // 0x28

    UnkIdVecEntry_b0213& operator=(const UnkIdVecEntry_b0213& other) {
        fn_7100f86254(reinterpret_cast<UnkRecord50Vec_b56*>(vec_0x0),
                      reinterpret_cast<const UnkRecord50Vec_b56*>(other.vec_0x0));
        id = other.id;
        return *this;
    }
};

void* fn_7100fff328(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkIdVecEntry_b0213, CStrId>;
    V* self = static_cast<V*>(dst);
    const V* other = static_cast<const V*>(src);
    if (other != self) {
        self->Index() = other->Index();
        self->Element() = other->Element();
    }
    return self;
}

// ---- Map iterator

void fn_7101005d8c(void* node);

// A chained map's node list (links at +0x38) and bucket array, as the maps of
// src/game/UnkStrIdMapErase_A6_b73.cpp keep them.
struct UnkNodeMap_b0213 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    union {
        base::UnkVector<void*> buckets;  // 0x10
    };
    char* tail;  // 0x38
    char* head;  // 0x40

    __attribute__((always_inline)) ~UnkNodeMap_b0213() {
        if (count) {
            for (char* n = head; n;) {
                char* next = *reinterpret_cast<char**>(n + 0x38);
                fn_7101005d8c(n);
                allocator->vf_0x48(n);
                n = next;
            }
            __builtin_memset(buckets.data, 0, (u64)buckets.capacity * 8);
            count = 0;
            head = nullptr;
            tail = nullptr;
        }
        buckets.Free();
    }
};

struct UnkMapEntry_b0213 {
    base::UnkStrIdMember id;
    UnkNodeMap_b0213 map;
};

struct UnkMapIter_b0213 {
    void* node;
    union {
        struct {
            base::UnkStrIdMember key;
            UnkMapEntry_b0213 value;
        } entry;  // kind 1
    };
    u32 kind;

    UnkMapIter_b0213() {}
    __attribute__((always_inline)) ~UnkMapIter_b0213() {
        switch (kind) {
        case 1:
            entry.value.map.~UnkNodeMap_b0213();
            entry.value.id.~UnkStrIdMemberOf();
            entry.key.~UnkStrIdMemberOf();
            kind = 0;
            break;
        case 2:
            kind = 0;
            break;
        }
    }
};

void fn_7100ff89f8(UnkMapIter_b0213* it);

struct UnkNodeMapHead_b0213 {
    u8 unk_0x0[0x40];
    void* head;
};

void fn_7101004c74(const UnkNodeMapHead_b0213*, UnkMapIter_b0213* out) {
    UnkMapIter_b0213 it;
    it.node = nullptr;
    it.kind = 0;
    fn_7100ff89f8(&it);
    out->node = it.node;
    fn_7100ff89f8(out);
}

// The begin iterator: a temporary starts at the first node and is advanced, the result takes its node
// and is advanced in turn.
void fn_7101004b0c(const UnkNodeMapHead_b0213* map, UnkMapIter_b0213* out) {
    UnkMapIter_b0213 it;
    it.node = map->head;
    it.kind = 0;
    fn_7100ff89f8(&it);
    out->node = it.node;
    fn_7100ff89f8(out);
}

// Destroys every group and frees the array.
void fn_7100ffaa88(void* array) {
    auto* self = static_cast<base::UnkVector<UnkPathGroup_b0213>*>(array);
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkPathGroup_b0213();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}

// ---- Id array map

void fn_7101001694(UnkElemArray<UnkElem78>* self, const UnkElem78* value);

struct UnkHashView_b0213 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkIdArrayRecord_b0213 {
    CStrId id;
    UnkElemArray<UnkElem78> items;
};

struct UnkIdArrayNode_b0213 {
    CStrId key;
    UnkIdArrayRecord_b0213 value;
    UnkIdArrayNode_b0213* next;
    UnkIdArrayNode_b0213* prev;
    UnkIdArrayNode_b0213* chain;
};

struct UnkIdArrayMap_b0213 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkIdArrayNode_b0213** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
    u8 pad_0x20[0x18];
    UnkIdArrayNode_b0213* tail;
    UnkIdArrayNode_b0213* head;
};

UnkIdArrayNode_b0213* fn_7101005e44(UnkIdArrayMap_b0213* self, const CStrId* key, const UnkIdArrayRecord_b0213* value) {
    UnkIdArrayNode_b0213** slot = nullptr;
    if (self->bucket_count) {
        u64 h = reinterpret_cast<const UnkHashView_b0213*>(key->data)->hash;
        u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
        // the bucket index is computed before the bucket array is loaded
        asm("" : "+r"(i));
        slot = &self->buckets[i];
    }
    UnkIdArrayNode_b0213* chain = *slot;
    auto* n = static_cast<UnkIdArrayNode_b0213*>(self->allocator->vf_0x40(0x50, 8, self->flags));
    if (n) {
        new (&n->key) CStrId(*key);
        new (&n->value.id) CStrId(value->id);
        n->value.items.data = nullptr;
        n->value.items.count = 0;
        n->value.items.capacity = 0;
        n->value.items.step = value->items.step;
        n->value.items.field_0x14 = value->items.field_0x14;
        n->value.items.flags = value->items.flags;
        n->value.items.allocator = value->items.allocator;
        fn_710100147c(&n->value.items, value->items.capacity);
        for (const UnkElem78 *it = value->items.data, *end = it + value->items.count; it != end; ++it)
            fn_7101001694(&n->value.items, it);
        n->chain = nullptr;
        n->prev = nullptr;
        n->next = nullptr;
    }
    n->chain = chain;
    *slot = n;
    if (!self->head) {
        self->head = n;
    } else if (self->tail) {
        UnkIdArrayNode_b0213* tail = self->tail;
        __builtin_memcpy(&n->prev, &tail, sizeof(tail));
        self->tail->next = n;
    }
    self->tail = n;
    ++self->count;
    return n;
}

// ---- Id maps

// A map with an id in front (an id and a map of Node nodes: a key, a value, then the links).
template <class Node>
struct UnkIdMapOf_b0213 {
    CStrId id;
    base::UnkAllocator* allocator;  // 0x8
    u32 flags;
    u32 count;
    union {
        base::UnkVector<Node*> buckets;  // 0x18
    };
    Node* tail;  // 0x40
    Node* head;  // 0x48
};

// The value of an outer map's nodes (0x50 bytes), a map of UnkIdArrayNode_b0213 nodes.
using UnkIdMapValue_b0213 = UnkIdMapOf_b0213<UnkIdArrayNode_b0213>;
static_assert(sizeof(UnkIdMapValue_b0213) == 0x50, "UnkIdMapValue_b0213 layout");

// The empty map: the empty id, an allocator, and eleven empty buckets.
template <class Node>
__attribute__((always_inline)) static inline void ConstructEmpty(UnkIdMapOf_b0213<Node>* self) {
    self->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id.data = &g_unk_7101d47108->empty;
    self->allocator = fn_7100083990();
    self->flags = 0xe;
    self->count = 0;
    self->buckets.data = nullptr;
    self->buckets.count = 0;
    self->buckets.capacity = 0;
    self->buckets.step = 10;
    self->buckets.field_0x14 = true;
    self->buckets.flags = 0xe;
    self->buckets.allocator = fn_7100083990();
    self->tail = nullptr;
    self->head = nullptr;
    u32 n = 11;
    if (self->buckets.count < n) {
        if (self->buckets.capacity < n) {
            auto** p = static_cast<Node**>(
                self->buckets.allocator->vf_0x40((u64)n * sizeof(void*), 8, self->buckets.flags));
            if (self->buckets.data) {
                __builtin_memcpy(p, self->buckets.data, (u64)self->buckets.count * sizeof(void*));
                self->buckets.allocator->vf_0x48(self->buckets.data);
            }
            self->buckets.capacity = n;
            self->buckets.data = p;
        }
        __builtin_memset(self->buckets.data + self->buckets.count, 0, (u64)(n - self->buckets.count) * sizeof(void*));
    } else if (self->buckets.count == n) {
        return;
    }
    self->buckets.count = n;
}

void fn_7100feee0c(UnkIdMapValue_b0213* self) {
    ConstructEmpty(self);
}

// The copy: an empty map, then the other's id and each of its nodes inserted.
void fn_7100fed7b0(void* dst, const void* src) {
    auto* self = static_cast<UnkIdMapValue_b0213*>(dst);
    auto* other = static_cast<const UnkIdMapValue_b0213*>(src);
    ConstructEmpty(self);
    self->id = other->id;
    for (const UnkIdArrayNode_b0213* it = other->head; it; it = it->next)
        fn_7101005e44(reinterpret_cast<UnkIdArrayMap_b0213*>(&self->allocator), &it->key,
                      reinterpret_cast<const UnkIdArrayRecord_b0213*>(&it->value));
}

// ---- Groups, continued

// Move assignment: the word, the entries taken over, the run of fields, the ids taken over.
UnkPathGroup_b0213* fn_7101000724(UnkPathGroup_b0213* self, UnkPathGroup_b0213* other) {
    self->field_0x0 = other->field_0x0;
    self->entries = static_cast<base::UnkVector<UnkPathEntry_b0213>&&>(other->entries);
    __builtin_memcpy(&self->field_0x30, &other->field_0x30, 0x1d);
    self->ids = static_cast<base::UnkVector<u64>&&>(other->ids);
    return self;
}

// ---- Id array records

// A string id and an array of groups; copied by appending each of the other's groups.
struct UnkIdArrayCopy_b0213 {
    CStrId id;
    UnkElemArray<UnkElem78> items;

    UnkIdArrayCopy_b0213(const UnkIdArrayCopy_b0213& other) : id(other.id) {
        items.data = nullptr;
        items.count = 0;
        items.capacity = 0;
        items.step = other.items.step;
        items.field_0x14 = other.items.field_0x14;
        items.flags = other.items.flags;
        items.allocator = other.items.allocator;
        fn_710100147c(&items, other.items.capacity);
        for (const UnkElem78 *it = other.items.data, *end = it + other.items.count; it != end; ++it)
            fn_7101001694(&items, it);
    }
};

// Moves an iterator value (kind 1: its key and record; kind 2: the key and record it refers to).
void fn_710100330c(void* dst, void* src) {
    base::refl_IterValueMove<UnkIdArrayCopy_b0213, base::UnkStrIdMember>(dst, src);
}

// ---- Id map iterators

// The id map of fn_7100fed7b0, copy-assigned by inserting each of the other's nodes.
struct UnkIdMapElem_b0213 : UnkIdMapValue_b0213 {
    UnkIdMapElem_b0213& operator=(const UnkIdMapElem_b0213& other) {
        if (this != &other) {
            id = other.id;
            for (const UnkIdArrayNode_b0213* it = other.head; it; it = it->next)
                fn_7101005e44(reinterpret_cast<UnkIdArrayMap_b0213*>(&allocator), &it->key,
                              reinterpret_cast<const UnkIdArrayRecord_b0213*>(&it->value));
        }
        return *this;
    }
};

// Copy assignment of an iterator value (a key and an id map).
void* fn_7101004004(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkIdMapElem_b0213, CStrId>;
    V* self = static_cast<V*>(dst);
    const V* other = static_cast<const V*>(src);
    if (other != self) {
        self->Index() = other->Index();
        self->Element() = other->Element();
    }
    return self;
}

// An id and an array of groups, copy-assigned by clearing the array and appending the other's groups.
struct UnkIdArrayAssign_b0213 {
    CStrId id;
    UnkElemArray<UnkElem78> items;

    __attribute__((always_inline)) UnkIdArrayAssign_b0213& operator=(const UnkIdArrayAssign_b0213& other) {
        id = other.id;
        if (this != &other) {
            fn_71010017f4(&items);
            fn_710100147c(&items, other.items.capacity);
            for (const UnkElem78 *it = other.items.data, *end = it + other.items.count; it != end; ++it)
                fn_7101001694(&items, it);
        }
        return *this;
    }
};

// Copy assignment of an iterator value (a key and an id array record).
void* fn_7101002d28(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkIdArrayAssign_b0213, CStrId>;
    V* self = static_cast<V*>(dst);
    const V* other = static_cast<const V*>(src);
    if (self != other) {
        self->Index() = other->Index();
        self->Element() = other->Element();
    }
    return self;
}

// Copy construction of a group iterator value (kind 1: its pair; kind 2: the pair it refers to).
void fn_7101000d34(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkPathGroup_b0213, u32>;
    V* self = static_cast<V*>(dst);
    const V* other = static_cast<const V*>(src);
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        new (&self->value) V::Value{other->value.index, other->value.element};
    } else if (other->state == 2) {
        self->state = 1;
        V::Ref r = other->ref;
        new (&self->value) V::Value{*r.index, *r.element};
    }
}
