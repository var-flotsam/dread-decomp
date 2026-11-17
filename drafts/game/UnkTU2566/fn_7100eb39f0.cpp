// Unit 2566 (tools/tu_map.py; hint CMinimapManager::CTunableMinimapManager): the hashed set's insert of a
// new node for a key, whose value is four default minimap parts and an empty id. Names and layouts are
// placeholders.
#include <new>

#include "base/global/UnkBucketSet.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU2567.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkPoolIter;
using base::global::UnkStringData;

// As src/game/UnkTU2566/UnkTU2566_Minimap_ovb0200.cpp declares them.
struct UnkKeyObject_b0200 {
    u8 pad_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

struct UnkKeyRecord_b0200 {
    UnkKeyObject_b0200* key;
    u8 value[0x15f0];  // 0x8
    UnkKeyRecord_b0200* next;  // 0x15f8
};

struct UnkKeyBuckets_b0200 {
    UnkKeyRecord_b0200** data;
    u32 count;
    u32 capacity;
};

struct UnkKeyIter_b0200 {
    UnkKeyRecord_b0200* node;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    UnkKeyIter_b0200(const UnkKeyIter_b0200& o)
        : node(o.node), field_0x8(o.field_0x8), field_0x10(o.field_0x10), field_0x18(o.field_0x18) {}
};

// As src/game/CMinimapManager.cpp defines it: resizes a fixed vector of modifiers.
struct UnkMinimapModifiers;
void fn_7100ea9b04(UnkMinimapModifiers* self, u32 size);

// A minimap part (0x578 bytes) with two vectors of modifiers.
struct UnkMinimapPart_leb39f0 {
    u8 pad_0x0[0x60];
    u8 alpha_modifiers[0x28c];  // 0x60
    u8 size_modifiers[0x28c];   // 0x2ec

    ~UnkMinimapPart_leb39f0() {
        fn_7100ea9b04(reinterpret_cast<UnkMinimapModifiers*>(size_modifiers), 0);
        fn_7100ea9b04(reinterpret_cast<UnkMinimapModifiers*>(alpha_modifiers), 0);
    }
};
static_assert(sizeof(UnkMinimapPart_leb39f0) == 0x578, "UnkMinimapPart_leb39f0 size");

// Up to four parts in raw storage, the count first (a fixed-capacity vector).
struct UnkMinimapParts_leb39f0 {
    u32 count;
    alignas(4) u8 storage[4 * sizeof(UnkMinimapPart_leb39f0)];

    UnkMinimapParts_leb39f0() : count(0) {}
    UnkMinimapPart_leb39f0* data() { return reinterpret_cast<UnkMinimapPart_leb39f0*>(storage); }

    void Resize(u32 size) {
        if (count < size) {
            for (u32 i = count; i < size; ++i)
                fn_7100eb7680(&data()[i]);
        } else if (count != size) {
            for (u64 i = size; i < count; ++i)
                data()[i].~UnkMinimapPart_leb39f0();
        }
        count = size;
    }
};

struct UnkMinimapValue_leb39f0 {
    UnkMinimapParts_leb39f0 parts;
    UnkEmptyStrId id;  // 0x15e8

    UnkMinimapValue_leb39f0() { parts.Resize(4); }
};

struct UnkMinimapNode_leb39f0 {
    CStrId key;
    UnkMinimapValue_leb39f0 value;  // 0x8
    UnkMinimapNode_leb39f0* next;  // 0x15f8

    explicit UnkMinimapNode_leb39f0(const CStrId& key) : key(key), next(nullptr) {}
};
static_assert(__builtin_offsetof(UnkMinimapNode_leb39f0, next) == 0x15f8, "UnkMinimapNode_leb39f0 layout");

// The set (base/global/UnkBucketSet.h).
using UnkMinimapSet_leb39f0 = base::global::UnkBucketSet<UnkMinimapNode_leb39f0>;

UnkPoolIter fn_7100eb39f0(UnkKeyBuckets_b0200* buckets, const CStrId* key) {
    UnkMinimapSet_leb39f0* set = reinterpret_cast<UnkMinimapSet_leb39f0*>(buckets);
    ++set->version;
    UnkMinimapNode_leb39f0* node = static_cast<UnkMinimapNode_leb39f0*>(
        set->node_allocator->vf_0x40(sizeof(UnkMinimapNode_leb39f0), 8, set->node_flags));
    if (node)
        new (node) UnkMinimapNode_leb39f0(*key);
    u32 bucket;
    if (set->buckets.count) {
        const UnkKeyObject_b0200* object = reinterpret_cast<const UnkKeyObject_b0200*>(key->data);
        bucket = (object->hash_a ^ object->hash_b) % set->buckets.capacity;
    } else {
        bucket = -1;
    }
    u32& first = set->first_bucket;
    first = bucket < first ? bucket : first;
    if (first == bucket)
        set->first_node = node;
    UnkMinimapNode_leb39f0** head =
        reinterpret_cast<UnkMinimapNode_leb39f0**>(fn_7100eb3814(reinterpret_cast<UnkRecordBuckets*>(set), bucket));
    node->next = *head;
    *head = node;
    ++set->node_count;
    return UnkPoolIter(reinterpret_cast<char*>(node), nullptr, reinterpret_cast<char*>(set), bucket);
}
