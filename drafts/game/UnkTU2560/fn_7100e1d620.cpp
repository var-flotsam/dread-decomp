// Unit 2560 (the billboard manager, 0xe0 bytes, built by fn_7100e1bca4): its two chained hash maps keyed by
// string id. The map at +0x50 holds lists of up to 16 0x28-byte drop requests (nodes of 0x2a8 bytes), the map
// at +0x98 lists of up to 16 0xb0-byte records (nodes of 0xb28 bytes). Lookup by the id's 64-bit hash, adding a
// request or record under an id, random pick, erase, node copy and destruction. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CStrId;

extern "C" int rand();

struct UnkPart48;
void fn_71000cbae4(UnkPart48* self, const UnkPart48* other);
struct UnkUnit2560Record;
void fn_7100e1d538(UnkUnit2560Record* self);
struct UnkDropTarget_ovb0208;
struct UnkDropRequest_ovb0208;
struct UnkCharClassBillboardLists_ovb0020;
void fn_71007302e4(UnkCharClassBillboardLists_ovb0020* self, char* item);
void fn_71001b7128(void* target, void* resource, u32 index);

// base::UnkVector's layout; the elements of the u32 and u8 arrays are copied inline.
template <class T>
struct UnkArray_b218 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkArray_b218(u32 f)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(f), allocator(fn_7100083990()) {}

    __attribute__((always_inline)) UnkArray_b218(const UnkArray_b218& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        u32 n = other.capacity;
        if (capacity < n) {
            T* p = static_cast<T*>(allocator->vf_0x40((u64)n * sizeof(T), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(T));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(T));
        count = other.count;
    }
};

// The array copied out of line.
struct UnkPartArray_b218 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkPartArray_b218(u32 f)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(f), allocator(fn_7100083990()) {}
    UnkPartArray_b218(const UnkPartArray_b218& other) {
        fn_71000cbae4(reinterpret_cast<UnkPart48*>(this), reinterpret_cast<const UnkPart48*>(&other));
    }
};

#pragma pack(push, 4)
struct UnkRecord_b218 {
    CStrId id;
    u8 field_0x8;
    u8 pad_0x9[7];
    UnkPartArray_b218 field_0x10;
    UnkArray_b218<u32> field_0x38;
    UnkArray_b218<u8> field_0x60;
    UnkArray_b218<u32> field_0x88;

    UnkRecord_b218()
        : id(g_unk_710162e2ed, true), field_0x8(0), field_0x10(0xf), field_0x38(0xf), field_0x60(0xf), field_0x88(0xf) {}
    __attribute__((always_inline)) UnkRecord_b218(const UnkRecord_b218& other)
        : id(other.id), field_0x8(other.field_0x8), field_0x10(other.field_0x10), field_0x38(other.field_0x38),
          field_0x60(other.field_0x60), field_0x88(other.field_0x88) {}
};
static_assert(sizeof(UnkRecord_b218) == 0xb0, "record layout");

struct UnkRecordList_b218 {
    u32 count;
    UnkRecord_b218 items[16];
};

struct UnkDrop_b218 {
    CStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    bool field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    f32 field_0x20;
    u32 pad_0x24;

    UnkDrop_b218(const UnkDrop_b218& other) : id(other.id) {
        __builtin_memcpy(&field_0x8, &other.field_0x8, 0x1c);  // the words, not the last padding
    }
};
static_assert(sizeof(UnkDrop_b218) == 0x28, "drop layout");

struct UnkDropList_b218 {
    u32 count;
    UnkDrop_b218 items[16];
};

#pragma pack(pop)

// A record list that is not constructed (only its count is set).
union UnkRecordListStorage_b218 {
    UnkRecordList_b218 list;
    UnkRecordListStorage_b218() {}
    ~UnkRecordListStorage_b218() {}
};

