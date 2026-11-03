// navmesh::CDynamicSmartLinkGroup's unit (tools/tu_map.py unit 351): the setters (slots 0x140 and
// 0x148) of a reflected map from u32 keys to entries of a file-path id and a counted resource
// reference. The entry under the key (found or added by fn_7100268c64) is copy-assigned
// (fn_7100267cfc) or move-assigned (fn_7100267dcc, the reference still copied) member by member,
// as CBlendSpaceResource's map (src/base/animation/CBlendSpaceResource/CBlendSpaceResource_MapSet_b49.cpp).
// Names are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The moved-from id is cleared through a union, so the pool is reloaded after the store.
union UnkSmartLinkIdSlot_b49a1 {
    UnkStringData* p;
};

struct UnkSmartLinkPathId_b49a1 {
    UnkStringData* data;

    UnkSmartLinkPathId_b49a1& operator=(const UnkSmartLinkPathId_b49a1& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
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

    UnkSmartLinkPathId_b49a1& operator=(UnkSmartLinkPathId_b49a1&& other) {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        reinterpret_cast<UnkSmartLinkIdSlot_b49a1*>(&data)->p = other.data;
        other.data = &g_unk_7101d47200->empty;
        return *this;
    }
};

// A counted reference whose assignment reads the source pointer first.
struct UnkSmartLinkRef_b49a1 {
    UnkCountedResource* ptr;

    UnkSmartLinkRef_b49a1& operator=(const UnkSmartLinkRef_b49a1& other) {
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

struct UnkSmartLinkEntry_b49a1 {
    UnkSmartLinkPathId_b49a1 path;
    UnkSmartLinkRef_b49a1 resource;
};

struct UnkSmartLinkMap_b49a1;
UnkSmartLinkEntry_b49a1* fn_7100268c64(UnkSmartLinkMap_b49a1* self, u32 key);  // find or add

void fn_7100267cfc(UnkSmartLinkMap_b49a1* self, const u32* key, const UnkSmartLinkEntry_b49a1* value) {
    *fn_7100268c64(self, *key) = *value;
}

void fn_7100267dcc(UnkSmartLinkMap_b49a1* self, const u32* key, UnkSmartLinkEntry_b49a1* value) {
    *fn_7100268c64(self, *key) = static_cast<UnkSmartLinkEntry_b49a1&&>(*value);
}
