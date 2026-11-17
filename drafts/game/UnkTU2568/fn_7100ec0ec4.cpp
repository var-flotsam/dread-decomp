// The navigation-mesh manager's unit (tools/tu_map.py unit 2568): appends a node to the manager's list of
// smart-link resources and resolves the resource the name's path refers to (in the registry's category
// g_unk_71727b9930) into it; returns the list's last node. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkLinkedList.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU27.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// A counted reference to a resource, as the registry returns it (in memory).
struct UnkAssetResRef {
    UnkCountedResource* ptr;

    UnkAssetResRef() : ptr(nullptr) {}
    UnkAssetResRef& operator=(const UnkAssetResRef& other) {
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

// The file path of a string id's text (its use count at +0x44 goes up first).
struct UnkTextPath_lec0ec4 {
    union {
        CFilePathStrId id;
    };

    explicit UnkTextPath_lec0ec4(const CStrId& name) {
        ++name.data->reads;
        fn_710001a270(&id, name.data->String(), true);
    }
    ~UnkTextPath_lec0ec4() { id.~CFilePathStrId(); }
};

// The ids a resource is looked up by: its category and its path (counted copies).
struct UnkLinkIds_lec0ec4 {
    CStrId category;
    CFilePathStrId path;

    UnkLinkIds_lec0ec4(const CStrId& category, const UnkTextPath_lec0ec4& path) : category(category), path(path.id) {}

    // The registry reads the pair as its id and path pair.
    const UnkStrIdPathPair* AsPair() const { return reinterpret_cast<const UnkStrIdPathPair*>(this); }
};

struct UnkLinkNode_lec0ec4 {
    u32 field_0x0;
    UnkAssetResRef resource;  // 0x8
    UnkLinkNode_lec0ec4* next;  // 0x10

    UnkLinkNode_lec0ec4() : field_0x0(0), next(nullptr) {}
};

using UnkLinkList_lec0ec4 = base::UnkLinkedList<UnkLinkNode_lec0ec4>;

// Appends a new node (nothing when the allocator has no memory).
static inline void AddNode(UnkLinkList_lec0ec4* list) {
    void* memory = list->allocator->vf_0x40(sizeof(UnkLinkNode_lec0ec4), 8, list->flags);
    if (memory)
        list->PushBack(new (memory) UnkLinkNode_lec0ec4);
}

// The resource the registry has for the ids.
static inline UnkAssetResRef Resolve(const UnkLinkIds_lec0ec4& ids) {
    return fn_710002038c(g_unk_7101d4bbc0, ids.AsPair(), true, nullptr);
}

struct UnkNavLinks_lec0ec4 {
    u8 pad_0x0[0x62e60];
    UnkLinkList_lec0ec4 links;  // 0x62e60
};
static_assert(__builtin_offsetof(UnkNavLinks_lec0ec4, links) == 0x62e60, "layout");

UnkLinkNode_lec0ec4* fn_7100ec0ec4(UnkNavLinks_lec0ec4* self, const CStrId* name) {
    AddNode(&self->links);
    UnkLinkNode_lec0ec4* node = self->links.tail;
    UnkLinkIds_lec0ec4 ids(*reinterpret_cast<const CStrId*>(&g_unk_71727b9930), UnkTextPath_lec0ec4(*name));
    node->resource = Resolve(ids);
    return node;
}