struct UnkRecordNode_b218 {
    CStrId key;
    UnkRecordList_b218 list;
    UnkRecordNode_b218* next;   // 0xb10
    UnkRecordNode_b218* prev;   // 0xb18
    UnkRecordNode_b218* chain;  // 0xb20

    // The list links, through accessors (their stores may alias each other).
    UnkRecordNode_b218*& Next() { return next; }
    UnkRecordNode_b218*& Prev() { return prev; }
};
static_assert(sizeof(UnkRecordNode_b218) == 0xb28, "node layout");

struct UnkDropNode_b218 {
    CStrId key;
    u32 count;
    UnkDrop_b218 items[16];
    UnkDropNode_b218* next;   // 0x290
    UnkDropNode_b218* prev;   // 0x298
    UnkDropNode_b218* chain;  // 0x2a0

    // The list links, through accessors (their stores may alias each other).
    UnkDropNode_b218*& Next() { return next; }
    UnkDropNode_b218*& Prev() { return prev; }
};
static_assert(sizeof(UnkDropNode_b218) == 0x2a8, "node layout");

template <class Node>
struct UnkMap_b218 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    Node** buckets;                 // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40
};

struct UnkStrData_b218 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
    u64 hash() const { return *reinterpret_cast<const u64*>(&hash_lo); }
};

// A render item as the billboard nodes hold it.
struct UnkBBItem_b218 {
    u8 pad_0x0[0x113];
    u8 field_0x113;
    u8 pad_0x114[0x14];
    void* field_0x128;
};

// One 0x190-byte element of a billboard node; its destructor and update are out of line.
struct UnkBBElement_b218 {
    u8 data[0x190];
    ~UnkBBElement_b218() asm("fn_7100744d48");
};
void fn_7100744e4c(UnkBBElement_b218* element, f32 dt);

struct UnkBillboardNode_b218 {
    u32 id;  // 0x0
    u8 pad_0x4[8];
    u32 count;                     // 0xc
    UnkBBElement_b218* elements;   // 0x10
    UnkBBItem_b218* item;          // 0x18
    char* field_0x20;
    bool enabled;                  // 0x28
    u8 pad_0x29[7];
    void* field_0x30;
    u8 pad_0x38[0x20];
    UnkBillboardNode_b218* next;   // 0x58
    UnkBillboardNode_b218* prev;   // 0x60
    UnkBillboardNode_b218* chain;  // 0x68

    // The list links, through accessors (their stores may alias each other).
    UnkBillboardNode_b218*& Next() { return next; }
    UnkBillboardNode_b218*& Prev() { return prev; }
};
static_assert(sizeof(UnkBillboardNode_b218) == 0x70, "node layout");

struct UnkBillboardManager_b218 {
    UnkMap_b218<UnkBillboardNode_b218> billboards;  // 0x0
    u8 pad_0x48[8];
    UnkMap_b218<UnkDropNode_b218> drops;       // 0x50
    UnkMap_b218<UnkRecordNode_b218> records;   // 0x98
};
static_assert(sizeof(UnkBillboardManager_b218) == 0xe0, "manager layout");

// A record list resized to n: new records are default-built, dropped ones destroyed.
__attribute__((noinline)) void fn_7100e1d414(UnkRecordList_b218* list, u32 n) {
    if (list->count < n) {
        for (u32 i = list->count; i < n; ++i)
            new (&list->items[i]) UnkRecord_b218();
    } else if (list->count > n) {
        for (u64 i = n; i < list->count; ++i)
            fn_7100e1d538(reinterpret_cast<UnkUnit2560Record*>(&list->items[i]));
    }
    list->count = n;
}

__attribute__((noinline)) void fn_7100e1d8d8(UnkRecord_b218* self, const UnkRecord_b218* other) {
    new (self) UnkRecord_b218(*other);
}

static inline u32 HashU64(u64 h) { return (u32)(h >> 32) ^ (u32)h; }

