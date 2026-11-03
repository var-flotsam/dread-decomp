// navmesh::CDynamicSmartLinkGroup (name from reflection,
// src/reflection/navmesh_CDynamicSmartLinkGroup.cpp): the copy and move assignments its descriptor points at
// (0x10 bytes: a file-path id and a counted resource reference), and that of an iterator value over
// the same members. Member names are placeholders.
// The member types have external linkage: in an anonymous namespace the path id's loads come out
// in the other order.
#include "base/UnkCollectionIter.h"
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

// A file-path id copied in: `data` is read before `other.data`, and the released id is reloaded
// for the release call (as CStrId's copy assignment, on the file-path pool).
struct UnkSmartLinkGroupPathId {
    base::global::UnkStringData* data;

    UnkSmartLinkGroupPathId& operator=(const UnkSmartLinkGroupPathId& other) {
        if (data != other.data) {
            base::global::UnkStringData* d = other.data;
            base::global::UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
    // Moved in: the old id is released (decremented and tested in two steps) and the source gets
    // the empty id of the pool loaded again (as UnkStrIdBase::MoveFrom).
    UnkSmartLinkGroupPathId& operator=(UnkSmartLinkGroupPathId&& other) {
        base::global::UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &g_unk_7101d47200->empty;
        return *this;
    }
};

// A counted reference whose assignment reads the source pointer first.
struct UnkSmartLinkGroupRef {
    UnkCountedResource* ptr;

    UnkSmartLinkGroupRef& operator=(const UnkSmartLinkGroupRef& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
};

struct CDynamicSmartLinkGroupValue {
    UnkSmartLinkGroupPathId field_0x0;
    UnkSmartLinkGroupRef field_0x8;

    __attribute__((always_inline)) CDynamicSmartLinkGroupValue& operator=(const CDynamicSmartLinkGroupValue& other) {
        if (this != &other) {
            field_0x0 = other.field_0x0;
            field_0x8 = other.field_0x8;
        }
        return *this;
    }
    // The move assignment copies the reference.
    __attribute__((always_inline)) CDynamicSmartLinkGroupValue& operator=(CDynamicSmartLinkGroupValue&& other) {
        if (this != &other) {
            field_0x0 = static_cast<UnkSmartLinkGroupPathId&&>(other.field_0x0);
            field_0x8 = other.field_0x8;
        }
        return *this;
    }
};

void* fn_71002691f4(void* dst, const void* src) {
    return &(*static_cast<CDynamicSmartLinkGroupValue*>(dst) = *static_cast<const CDynamicSmartLinkGroupValue*>(src));
}
void* fn_71002692c8(void* dst, const void* src) {
    CDynamicSmartLinkGroupValue* d = static_cast<CDynamicSmartLinkGroupValue*>(dst);
    return &(*d = static_cast<CDynamicSmartLinkGroupValue&&>(*static_cast<CDynamicSmartLinkGroupValue*>(const_cast<void*>(src))));
}

// The same members as an element of a reflected collection (assigned member by member, no self
// check), and the copy assignment of its iterator value (base/UnkCollectionIter.h, u32 index).
struct UnkSmartLinkGroupElem {
    UnkSmartLinkGroupPathId field_0x0;
    UnkSmartLinkGroupRef field_0x8;
};

void* fn_71002673f8(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkSmartLinkGroupElem, u32>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
