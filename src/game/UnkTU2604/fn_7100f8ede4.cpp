// Unit 2604: erases the node a string id names from a map from string id to string id
// (game/UnkStrIdToIdMap.h): the node is unlinked from its bucket chain and from the list of all nodes, its
// value and key are released and it is freed; returns the node before it in the list. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkStrIdToIdMap.h"
#include "game/UnkTU2604.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkHashedStr_lf8ede4 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

UnkStrIdToIdNode* fn_7100f8ede4(UnkStrIdToIdMap* self, const CStrId* key) {
    UnkStringData* k = key->data;
    const UnkHashedStr_lf8ede4* h = reinterpret_cast<const UnkHashedStr_lf8ede4*>(k);
    u32 b = (h->hash_lo ^ h->hash_hi) % self->divisor;
    UnkStrIdToIdNode** slot = self->buckets + b;
    UnkStrIdToIdNode* n = *slot;
    if (!n)
        return nullptr;
    UnkStrIdToIdNode* before = nullptr;
    if (n->key.data != k) {
        do {
            before = n;
            n = n->chain;
            if (!n)
                return nullptr;
        } while (n->key.data != k);
    }
    *(before ? &before->chain : slot) = n->chain;
    if (self->head == n)
        self->head = n->Prev();
    if (self->tail == n)
        self->tail = n->Next();
    if (n->Next())
        n->Next()->Prev() = n->Prev();
    UnkStrIdToIdNode* result = nullptr;
    if (n->Prev()) {
        n->Prev()->Next() = n->Next();
        result = n->Prev();
    }
    n->value.~CStrId();
    n->key.~CStrId();
    self->allocator->vf_0x48(n);
    --self->count;
    return result;
}