// The node in the map whose key's hash is h, or null.
template <class Node>
static inline __attribute__((always_inline)) Node* FindByHash(UnkMap_b218<Node>* map, u64 h) {
    if (map->bucket_count == 0)
        return nullptr;
    u32 index = HashU64(h) % map->bucket_capacity;
    Node** bucket = map->buckets + index;
    if (!bucket)
        return nullptr;
    Node* n = *bucket;
    if (!n)
        return nullptr;
    while (reinterpret_cast<const UnkStrData_b218*>(n->key.data)->hash() != h) {
        n = n->chain;
        if (!n)
            return nullptr;
    }
    return n;
}

// A random record from the list stored under the id that hashes to `outer`; the list grows to hold the pick.
void* fn_7100e1cc70(void* self_, u64* outer) {
    UnkBillboardManager_b218* self = static_cast<UnkBillboardManager_b218*>(self_);
    UnkRecordNode_b218* n = FindByHash(&self->records, *outer);
    if (!n)
        return nullptr;
    u32 count = n->list.count;
    if (!count)
        return nullptr;
    u32 pick = (rand() & 0x7fff) % count;
    if (pick >= n->list.count)
        fn_7100e1d414(&n->list, pick + 1);
    return &n->list.items[pick];
}

// Drops the references a drop node holds and clears it.
__attribute__((noinline)) void fn_7100e1d328(UnkDropNode_b218* node) {
    for (u64 i = 0; i < node->count; ++i)
        node->items[i].id.Reset();
    node->count = 0;
    node->key.Reset();
}

// Destroys a billboard node: its elements, buffer and render item, and frees it.
__attribute__((noinline)) void fn_7100e1d264(UnkMap_b218<UnkBillboardNode_b218>* map, UnkBillboardNode_b218* node) {
    if (!node)
        return;
    if (node->elements) {
        delete[] node->elements;
        node->elements = nullptr;
    }
    if (node->field_0x20) {
        delete[] node->field_0x20;
        node->field_0x20 = nullptr;
    }
    if (node->item) {
        if (!node->field_0x30)
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(node->item), 0);
        node->field_0x30 = nullptr;
        node->item = nullptr;
    }
    node->count = 0;
    map->allocator->vf_0x48(node);
}

// Updates the elements of every enabled billboard and marks its render item.
void fn_7100e1bf68(void* self_, f32 dt) {
    UnkBillboardManager_b218* self = static_cast<UnkBillboardManager_b218*>(self_);
    for (UnkBillboardNode_b218* node = self->billboards.head; node; node = node->next) {
        if (!node->item || !node->enabled)
            continue;
        for (u64 i = 0; i < node->count; ++i)
            fn_7100744e4c(&node->elements[i], dt);
        UnkBBItem_b218* item = node->item;
        char* resource = node->field_0x20;
        void* target = item->field_0x128;
        fn_71001b7128(target, resource, node->count);
        item->field_0x113 = 1;
    }
}

// Releases the billboard with the given id (which must exist): its attached object is told, then the node is
// unlinked from its bucket and from the list of all nodes, destroyed and counted out.
void fn_7100e1c9d8(void* manager_, u32 id) {
    UnkMap_b218<UnkBillboardNode_b218>* map = &static_cast<UnkBillboardManager_b218*>(manager_)->billboards;
    u32 index = id % map->bucket_capacity;
    UnkBillboardNode_b218* found = map->buckets[index];
    while (found->id != id)
        found = found->chain;
    if (found->field_0x30)
        fn_71007302e4(static_cast<UnkCharClassBillboardLists_ovb0020*>(found->field_0x30),
                      reinterpret_cast<char*>(found->item));
    index = id % map->bucket_capacity;
    UnkBillboardNode_b218** bucket = map->buckets + index;
    UnkBillboardNode_b218* n = *bucket;
    if (!n)
        return;
    UnkBillboardNode_b218* before = nullptr;
    while (n->id != id) {
        before = n;
        n = n->chain;
        if (!n)
            return;
    }
    if (before)
        before->chain = n->chain;
    else
        *bucket = n->chain;
    if (map->head == n)
        map->head = n->next;
    if (map->tail == n)
        map->tail = n->prev;
    if (n->Prev())
        n->Prev()->Next() = n->Next();
    if (n->Next())
        n->Next()->Prev() = n->Prev();
    fn_7100e1d264(map, n);
    --map->count;
}

