// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): creates the object kept under a string
// id in the map at +0x2290, deleting and erasing any object already there, and returns it. Names and layouts
// are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

// The object created (0x50 bytes; fn_7101051530 constructs it).
struct UnkIdObject_df1ff4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    u8 pad_0x8[0x8];
    CStrId id;  // 0x10
    u8 pad_0x18[0x38];
};
static_assert(sizeof(UnkIdObject_df1ff4) == 0x50, "layout");

void fn_7101051530(UnkIdObject_df1ff4* self);

struct UnkIdHashed_df1ff4 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkIdNode_df1ff4 {
    CStrId key;
    UnkIdObject_df1ff4* value;  // 0x8
    UnkIdNode_df1ff4* next;     // 0x10
    UnkIdNode_df1ff4* prev;     // 0x18
    UnkIdNode_df1ff4* chain;    // 0x20

    UnkIdNode_df1ff4(const CStrId& k, UnkIdObject_df1ff4* v)
        : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkIdNode_df1ff4*& Next() { return next; }
    UnkIdNode_df1ff4*& Prev() { return prev; }
};

// A hash map by string id whose nodes are also kept in a list.
struct UnkIdMap_df1ff4 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;  // 0xc
    UnkIdNode_df1ff4** buckets;
    u32 bucket_count;
    u32 divisor;
    u8 pad_0x20[0x18];
    UnkIdNode_df1ff4* tail;  // 0x38
    UnkIdNode_df1ff4* head;  // 0x40

    UnkIdNode_df1ff4* Find(const CStrId& key) {
        UnkIdNode_df1ff4** bucket = nullptr;
        if (bucket_count != 0) {
            const UnkIdHashed_df1ff4* h = reinterpret_cast<const UnkIdHashed_df1ff4*>(key.data);
            u32 i = (h->hash_lo ^ h->hash_hi) % divisor;
            bucket = buckets + i;
        }
        if (!bucket)
            return nullptr;
        for (UnkIdNode_df1ff4* n = *bucket; n; n = n->chain)
            if (n->key.data == key.data)
                return n;
        return nullptr;
    }
    void Insert(const CStrId& key, UnkIdObject_df1ff4* value) {
        UnkIdNode_df1ff4** bucket = nullptr;
        if (bucket_count != 0) {
            const UnkIdHashed_df1ff4* h = reinterpret_cast<const UnkIdHashed_df1ff4*>(key.data);
            u32 i = (h->hash_lo ^ h->hash_hi) % divisor;
            bucket = buckets + i;
        }
        UnkIdNode_df1ff4* old = *bucket;
        auto* node = static_cast<UnkIdNode_df1ff4*>(allocator->vf_0x40(sizeof(UnkIdNode_df1ff4), 8, flags));
        if (node)
            new (node) UnkIdNode_df1ff4(key, value);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->Prev() = tail;
            tail->Next() = node;
        }
        tail = node;
        ++count;
    }
};

// Erases a node (its key) from the map (generated: src/clones/).
UnkIdNode_df1ff4* fn_7100df2284(UnkIdMap_df1ff4* map, const CStrId* key);

// The game manager's members this uses (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerIds_df1ff4 {
    u8 pad_0x0[0x2290];
    UnkIdMap_df1ff4 field_0x2290;
};

void* fn_7100df1ff4(UnkGameManager* manager, const CStrId& id) {
    auto* self = reinterpret_cast<UnkGameManagerIds_df1ff4*>(manager);
    if (UnkIdNode_df1ff4* node = self->field_0x2290.Find(id)) {
        if (node->value)
            node->value->vf_0x10();
        fn_7100df2284(&self->field_0x2290, &node->key);
    }
    auto* object = static_cast<UnkIdObject_df1ff4*>(fn_71000839a0(sizeof(UnkIdObject_df1ff4), 5));
    fn_7101051530(object);
    if (object) {
        object->vf_0x18();
        object->id = id;
        self->field_0x2290.Insert(id, object);
    }
    return object;
}
