// Unit 2560 (the billboard manager, 0xe0 bytes, built by fn_7100e1bca4): its two chained hash maps keyed by
// string id. The map at +0x50 holds lists of up to 16 0x28-byte drop requests (nodes of 0x2a8 bytes), the map
// at +0x98 lists of up to 16 0xb0-byte records (nodes of 0xb28 bytes). Lookup by the id's 64-bit hash, adding a
// request or record under an id, random pick, erase, node copy and destruction. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern "C" int rand();

struct UnkPart48;
void fn_71000cbae4(UnkPart48* self, const UnkPart48* other);
struct UnkUnit2560Record;
void fn_7100e1d538(UnkUnit2560Record* self);
struct UnkDropTarget_ovb0208;
struct UnkDropRequest_ovb0208;

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
};
static_assert(sizeof(UnkDrop_b218) == 0x28, "drop layout");

struct UnkDropList_b218 {
    u32 count;
    UnkDrop_b218 items[16];
};

#pragma pack(pop)

struct UnkRecordNode_b218 {
    CStrId key;
    UnkRecordList_b218 list;
    UnkRecordNode_b218* next;   // 0xb10
    UnkRecordNode_b218* prev;   // 0xb18
    UnkRecordNode_b218* chain;  // 0xb20
};
static_assert(sizeof(UnkRecordNode_b218) == 0xb28, "node layout");

struct UnkDropNode_b218 {
    CStrId key;
    UnkDropList_b218 list;
    UnkDropNode_b218* next;   // 0x290
    UnkDropNode_b218* prev;   // 0x298
    UnkDropNode_b218* chain;  // 0x2a0
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

struct UnkBillboardManager_b218 {
    u8 pad_0x0[0x50];
    UnkMap_b218<UnkDropNode_b218> drops;       // 0x50
    UnkMap_b218<UnkRecordNode_b218> records;   // 0x98
};
static_assert(sizeof(UnkBillboardManager_b218) == 0xe0, "manager layout");

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

// The entry whose id hashes to `inner` in the list stored under the id that hashes to `outer`.
void* fn_7100e1cbdc(void* self_, u64* outer, void* inner_) {
    UnkBillboardManager_b218* self = static_cast<UnkBillboardManager_b218*>(self_);
    UnkRecordNode_b218* n = FindByHash(&self->records, *outer);
    if (!n)
        return nullptr;
    u64 size = (u64)n->list.count * sizeof(UnkRecord_b218);
    asm("" : "+r"(size));  // the original tests the byte size on its own
    if (!size)
        return nullptr;
    u64 bytes = (u64)n->list.count * sizeof(UnkRecord_b218);
    u8* it = reinterpret_cast<u8*>(n->list.items);
    u64 want = *static_cast<u64*>(inner_);
    do {
        if ((*reinterpret_cast<UnkStrData_b218**>(it))->hash() == want)
            return it;
        it += sizeof(UnkRecord_b218);
        bytes -= sizeof(UnkRecord_b218);
    } while (bytes);
    return nullptr;
}