static inline u32 HashId(const CStrId* id) {
    const UnkStrData_b218* d = reinterpret_cast<const UnkStrData_b218*>(id->data);
    return d->hash_lo ^ d->hash_hi;
}

// Adds a record node for `key` holding a copy of `list` to the front of its bucket and the end of the node list.
__attribute__((noinline)) UnkRecordNode_b218* fn_7100e1d790(UnkMap_b218<UnkRecordNode_b218>* map, CStrId* key, UnkRecordList_b218* list) {
    UnkRecordNode_b218** bucket = nullptr;
    if (map->bucket_count) {
        u32 index = HashId(key) % map->bucket_capacity;
        bucket = map->buckets + index;
    }
    UnkRecordNode_b218* old = *bucket;
    UnkRecordNode_b218* n = static_cast<UnkRecordNode_b218*>(map->allocator->vf_0x40(sizeof(UnkRecordNode_b218), 8, map->flags));
    if (n) {
        new (&n->key) CStrId(*key);
        const UnkRecord_b218* src = list->items;
        UnkRecord_b218* dst = n->list.items;
        for (u64 i = 0; i < list->count; ++i, ++src, ++dst)
            fn_7100e1d8d8(dst, src);
        n->list.count = list->count;
        n->next = nullptr;
        n->prev = nullptr;
        n->chain = nullptr;
    }
    n->chain = old;
    *bucket = n;
    if (!map->head)
        map->head = n;
    else if (map->tail) {
        n->Prev() = map->tail;
        map->tail->Next() = n;
    }
    map->tail = n;
    ++map->count;
    return n;
}

// The same for a drop node.
__attribute__((noinline)) UnkDropNode_b218* fn_7100e1d620(UnkMap_b218<UnkDropNode_b218>* map, CStrId* key, UnkDropList_b218* list) {
    UnkDropNode_b218** bucket = nullptr;
    if (map->bucket_count) {
        u32 index = HashId(key) % map->bucket_capacity;
        bucket = map->buckets + index;
    }
    UnkDropNode_b218* old = *bucket;
    UnkDropNode_b218* n = static_cast<UnkDropNode_b218*>(map->allocator->vf_0x40(sizeof(UnkDropNode_b218), 8, map->flags));
    if (n) {
        new (&n->key) CStrId(*key);
        const UnkDrop_b218* src = list->items;
        for (u64 i = 0; i < list->count; ++i, ++src)
            new (&n->items[i]) UnkDrop_b218(*src);
        n->count = list->count;
        n->next = nullptr;
        n->prev = nullptr;
        n->chain = nullptr;
    }
    n->chain = old;
    *bucket = n;
    if (!map->head)
        map->head = n;
    else if (map->tail) {
        n->Prev() = map->tail;
        map->tail->Next() = n;
    }
    map->tail = n;
    ++map->count;
    return n;
}


// Appends a copy of `record` to the list stored under `key` (the list is created first when there is none).
void fn_7100e1cfec(void* self_, CStrId* key, UnkRecord_b218* record) {
    UnkMap_b218<UnkRecordNode_b218>* map = &static_cast<UnkBillboardManager_b218*>(self_)->records;
    UnkRecordNode_b218* n = nullptr;
    if (map->bucket_count) {
        u32 index = HashId(key) % map->bucket_capacity;
        UnkRecordNode_b218** bucket = map->buckets + index;
        if (bucket) {
            n = *bucket;
            while (n && n->key.data != key->data)
                n = n->chain;
        }
    }
    if (!n) {
        UnkRecordListStorage_b218 empty;
        empty.list.count = 0;
        n = fn_7100e1d790(map, key, &empty.list);
        fn_7100e1d414(&empty.list, 0);
        if (!n)
            return;
    }
    fn_7100e1d8d8(&n->list.items[n->list.count], record);
    ++n->list.count;
}

