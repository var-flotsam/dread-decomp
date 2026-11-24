// Unit 2734 (tools/tu_map.py; the sub-area GUI cache): adds an actor definition to the cache unless its key is
// there already: a node with as many pooled actor slots as the definition asks for, then (with `recurse`) the
// definitions its children name. Names and layouts are placeholders.
#include "UnkLeftoverReg.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game/UnkTU344.h"
#include "types.h"

#include <new>

using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkUnusedArg_b49 : UnkLeftoverReg {};
struct UnkPowerUpLifeEntry_b52;
struct UnkPoolOwner_b0231;
UnkPowerUpLifeEntry_b52* fn_710119e5b8(UnkUnusedArg_b49, UnkPoolOwner_b0231* owner);
base::reflection::UnkReflObject* fn_7101193ee0(UnkActorDefFactory* self, const CStrId* id);
void fn_710119dff8(UnkSubAreaGuiCache* self, UnkCountedResource* def, bool flag);

namespace {

// A string id that is taken, not copied, when moved.
struct UnkGuiKey_l19dff8 {
    UnkStringData* data;

    UnkGuiKey_l19dff8(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkGuiKey_l19dff8(UnkGuiKey_l19dff8&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkGuiKey_l19dff8() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

// A pooled actor as the slots hold it.
struct UnkGuiActor_l19dff8 {
    u8 pad_0x0[0x30];
    CStrId id;  // 0x30
};

struct UnkGuiSlot_l19dff8 {
    UnkGuiActor_l19dff8* actor;
    u32 field_0x8;
    bool in_use;  // 0xc
    UnkGuiKey_l19dff8 id;  // 0x10

    explicit UnkGuiSlot_l19dff8(UnkGuiActor_l19dff8* a) : actor(a), field_0x8(0), in_use(true), id(a->id) {}
    UnkGuiSlot_l19dff8(UnkGuiSlot_l19dff8&& other) = default;
};
static_assert(sizeof(UnkGuiSlot_l19dff8) == 0x18, "slot");

// A node's entry: the definition's id and its slots.
struct UnkGuiEntry_l19dff8 {
    UnkGuiKey_l19dff8 id;
    CRntVector<UnkGuiSlot_l19dff8> slots;  // 0x8
    u32 size;                              // 0x30

    UnkGuiEntry_l19dff8(const CStrId& key, u32 n) : id(key), slots(0xf), size(n) { slots.Reserve(n); }
    UnkGuiEntry_l19dff8(UnkGuiEntry_l19dff8&& other)
        : id(static_cast<UnkGuiKey_l19dff8&&>(other.id)),
          slots(static_cast<CRntVector<UnkGuiSlot_l19dff8>&&>(other.slots)), size(other.size) {}
    ~UnkGuiEntry_l19dff8() { slots.Free(); }
};

struct UnkGuiNode_l19dff8 {
    CStrId key;
    UnkGuiEntry_l19dff8 entry;  // 0x8
    UnkGuiNode_l19dff8* list_next;   // 0x40
    UnkGuiNode_l19dff8* list_prev;   // 0x48
    UnkGuiNode_l19dff8* chain_next;  // 0x50

    UnkGuiNode_l19dff8(const CStrId& k, UnkGuiEntry_l19dff8&& e)
        : key(k), entry(static_cast<UnkGuiEntry_l19dff8&&>(e)), list_next(nullptr), list_prev(nullptr),
          chain_next(nullptr) {}
};
static_assert(sizeof(UnkGuiNode_l19dff8) == 0x58, "node");

struct UnkGuiCache_l19dff8 {
    base::IAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 size;                       // 0xc
    UnkGuiNode_l19dff8** buckets;   // 0x10
    u32 count;                      // 0x18
    u32 divisor;                    // 0x1c
    u8 pad_0x20[0x18];
    UnkGuiNode_l19dff8* tail;  // 0x38
    UnkGuiNode_l19dff8* head;  // 0x40

    UnkGuiNode_l19dff8** BucketFor(const CStrId& key) {
        if (count == 0)
            return nullptr;
        return &buckets[key.Hash() % divisor];
    }
    bool Contains(const CStrId* key) {
        UnkGuiNode_l19dff8** bucket = BucketFor(*key);
        if (bucket) {
            for (UnkGuiNode_l19dff8* node = *bucket; node; node = node->chain_next) {
                if (node->key == *key)
                    return true;
            }
        }
        return false;
    }
    UnkGuiNode_l19dff8* Insert(const CStrId& key, UnkGuiEntry_l19dff8&& entry) {
        UnkGuiNode_l19dff8** bucket = BucketFor(key);
        UnkGuiNode_l19dff8* next = *bucket;
        UnkGuiNode_l19dff8* node =
            static_cast<UnkGuiNode_l19dff8*>(allocator->vf_0x40(sizeof(UnkGuiNode_l19dff8), 8, flags));
        if (node)
            new (node) UnkGuiNode_l19dff8(key, static_cast<UnkGuiEntry_l19dff8&&>(entry));
        node->chain_next = next;
        *bucket = node;
        if (!head) {
            head = node;
        } else if (tail) {
            node->list_prev = tail;
            tail->list_next = node;
        }
        tail = node;
        ++size;
        return node;
    }
};

struct UnkGuiDef_l19dff8 {
    u8 pad_0x0[0x60];
    bool pooled;      // 0x60
    u32 pool_size;    // 0x64
    u8 pad_0x68[0x8];
    CStrId* children;  // 0x70
    u32 child_count;   // 0x78
    u8 pad_0x7c[0x1c];
    CStrId key;  // 0x98
};

}  // namespace

void fn_710119dff8(UnkSubAreaGuiCache* cache, UnkCountedResource* resource, bool recurse) {
    UnkGuiCache_l19dff8* self = reinterpret_cast<UnkGuiCache_l19dff8*>(cache);
    UnkGuiDef_l19dff8* def = reinterpret_cast<UnkGuiDef_l19dff8*>(resource);
    if (self->Contains(&def->key))
        return;

    if (def->pooled) {
        u32 n = def->pool_size;
        CStrId key = def->key;
        UnkGuiNode_l19dff8* node = self->Insert(key, UnkGuiEntry_l19dff8(key, n));
        if (node) {
            for (s32 i = 0; i < (s32)n; ++i) {
                UnkGuiActor_l19dff8* actor = reinterpret_cast<UnkGuiActor_l19dff8*>(
                    fn_710119e5b8(UnkUnusedArg_b49(), reinterpret_cast<UnkPoolOwner_b0231*>(&node->entry)));
                node->entry.slots.EmplaceBack(UnkGuiSlot_l19dff8(actor));
            }
        }
    }

    if (recurse) {
        for (CStrId* it = def->children, *end = it + def->child_count; it != end; ++it) {
            if (self->Contains(it))
                continue;
            if (base::reflection::UnkReflObject* child = fn_7101193ee0(g_unk_71726a0220, it))
                fn_710119dff8(cache, reinterpret_cast<UnkCountedResource*>(child), true);
        }
    }
}
