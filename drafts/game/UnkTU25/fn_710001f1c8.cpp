// Unit 25: assigns a string id and file-path id pair from another pair's id (unless it is the pair itself):
// the pair takes a copy of that id and a copy of the global empty path id. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The moved-from id is cleared through a union, so the pool is reloaded after the store.
union UnkIdSlot_l01f1c8 {
    UnkStringData* p;
};

struct UnkPairId_l01f1c8 : base::global::CStrId {
    UnkPairId_l01f1c8(const base::global::CStrId& other) : CStrId(other) {}
    UnkPairId_l01f1c8& operator=(UnkPairId_l01f1c8&& other) {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
        reinterpret_cast<UnkIdSlot_l01f1c8*>(&data)->p = other.data;
        other.data = &g_unk_7101d47108->empty;
        return *this;
    }
};

struct UnkPairPath_l01f1c8 : base::global::UnkFilePathStrIdBase {
    UnkPairPath_l01f1c8(const base::global::CFilePathStrId& other) {
        data = other.data;
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkPairPath_l01f1c8& operator=(UnkPairPath_l01f1c8&& other) {
        UnkStringPool* pool = g_unk_7101d47200;
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
        other.data = &pool->empty;
        return *this;
    }
};

struct UnkPair_l01f1c8 {
    UnkPairId_l01f1c8 id;
    UnkPairPath_l01f1c8 path;

    UnkPair_l01f1c8(const base::global::CStrId& id_, const base::global::CFilePathStrId& path_)
        : id(id_), path(path_) {}
};

UnkPair_l01f1c8* fn_710001f1c8(UnkPair_l01f1c8* self, const UnkPair_l01f1c8* other) {
    if (self != other) {
        UnkPair_l01f1c8 temp(other->id, g_unk_7101d470e8);
        self->id = static_cast<UnkPairId_l01f1c8&&>(temp.id);
        self->path = static_cast<UnkPairPath_l01f1c8&&>(temp.path);
    }
    return self;
}
