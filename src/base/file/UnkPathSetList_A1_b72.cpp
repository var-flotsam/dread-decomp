// Unit 67 of tools/tu_map.py (no class hint): lists the file-path ids of a path set. The set is found
// under `path` in the owner's chained hash map at +0x10 (bucket = the string's 64-bit hash at +0x30,
// halves xored, modulo the bucket capacity; next node at +0x40); each index it holds names an id in
// the owner's array at +0x88, and a copy of that id is appended to `out`. False when there is no
// such set. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkTU63.h"
#include "game/UnkTU67.h"
#include "types.h"

using base::UnkStrIdMemberOf;
using base::global::CFilePathStrId;

// UnkListedPath and UnkFilePathList as src/base/file/CPackManagerDevice.cpp defines them.
struct UnkListedPath {
    UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath* next;
};
struct UnkFilePathList {
    void** vtable;
    u32 count;
    UnkListedPath* head;
    UnkListedPath* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        for (UnkListedPath* n = head; n;) {
            UnkListedPath* next = n->next;
            n->id.~UnkStrIdMemberOf();
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

// fn_710008c174's signature as src/game/CSubAreaManager.cpp declares it.
using UnkPathIdMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;
UnkPathIdMember* fn_710008c174(base::UnkVector<UnkPathIdMember>*, u32);

struct UnkPathSetNode_A1b72 {
    base::global::UnkStringData* key;
    base::UnkVector<u32> indices;  // 0x8
    u8 pad_0x30[0x10];
    UnkPathSetNode_A1b72* chain;  // 0x40
};

struct UnkHashView_A1b72 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkPathSetOwner_A1b72 {
    u8 pad_0x0[0x10];
    UnkPathSetNode_A1b72** buckets;  // 0x10
    u32 bucket_count;                // 0x18
    u32 bucket_capacity;             // 0x1c
    u8 pad_0x20[0x68];
    base::UnkVector<UnkPathIdMember> ids;  // 0x88
};

// The signature src/base/file/CPackManagerDevice.cpp declares.
bool fn_710008c3bc(void* base_object, const CFilePathStrId& path, UnkFilePathList* out) {
    auto* self = static_cast<UnkPathSetOwner_A1b72*>(base_object);
    if (!self->bucket_count)
        return false;
    base::global::UnkStringData* key = path.data;
    u64 h = reinterpret_cast<const UnkHashView_A1b72*>(key)->hash;
    u32 b = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
    asm("" : "+r"(b));  // the original computes the index before loading the buckets
    UnkPathSetNode_A1b72** bucket = &self->buckets[b];
    if (!bucket)
        return false;
    UnkPathSetNode_A1b72* node = *bucket;
    for (; node; node = node->chain) {
        if (node->key == key)
            break;
    }
    if (!node)
        return false;
    u32 count = node->indices.count;
    for (u32 i = 0; i != count; ++i) {
        UnkPathIdMember* id = fn_710008c174(&self->ids, *fn_710008c504(&node->indices, i));
        auto* n = static_cast<UnkListedPath*>(out->allocator->vf_0x40(0x10, 8, out->flags));
        if (!n)
            continue;
        base::global::UnkStringData* d = id->data;
        n->id.data = d;
        if (&g_unk_7101d47200->empty != d)
            ++d->ref_count;
        n->next = nullptr;
        u32 c;
        if (out->count == 0) {
            out->head = n;
            out->tail = n;
            c = 1;
        } else {
            __builtin_memcpy(&out->tail->next, &n, sizeof(n));
            n->next = nullptr;
            out->tail = n;
            c = out->count + 1;
        }
        out->count = c;
    }
    return true;
}
