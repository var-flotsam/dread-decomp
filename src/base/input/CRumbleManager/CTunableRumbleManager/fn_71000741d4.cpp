// Copies of base::refl_StrIdHashErase (include/base/UnkCloneHelpers.h; fn_71002430e8 in src/clones/)
// for maps keyed by CFilePathStrId-pool string ids (g_unk_7101d47200): fn_71000741d4, fn_71011aa6a0.
// Erase unlinks the node for `key` from its bucket chain and the list, releases the key and frees the
// node, returning the next node in list order. Names and fields are placeholders.
#include "base/UnkCloneHelpers.h"

using FileKey = base::UnkStrIdMemberOf<g_unk_7101d47200>;

struct UnkFileHashNode {
    FileKey key;
    u8 unk_0x8[0x8];
    // Union members: the original reloads them after each link store.
    union {
        UnkFileHashNode* next;  // list order
    };
    union {
        UnkFileHashNode* prev;
    };
    UnkFileHashNode* chain;  // bucket chain
};

struct UnkFileHashMap {
    base::UnkAllocator* allocator;
    u32 field_0x8;
    u32 count;
    UnkFileHashNode** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 unk_0x20[0x18];
    UnkFileHashNode* tail;
    UnkFileHashNode* head;
};

static __attribute__((always_inline)) inline UnkFileHashNode* Erase(UnkFileHashMap* m, const FileKey* key) {
    using Node = UnkFileHashNode;
    base::global::UnkStringData* k = key->data;
    const char* raw = reinterpret_cast<const char*>(k);
    u32 h = *reinterpret_cast<const u32*>(raw + 0x30) ^ *reinterpret_cast<const u32*>(raw + 0x34);
    u32 i = h % m->bucket_count;
    Node** slot = m->buckets + i;
    Node* n = *slot;
    if (!n)
        return nullptr;
    Node* prev = nullptr;
    if (n->key.data != k) {
        do {
            prev = n;
            n = n->chain;
            if (!n)
                return nullptr;
        } while (n->key.data != k);
    }
    *(prev ? &prev->chain : slot) = n->chain;
    if (m->head == n)
        m->head = n->next;
    if (m->tail == n)
        m->tail = n->prev;
    if (n->prev)
        n->prev->next = n->next;
    if (n->next)
        n->next->prev = n->prev;
    Node* next = n->next;
    n->key.~FileKey();
    m->allocator->vf_0x48(n);
    --m->count;
    return next;
}

void* fn_71000741d4(UnkFileHashMap* m, const FileKey* key) { return Erase(m, key); }
void* fn_71011aa6a0(UnkFileHashMap* m, const FileKey* key) { return Erase(m, key); }
