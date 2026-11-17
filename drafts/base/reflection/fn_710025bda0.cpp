// base::reflection's unit (tools/tu_map.py, unit 346): when marked out of date, rebuilds a map of link sets
// from another one: for each named set it adds an entry under the same name and copies in the links whose
// target actor is live, ordered by the id the actor's name holds. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkSetArray;
void fn_7100246eb8(UnkSetArray* self, const UnkCallbackSet_7101a20950* value, u32 index);

struct UnkLinkName_l25bda0 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkLinkActor_l25bda0 {
    u8 pad_0x0[0x30];
    UnkLinkName_l25bda0* name;  // 0x30
};

struct UnkLinkTarget_l25bda0 {
    u8 pad_0x0[0x10];
    UnkLinkActor_l25bda0* actor;  // 0x10
};

// A link of a set (one of its 0x40-byte callback-set elements, seen as a link).
struct UnkLink_l25bda0 {
    u8 pad_0x0[8];
    u8 member[0x28];                 // 0x8
    s32 state;                       // 0x30: 1 until resolved, then 2
    u8 pad_0x34[4];
    UnkLinkTarget_l25bda0* target;  // 0x38

    // The link's target, resolving it first if that has not been done.
    UnkLinkTarget_l25bda0* Target() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(member);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&target),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return target;
    }
};

using UnkLinkSet_l25bda0 = CRntVector<UnkCallbackSet_7101a20950>;

// The link at `index` of a set, growing the set to reach it.
static inline UnkLink_l25bda0* LinkAt(UnkLinkSet_l25bda0* set, u32 index) {
    return reinterpret_cast<UnkLink_l25bda0*>(fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(set), index));
}

// Grows the set's array to hold at least n links, moving the old ones over.
static void Reserve(UnkLinkSet_l25bda0* set, u32 n) {
    if (set->capacity < n) {
        UnkCallbackSet_7101a20950* p = static_cast<UnkCallbackSet_7101a20950*>(
            set->allocator->vf_0x40((u64)n * sizeof(UnkCallbackSet_7101a20950), 8, set->flags));
        if (set->data) {
            UnkCallbackSet_7101a20950* src = set->data;
            UnkCallbackSet_7101a20950* dst = p;
            for (u32 i = 0; i < set->count; ++i) {
                new (dst) UnkCallbackSet_7101a20950(static_cast<UnkCallbackSet_7101a20950&&>(*src));
                src->vf0();
                ++dst;
                ++src;
            }
            set->allocator->vf_0x48(set->data);
        }
        set->data = p;
        set->capacity = n;
    }
}

struct UnkLinkNode_l25bda0 {
    CStrId name;                       // 0x0
    UnkLinkSet_l25bda0 links;          // 0x8
    UnkLinkNode_l25bda0* next;         // 0x30, in insertion order
    UnkLinkNode_l25bda0* prev;         // 0x38
    UnkLinkNode_l25bda0* bucket_next;  // 0x40

    UnkLinkNode_l25bda0(const CStrId& name, UnkLinkSet_l25bda0&& links)
        : name(name), links(static_cast<UnkLinkSet_l25bda0&&>(links)), next(nullptr), prev(nullptr),
          bucket_next(nullptr) {}
};
static_assert(sizeof(UnkLinkNode_l25bda0) == 0x48, "layout");

struct UnkLinkMap_l25bda0 {
    base::UnkAllocator* allocator;          // 0x0, for the nodes
    u32 flags;                              // 0x8
    u32 count;                              // 0xc
    CRntVector<UnkLinkNode_l25bda0*> buckets;  // 0x10
    UnkLinkNode_l25bda0* last;              // 0x38
    UnkLinkNode_l25bda0* first;             // 0x40

    void Clear() {
        if (count) {
            for (UnkLinkNode_l25bda0* n = first; n;) {
                UnkLinkNode_l25bda0* next = n->next;
                fn_71002486a8(reinterpret_cast<UnkNodeMap_710025ad0c*>(this), reinterpret_cast<UnkNode_710025ad0c*>(n));
                n = next;
            }
            memset(buckets.data, 0, (u64)buckets.capacity * 8);
            count = 0;
            last = nullptr;
            first = nullptr;
        }
    }

    // Adds a node for `name` with an empty set (names are not checked for duplicates) and returns its set.
    UnkLinkSet_l25bda0& Add(const CStrId& name) {
        UnkLinkSet_l25bda0 empty(0xf);
        UnkLinkNode_l25bda0** bucket =
            buckets.count ? &buckets.data[(name.data->hash[0] ^ name.data->hash[1]) % buckets.capacity] : nullptr;
        UnkLinkNode_l25bda0* chain = *bucket;
        UnkLinkNode_l25bda0* node =
            static_cast<UnkLinkNode_l25bda0*>(allocator->vf_0x40(sizeof(UnkLinkNode_l25bda0), 8, flags));
        if (node)
            new (node) UnkLinkNode_l25bda0(name, static_cast<UnkLinkSet_l25bda0&&>(empty));
        node->bucket_next = chain;
        *bucket = node;
        if (!first) {
            first = node;
        } else if (last) {
            node->prev = last;
            last->next = node;
        }
        u32& size = count;
        last = node;
        ++size;
        return node->links;
    }
};
static_assert(sizeof(UnkLinkMap_l25bda0) == 0x48, "layout");

struct UnkLinkSets_l25bda0 {
    u8 pad_0x0[0x30];
    UnkLinkMap_l25bda0 links;   // 0x30
    bool dirty;                 // 0x78
    UnkLinkMap_l25bda0 sorted;  // 0x80
};

void fn_710025bda0(void* object) {
    UnkLinkSets_l25bda0* self = static_cast<UnkLinkSets_l25bda0*>(object);
    if (!self->dirty)
        return;
    self->sorted.Clear();
    for (UnkLinkNode_l25bda0* src = self->links.first; src; src = src->next) {
        UnkLinkSet_l25bda0& set = self->sorted.Add(src->name);
        u32 count = src->links.count;
        Reserve(&set, count);
        for (u32 i = 0; i < count; ++i) {
            UnkLink_l25bda0* link = LinkAt(&src->links, i);
            UnkLinkTarget_l25bda0* target = link->Target();
            if (!target || !target->actor)
                continue;
            u32 j = 0;
            for (; j < set.count; ++j) {
                u64 id = LinkAt(&set, j)->Target()->actor->name->id;
                if (id > link->Target()->actor->name->id)
                    break;
            }
            fn_7100246eb8(reinterpret_cast<UnkSetArray*>(&set), reinterpret_cast<const UnkCallbackSet_7101a20950*>(link),
                          j);
        }
    }
    self->dirty = false;
}
