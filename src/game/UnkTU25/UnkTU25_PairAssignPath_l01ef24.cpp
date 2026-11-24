// Unit 25: assigns a string id and file-path id pair from path text (unless it is the pair itself): the ids
// are built from the text (fn_710001f074) and moved into the pair. Names and layouts are placeholders.
#include "base/UnkPathIdsFromText.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The moved-from id is cleared through a union, so the pool is reloaded after the store.
union UnkIdSlot_l01ef24 {
    UnkStringData* p;
};

struct UnkPairId_l01ef24 : base::global::CStrId {
    UnkPairId_l01ef24& operator=(base::UnkStrIdMember&& other) {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
        reinterpret_cast<UnkIdSlot_l01ef24*>(&data)->p = other.data;
        other.data = &g_unk_7101d47108->empty;
        return *this;
    }
};

struct UnkPairPath_l01ef24 : base::global::CFilePathStrId {
    UnkPairPath_l01ef24& operator=(base::UnkStrIdMemberOf<g_unk_7101d47200>&& other) {
        UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &pool->empty;
        return *this;
    }
};

struct UnkPair_l01ef24 {
    UnkPairId_l01ef24 id;
    UnkPairPath_l01ef24 path;
};

UnkPair_l01ef24* AssignAssetIdFromString(UnkPair_l01ef24* self, const UnkInlineString* other) {
    if (reinterpret_cast<const void*>(other) != reinterpret_cast<const void*>(self)) {
        UnkPathIdsFromText ids(*reinterpret_cast<const base::global::CRntString*>(other));
        self->id = static_cast<base::UnkStrIdMember&&>(ids.ids.id);
        self->path = static_cast<base::UnkStrIdMemberOf<g_unk_7101d47200>&&>(ids.ids.path);
    }
    return self;
}
