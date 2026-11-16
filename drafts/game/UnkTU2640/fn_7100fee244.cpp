// Unit 2640: the copy constructor of a named map of string ids to named maps: the empty name and an 11-bucket
// map, then the other's name copied and each of its nodes inserted anew with a copy of its value
// (fn_7100fed7b0). Names and layouts are placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

void fn_7100fed7b0(void* dst, const void* src);

// A node of the outer map: the key, the 0x50-byte named map it maps to, the list links and the bucket chain.
struct UnkNamedMapNode_lfee244 {
    CStrId key;
    u8 value[0x50];                  // 0x8, copied by fn_7100fed7b0
    UnkNamedMapNode_lfee244* next;   // 0x58
    UnkNamedMapNode_lfee244* prev;   // 0x60
    UnkNamedMapNode_lfee244* chain;  // 0x68

    UnkNamedMapNode_lfee244(const UnkNamedMapNode_lfee244& other) : key(other.key) {
        fn_7100fed7b0(value, other.value);
        next = nullptr;
        prev = nullptr;
        chain = nullptr;
    }
};
static_assert(sizeof(UnkNamedMapNode_lfee244) == 0x70, "UnkNamedMapNode_lfee244 layout");

struct UnkNamedMap_lfee244 {
    CStrId name;
    UnkChainedMap map;  // 0x8
};

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

// Inserts a copy of `node` at the head of its bucket and the end of the node list.
inline void InsertCopy_lfee244(UnkChainedMap* map, const UnkNamedMapNode_lfee244* node) {
    UnkMapNode** bucket = nullptr;
    if (map->buckets.count)
        bucket = map->buckets.data + node->key.Hash() % map->buckets.capacity;
    UnkMapNode* old = *bucket;
    auto* copy = static_cast<UnkNamedMapNode_lfee244*>(
        map->allocator->vf_0x40(sizeof(UnkNamedMapNode_lfee244), 8, map->flags));
    if (copy)
        new (copy) UnkNamedMapNode_lfee244(*node);
    copy->chain = reinterpret_cast<UnkNamedMapNode_lfee244*>(old);
    *bucket = reinterpret_cast<UnkMapNode*>(copy);
    // The new node becomes the first when there is none, else follows the last.
    if (!map->tail) {
        map->tail = reinterpret_cast<UnkMapNode*>(copy);
    } else if (UnkMapNode* last = map->head) {
        // Linked through a plain pointer slot, so the last node is read again after it.
        UnkNamedMapNode_lfee244*& prev = copy->prev;
        prev = reinterpret_cast<UnkNamedMapNode_lfee244*>(last);
        reinterpret_cast<UnkNamedMapNode_lfee244*>(map->head)->next = copy;
    }
    map->head = reinterpret_cast<UnkMapNode*>(copy);
    ++map->count;
}

void fn_7100fee244(void* dst, const void* src) {
    auto* self = static_cast<UnkNamedMap_lfee244*>(dst);
    auto* other = static_cast<const UnkNamedMap_lfee244*>(src);
    InitEmptyStrId(&self->name);
    new (&self->map) UnkChainedMap();
    self->map.ResizeBuckets(11);
    self->name = other->name;
    for (auto* node = reinterpret_cast<UnkNamedMapNode_lfee244*>(other->map.tail); node; node = node->next)
        InsertCopy_lfee244(&self->map, node);
}
