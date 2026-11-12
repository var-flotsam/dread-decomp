// Copies of fn_71000d6de0 (src/base/reflection/UnkReflValueMap_A1_b72.cpp) and fn_710024536c
// (src/game/CActor/CActor_PtrMapEraseNode_A1_b72.cpp): erasing a key from a chained hash map keyed
// by string ids (bucket = the two words at +0x30 of the id's data xored, modulo the capacity at
// +0x1c). A node holds the key at 0, its list links at Chain - 0x10 / Chain - 8 and the bucket chain
// at Chain. The first family releases the value at +8 and the key; the second destroys the whole
// node. Both free the node through the map's allocator and return the node after it in the list.
// The maps are the reflected-erase maps of src/game/UnkReflMapRemove_b41.cpp,
// src/game/sound/CMusicManager/CMusicManager_b41.cpp and src/game/CLightManager/CLightManager_b41.cpp.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "game/UnkTU2604.h"
#include "game/UnkTU2640.h"
#include "types.h"

using base::global::CStrId;

template <u32 NextOffset>
struct UnkStrIdMap_b41a3;
template <u32 NextOffset>
struct UnkMusicMap_b41a3;
struct UnkLightMap_b41a3;

namespace {

struct UnkHashView_A6b73 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkEraseMap_A6b73 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;     // 0xc
    char** buckets;  // 0x10
    u32 bucket_count;     // 0x18
    u32 bucket_capacity;  // 0x1c
    u8 pad_0x20[0x18];
    char* tail;  // 0x38
    char* head;  // 0x40
};

template <u32 Chain>
static inline __attribute__((always_inline)) char*& Link(char* n, u32 off) {
    return *reinterpret_cast<char**>(n + off);
}

// KeyAndValue: release the value at +8 then the key; otherwise destroy the node.
template <u32 Chain, bool KeyAndValue, typename F>
static inline __attribute__((always_inline)) void* Erase(void* map, const CStrId* key, F destroy) {
    UnkEraseMap_A6b73* self = static_cast<UnkEraseMap_A6b73*>(map);
    auto* k = key->data;
    u64 h = reinterpret_cast<const UnkHashView_A6b73*>(k)->hash;
    u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
    asm("" : "+r"(i));  // the original computes the index before loading the buckets
    char** bucket = &self->buckets[i];
    char* n = *bucket;
    if (!n)
        return nullptr;
    char* before = nullptr;
    while (*reinterpret_cast<decltype(k)*>(n) != k) {
        before = n;
        n = Link<Chain>(n, Chain);
        if (!n)
            return nullptr;
    }
    asm("" : "+r"(before));  // keeps &before->chain out of the search loop
    *(before ? &Link<Chain>(before, Chain) : bucket) = Link<Chain>(n, Chain);
    if (self->head == n)
        self->head = Link<Chain>(n, Chain - 0x10);
    if (self->tail == n)
        self->tail = Link<Chain>(n, Chain - 8);
    if (Link<Chain>(n, Chain - 8))
        Link<Chain>(Link<Chain>(n, Chain - 8), Chain - 0x10) = Link<Chain>(n, Chain - 0x10);
    char* r = nullptr;
    if (Link<Chain>(n, Chain - 0x10)) {
        Link<Chain>(Link<Chain>(n, Chain - 0x10), Chain - 8) = Link<Chain>(n, Chain - 8);
        r = Link<Chain>(n, Chain - 0x10);
    }
    if (KeyAndValue) {
        destroy(n + 8);
        reinterpret_cast<base::UnkStrIdMember*>(n)->~UnkStrIdMemberOf();
    } else {
        destroy(n);
    }
    self->allocator->vf_0x48(n);
    --self->count;
    return r;
}

}  // namespace

void fn_7100fed6cc(char*);
void fn_7100e0da38(void*);

void* fn_7100f796b8(UnkStrIdMap_b41a3<0x3c0>* map, const CStrId* key) {
    return Erase<0x3c0, true>(map, key, [](char* p) { fn_7100d8f9e4(p); });
}

void* fn_7100f87480(UnkMusicMap_b41a3<0x80>* map, const CStrId* key) {
    return Erase<0x80, true>(map, key, [](char* p) { fn_7100d90304(p); });
}

void* fn_7100f89fd8(UnkMusicMap_b41a3<0x68>* map, const CStrId* key) {
    return Erase<0x68, true>(map, key, [](char* p) { fn_7100fee034(p); });
}

void* fn_7100f94d0c(UnkStrIdMap_b41a3<0x90>* map, const CStrId* key) {
    return Erase<0x90, true>(map, key, [](char* p) { fn_7100f8da4c(p); });
}

void* fn_7100feefa0(UnkStrIdMap_b41a3<0x68>* map, const CStrId* key) {
    return Erase<0x68, true>(map, key, [](char* p) { fn_7100fed6cc(p); });
}

void* fn_7100e0b52c(UnkLightMap_b41a3* map, const CStrId* key) {
    return Erase<0xa8, false>(map, key, [](char* p) { fn_7100e0da38(p); });
}

void* fn_7100fedf08(UnkStrIdMap_b41a3<0x48>* map, const CStrId* key) {
    return Erase<0x48, false>(map, key, [](char* p) { fn_7101005d8c(p); });
}