// A drop list that is not constructed (only its count is set).
union UnkDropListStorage_b218 {
    UnkDropList_b218 list;
    UnkDropListStorage_b218() {}
    ~UnkDropListStorage_b218() {}
};

// Appends a copy of the request to the list stored under the id (the list is created first when there is none).
void fn_7100e1cd34(void* self_, UnkDropTarget_ovb0208* target, UnkDropRequest_ovb0208* request_) {
    UnkMap_b218<UnkDropNode_b218>* map = &static_cast<UnkBillboardManager_b218*>(self_)->drops;
    CStrId* key = reinterpret_cast<CStrId*>(target);
    UnkDrop_b218* request = reinterpret_cast<UnkDrop_b218*>(request_);
    UnkDropNode_b218* n = nullptr;
    if (map->bucket_count) {
        u32 index = HashId(key) % map->bucket_capacity;
        UnkDropNode_b218** bucket = map->buckets + index;
        if (bucket) {
            n = *bucket;
            while (n && n->key.data != key->data)
                n = n->chain;
        }
    }
    if (!n) {
        UnkDropListStorage_b218 empty;
        empty.list.count = 0;
        n = fn_7100e1d620(map, key, &empty.list);
        for (u64 i = 0; i < empty.list.count; ++i)
            empty.list.items[i].id.Reset();
        if (!n)
            return;
    }
    u8* base = reinterpret_cast<u8*>(n->items);
    u64 offset = (u64)n->count * sizeof(UnkDrop_b218);
    new (base + offset) UnkDrop_b218(*request);
    ++n->count;
}

// Unlinks the node whose key hashes to `hash` from its bucket and from the node list; null when there is none.
template <class Node>
static inline __attribute__((always_inline)) Node* Unlink(UnkMap_b218<Node>* map, const u64* hash) {
    u32 index = HashU64(*hash) % map->bucket_capacity;
    Node** bucket = map->buckets + index;
    Node* n = *bucket;
    if (!n)
        return nullptr;
    Node* before = nullptr;
    while (reinterpret_cast<const UnkStrData_b218*>(n->key.data)->hash() != *hash) {
        before = n;
        n = n->chain;
        if (!n)
            return nullptr;
    }
    asm("" : "+r"(before));  // keeps &before->chain out of the search loop
    *(before ? &before->chain : bucket) = n->chain;
    if (map->head == n)
        map->head = n->next;
    if (map->tail == n)
        map->tail = n->prev;
    if (n->Prev())
        n->Prev()->Next() = n->Next();
    return n;
}

// Erases the drop node whose key hashes to `hash`; returns the node that followed it.
UnkDropNode_b218* fn_7100e1ceb8(UnkMap_b218<UnkDropNode_b218>* map, u64* hash) {
    UnkDropNode_b218* n = Unlink(map, hash);
    if (!n)
        return nullptr;
    UnkDropNode_b218* r = nullptr;
    if (n->Next()) {
        n->Next()->Prev() = n->Prev();
        r = n->Next();
    }
    fn_7100e1d328(n);
    map->allocator->vf_0x48(n);
    --map->count;
    return r;
}

// Erases the record node whose key hashes to `hash`; returns the node that followed it.
UnkRecordNode_b218* fn_7100e1d0b0(UnkMap_b218<UnkRecordNode_b218>* map, u64* hash) {
    UnkRecordNode_b218* n = Unlink(map, hash);
    if (!n)
        return nullptr;
    UnkRecordNode_b218* r = nullptr;
    if (n->Next()) {
        n->Next()->Prev() = n->Prev();
        r = n->Next();
    }
    fn_7100e1d414(&n->list, 0);
    n->key.Reset();
    map->allocator->vf_0x48(n);
    --map->count;
    return r;
}
