// base::animation::CBlendSpaceResource (name from reflection,
// src/reflection/base_animation_CBlendSpaceResource.cpp): the copy assignment its descriptor points
// at, and that of an iterator value over the same members (0x18 bytes: a word, a file-path id and a
// counted resource reference). Member names are placeholders. The member types have external
// linkage: in an anonymous namespace the path id's loads come out in the other order.
#include "base/UnkCollectionIter.h"
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

// A file-path id copied in: `data` is read before `other.data`, and the released id is reloaded
// for the release call (as CStrId's copy assignment, on the file-path pool).
struct UnkBlendPathId {
    base::global::UnkStringData* data;

    UnkBlendPathId& operator=(const UnkBlendPathId& other) {
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
};

namespace {
// A counted reference whose assignment reads the source pointer first.
struct UnkBlendRef {
    UnkCountedResource* ptr;

    UnkBlendRef& operator=(const UnkBlendRef& other) {
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

struct CBlendSpaceResourceValue {
    u64 field_0x0;
    UnkBlendPathId field_0x8;
    UnkBlendRef field_0x10;

    __attribute__((always_inline)) CBlendSpaceResourceValue& operator=(const CBlendSpaceResourceValue& other) {
        if (this != &other) {
            field_0x0 = other.field_0x0;
            field_0x8 = other.field_0x8;
            field_0x10 = other.field_0x10;
        }
        return *this;
    }
};
}  // namespace

void* fn_710000f598(void* dst, const void* src) {
    return &(*static_cast<CBlendSpaceResourceValue*>(dst) = *static_cast<const CBlendSpaceResourceValue*>(src));
}

// The same members as an element of a reflected collection (assigned member by member, no self
// check), and the copy assignment of its iterator value (base/UnkCollectionIter.h, u32 index).
struct UnkBlendSpaceElem {
    u64 field_0x0;
    UnkBlendPathId field_0x8;
    UnkBlendRef field_0x10;
};

void* fn_710000d7c4(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkBlendSpaceElem, u32>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
