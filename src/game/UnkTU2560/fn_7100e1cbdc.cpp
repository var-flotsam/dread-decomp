// Unit 2560 (the billboard manager, 0xe0 bytes, built by fn_7100e1bca4): its two chained hash maps keyed by
// string id. The map at +0x50 holds lists of up to 16 0x28-byte drop requests (nodes of 0x2a8 bytes), the map
// at +0x98 lists of up to 16 0xb0-byte records (nodes of 0xb28 bytes). Lookup by the id's 64-bit hash, adding a
// request or record under an id, random pick, erase, node copy and destruction. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkArray_b218.h"
#include "game/UnkMap_b218.h"
#include "game/UnkPartArray_b218.h"
#include "game/UnkStrData_b218.h"
#include "game/UnkTU2560.h"
#include "types.h"
#include "game/UnkTU83.h"

using base::global::CStrId;

extern "C" int rand();

struct UnkPart48;
struct UnkUnit2560Record;
struct UnkDropTarget_ovb0208;
struct UnkDropRequest_ovb0208;

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

struct UnkDrop_b218_le1cbdc {
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
static_assert(sizeof(UnkDrop_b218_le1cbdc) == 0x28, "drop layout");

struct UnkDropList_b218_le1cbdc {
    u32 count;
    UnkDrop_b218_le1cbdc items[16];
};

#pragma pack(pop)

struct UnkRecordNode_b218_le1cbdc {
    CStrId key;
    UnkRecordList_b218 list;
    UnkRecordNode_b218_le1cbdc* next;   // 0xb10
    UnkRecordNode_b218_le1cbdc* prev;   // 0xb18
    UnkRecordNode_b218_le1cbdc* chain;  // 0xb20
};
static_assert(sizeof(UnkRecordNode_b218_le1cbdc) == 0xb28, "node layout");

struct UnkDropNode_b218_le1cbdc {
    CStrId key;
    UnkDropList_b218_le1cbdc list;
    UnkDropNode_b218_le1cbdc* next;   // 0x290
    UnkDropNode_b218_le1cbdc* prev;   // 0x298
    UnkDropNode_b218_le1cbdc* chain;  // 0x2a0
};
static_assert(sizeof(UnkDropNode_b218_le1cbdc) == 0x2a8, "node layout");

struct UnkBillboardManager_b218_le1cbdc {
    u8 pad_0x0[0x50];
    UnkMap_b218<UnkDropNode_b218_le1cbdc> drops;       // 0x50
    UnkMap_b218<UnkRecordNode_b218_le1cbdc> records;   // 0x98
};
static_assert(sizeof(UnkBillboardManager_b218_le1cbdc) == 0xe0, "manager layout");

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
// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void* fn_7100e1cbdc(void* self_, u64* outer, void* inner_) {
    UnkBillboardManager_b218_le1cbdc* self = static_cast<UnkBillboardManager_b218_le1cbdc*>(self_);
    UnkRecordNode_b218_le1cbdc* n = FindByHash(&self->records, *outer);
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
