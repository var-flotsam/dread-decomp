// Unit 2640: a map from string ids to named id maps, looked up by key; a missing key gets a new empty map
// (built out of line, copied into the node), named after the key. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2640.h"
#include "types.h"

using base::global::CStrId;

// The empty value's constructor and the value's copy constructor, as their definitions in this unit have them.
template <class Node>
struct UnkIdMapOf_b0213;
struct UnkIdArrayNode_b0213;
void fn_7100feee0c(UnkIdMapOf_b0213<UnkIdArrayNode_b0213>* self);
void fn_7100fed9dc(void* dst, const void* src);

// A string id and a hash map of Node nodes (chained in buckets and linked in insertion order).
template <class Node>
struct UnkIdMap_lfeeb84 {
    CStrId id;
    base::IAllocator* allocator;  // 0x8
    u32 flags;
    u32 count;
    base::global::CRntVector<Node*> buckets;  // 0x18
    Node* tail;  // 0x40
    Node* head;  // 0x48

    // The key's bucket, or none while there are no buckets.
    Node** Slot(const CStrId& key) {
        Node** slot = nullptr;
        if (buckets.count)
            slot = &buckets.data[key.Hash() % buckets.capacity];
        return slot;
    }

    Node* Find(const CStrId& key) {
        Node** slot = Slot(key);
        if (!slot)
            return nullptr;
        for (Node* n = *slot; n; n = n->chain)
            if (n->key == key)
                return n;
        return nullptr;
    }

    // Destroys the nodes and empties the buckets.
    void Clear() {
        if (count) {
            for (Node* n = head; n;) {
                Node* next = n->next;
                fn_7101005d8c(n);
                allocator->vf_0x48(n);
                n = next;
            }
            __builtin_memset(buckets.data, 0, (u64)buckets.capacity * sizeof(Node*));
            count = 0;
            tail = nullptr;
            head = nullptr;
        }
    }

    ~UnkIdMap_lfeeb84() {
        Clear();
        buckets.Free();
    }
};

// The inner map's nodes; only the link a clear follows is laid out.
struct UnkInnerNode_lfeeb84 {
    CStrId key;
    CStrId id;
    u8 pad_0x10[0x28];
    UnkInnerNode_lfeeb84* next;  // 0x38
};

using UnkIdMapValue_lfeeb84 = UnkIdMap_lfeeb84<UnkInnerNode_lfeeb84>;
static_assert(sizeof(UnkIdMapValue_lfeeb84) == 0x50);

struct UnkNode_lfeeb84 {
    CStrId key;
    UnkIdMapValue_lfeeb84 value;
    UnkNode_lfeeb84* next;  // 0x58
    UnkNode_lfeeb84* prev;  // 0x60
    UnkNode_lfeeb84* chain;  // 0x68
};
static_assert(sizeof(UnkNode_lfeeb84) == 0x70);

struct UnkMap_lfeeb84 : UnkIdMap_lfeeb84<UnkNode_lfeeb84> {
    // Adds a node holding a copy of the key and the value, at the front of its bucket and the end of the list.
    UnkNode_lfeeb84* Insert(const CStrId& key, const UnkIdMapValue_lfeeb84& value) {
        UnkNode_lfeeb84** slot = nullptr;
        if (buckets.count)
            slot = &buckets.data[key.Hash() % buckets.capacity];
        UnkNode_lfeeb84* chained = *slot;
        auto* n = static_cast<UnkNode_lfeeb84*>(allocator->vf_0x40(sizeof(UnkNode_lfeeb84), 8, flags));
        if (n) {
            new (&n->key) CStrId(key);
            fn_7100fed9dc(&n->value, &value);
            n->chain = nullptr;
            n->prev = nullptr;
            n->next = nullptr;
        }
        n->chain = chained;
        *slot = n;
        if (!head) {
            head = n;
        } else if (tail) {
            n->prev = tail;
            tail->next = n;
        }
        tail = n;
        ++count;
        return n;
    }
};

// An empty value, built by its out-of-line constructor and destroyed inline.
struct UnkEmptyValue_lfeeb84 {
    alignas(UnkIdMapValue_lfeeb84) u8 storage[sizeof(UnkIdMapValue_lfeeb84)];

    UnkEmptyValue_lfeeb84() { fn_7100feee0c(reinterpret_cast<UnkIdMapOf_b0213<UnkIdArrayNode_b0213>*>(storage)); }
    ~UnkEmptyValue_lfeeb84() { Get().~UnkIdMapValue_lfeeb84(); }
    UnkIdMapValue_lfeeb84& Get() { return *reinterpret_cast<UnkIdMapValue_lfeeb84*>(storage); }
};

UnkIdMapValue_lfeeb84* fn_7100feeb84(UnkMap_lfeeb84* self, const CStrId* key) {
    if (self->Find(*key))
        return &self->Find(*key)->value;
    UnkNode_lfeeb84* node;
    {
        UnkEmptyValue_lfeeb84 empty;
        node = self->Insert(*key, empty.Get());
    }
    node->value.id = *key;
    return &node->value;
}
