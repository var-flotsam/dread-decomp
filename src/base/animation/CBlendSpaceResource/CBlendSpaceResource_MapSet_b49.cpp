// base::animation::CBlendSpaceResource's unit (tools/tu_map.py unit 14): the setter its reflected
// map of resources by index points at (fn_710000e0e0). The value under the key (found or added by
// fn_710000efe0) is assigned (or moved, fn_710000e1b8) member by member: a word, a file-path id and a counted resource
// reference (the members of src/base/animation/CBlendSpaceResource/CBlendSpaceResource_Assign.cpp).
// Names are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

// A file-path id copied in: `data` is read before `other.data`, and the released id is reloaded
// for the release call.
struct UnkBlendPathId_b49a1 {
    base::global::UnkStringData* data;

    UnkBlendPathId_b49a1& operator=(const UnkBlendPathId_b49a1& other) {
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

    // Moved in: the old id is released (leaving the pool's empty string), then the other's taken.
    UnkBlendPathId_b49a1& operator=(UnkBlendPathId_b49a1&& other) {
        if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
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
struct UnkBlendRef_b49a1 {
    UnkCountedResource* ptr;

    UnkBlendRef_b49a1& operator=(const UnkBlendRef_b49a1& other) {
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

struct UnkBlendSpaceEntry_b49a1 {
    u64 field_0x0;
    UnkBlendPathId_b49a1 field_0x8;
    UnkBlendRef_b49a1 field_0x10;
};

struct UnkBlendSpaceMap_b49a1;
UnkBlendSpaceEntry_b49a1* fn_710000efe0(UnkBlendSpaceMap_b49a1* self, u32 key);  // find or add

void fn_710000e0e0(UnkBlendSpaceMap_b49a1* self, const u32* key, const UnkBlendSpaceEntry_b49a1* value) {
    *fn_710000efe0(self, *key) = *value;
}

// The same with the value moved in (fn_710000e1b8): the reference has no move, so it is copied.
void fn_710000e1b8(UnkBlendSpaceMap_b49a1* self, const u32* key, UnkBlendSpaceEntry_b49a1* value) {
    *fn_710000efe0(self, *key) = static_cast<UnkBlendSpaceEntry_b49a1&&>(*value);
}
