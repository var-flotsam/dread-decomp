// Unit 2640: builds a named map of string ids to named maps from another: the empty name and an 11-bucket
// map, then the other's name taken and each of its nodes inserted anew with a copy of its value
// (fn_7100fed9dc). Names and layouts are placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

void fn_7100fed9dc(void* dst, const void* src);

// A node of the outer map: the key, the 0x50-byte named map it maps to, the list links and the bucket chain.
struct UnkNamedMapNode_lfee558 {
    CStrId key;
    u8 value[0x50];                  // 0x8, copied by fn_7100fed9dc
    UnkNamedMapNode_lfee558* next;   // 0x58
    UnkNamedMapNode_lfee558* prev;   // 0x60
    UnkNamedMapNode_lfee558* chain;  // 0x68

    UnkNamedMapNode_lfee558(const UnkNamedMapNode_lfee558& other) : key(other.key) {
        fn_7100fed9dc(value, other.value);
        next = nullptr;
        prev = nullptr;
        chain = nullptr;
    }
};
static_assert(sizeof(UnkNamedMapNode_lfee558) == 0x70, "UnkNamedMapNode_lfee558 layout");

struct UnkNamedMap_lfee558 {
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
inline void InsertCopy_lfee558(UnkChainedMap* map, const UnkNamedMapNode_lfee558* node) {
    UnkMapNode** bucket = nullptr;
    if (map->buckets.count)
        bucket = map->buckets.data + node->key.Hash() % map->buckets.capacity;
    UnkMapNode* old = *bucket;
    auto* copy = static_cast<UnkNamedMapNode_lfee558*>(
        map->allocator->vf_0x40(sizeof(UnkNamedMapNode_lfee558), 8, map->flags));
    if (copy)
        new (copy) UnkNamedMapNode_lfee558(*node);
    copy->chain = reinterpret_cast<UnkNamedMapNode_lfee558*>(old);
    *bucket = reinterpret_cast<UnkMapNode*>(copy);
    // The list's end link: the first node's, or the last node's next.
    UnkMapNode** link = &map->tail;
    if (*link) {
        UnkMapNode* last = map->head;
        if (!last) {
            // The list has a first node but no last one: the new node only becomes the last.
            map->head = reinterpret_cast<UnkMapNode*>(copy);
            ++map->count;
            return;
        }
        copy->prev = reinterpret_cast<UnkNamedMapNode_lfee558*>(last);
        link = reinterpret_cast<UnkMapNode**>(&reinterpret_cast<UnkNamedMapNode_lfee558*>(map->head)->next);
    }
    *link = reinterpret_cast<UnkMapNode*>(copy);
    map->head = reinterpret_cast<UnkMapNode*>(copy);
    ++map->count;
}

void fn_7100fee558(void* dst, const void* src) {
    auto* self = static_cast<UnkNamedMap_lfee558*>(dst);
    auto* other = static_cast<UnkNamedMap_lfee558*>(const_cast<void*>(src));
    InitEmptyStrId(&self->name);
    new (&self->map) UnkChainedMap();
    self->map.ResizeBuckets(11);
    self->name.ReleaseAndTakeReloading(other->name);
    for (auto* node = reinterpret_cast<UnkNamedMapNode_lfee558*>(other->map.tail); node; node = node->next)
        InsertCopy_lfee558(&self->map, node);
}
