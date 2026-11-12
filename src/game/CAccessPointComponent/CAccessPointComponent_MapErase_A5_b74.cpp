// Copy of fn_7100fedf08 (src/game/UnkStrIdMapErase_A6_b73.cpp): erasing a string-id key from a chained hash map
// whose nodes keep the bucket chain at +0x40 and the list links at +0x30 / +0x38; the node is destroyed by
// fn_7100a73ec8, freed through the map's allocator, and the node after it in the list is returned.
// CAccessPointComponent unit (tools/tu_map.py, unit 1581). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkNamedIds;
void fn_7100a73ec8(UnkNamedIds* self);

struct UnkAccessPointMap_A5_b74;

namespace {

struct UnkHashView_A5b74 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkEraseMap_A5b74 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    char** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
    u8 pad_0x20[0x18];
    char* tail;
    char* head;
};

static inline __attribute__((always_inline)) char*& Link(char* n, u32 off) {
    return *reinterpret_cast<char**>(n + off);
}

}  // namespace

void* fn_7100a75858(UnkAccessPointMap_A5_b74* map, const CStrId* key) {
    constexpr u32 Chain = 0x40;
    UnkEraseMap_A5b74* self = reinterpret_cast<UnkEraseMap_A5b74*>(map);
    auto* k = key->data;
    u64 h = reinterpret_cast<const UnkHashView_A5b74*>(k)->hash;
    u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
    char** bucket = &self->buckets[i];
    char* n = *bucket;
    if (!n)
        return nullptr;
    char* before = nullptr;
    while (*reinterpret_cast<decltype(k)*>(n) != k) {
        before = n;
        n = Link(n, Chain);
        if (!n)
            return nullptr;
    }
    asm("" : "+r"(before));
    *(before ? &Link(before, Chain) : bucket) = Link(n, Chain);
    if (self->head == n)
        self->head = Link(n, Chain - 0x10);
    if (self->tail == n)
        self->tail = Link(n, Chain - 8);
    if (Link(n, Chain - 8))
        Link(Link(n, Chain - 8), Chain - 0x10) = Link(n, Chain - 0x10);
    char* r = nullptr;
    if (Link(n, Chain - 0x10)) {
        Link(Link(n, Chain - 0x10), Chain - 8) = Link(n, Chain - 8);
        r = Link(n, Chain - 0x10);
    }
    fn_7100a73ec8(reinterpret_cast<UnkNamedIds*>(n));
    self->allocator->vf_0x48(n);
    --self->count;
    return r;
}
