// Unit 25: looks an id pair up in a two-level table and returns the stored string-id pair
// (the defaults when either level misses). Names and layouts are placeholders.

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;


struct UnkIdPair_l01af30 {
    CStrId name;
    CFilePathStrId path;
};

// Parallel arrays: values and their keys; a miss returns the end.
template <class Value>
struct UnkKeyedTable {
    Value* values;
    u64* keys;
    u32 count;

    Value* Find(const u64* key) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == *key)
                return &values[i];
        }
        return &values[count];
    }
};

struct UnkInnerEntry : UnkKeyedTable<UnkIdPair_l01af30> {
    u8 pad_0x18[0x80 - sizeof(UnkKeyedTable<UnkIdPair_l01af30>)];
};

// Copy-assignment of a file-path id (CFilePathStrId has none), as CStrId's, on its pool.
inline void AssignPath(CFilePathStrId& dst, const CFilePathStrId& src) {
    if (dst.data != src.data) {
        base::global::UnkStringData* d = src.data;
        base::global::UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (dst.data != &pool->empty) {
            --dst.data->ref_count;
            if (dst.data->ref_count == 0)
                StringPool_Release(pool, dst.data);
        }
        dst.data = d;
    }
}

struct UnkIdKey {
    u64 outer;
    u64 inner;
};

struct UnkResult {
    CStrId name;
    CFilePathStrId path;
    UnkResult() : name(base::global::CStrId::Empty()), path(g_unk_7101d470e8) {}
};


UnkResult fn_710001af30(UnkKeyedTable<UnkInnerEntry>* table, const UnkIdKey* key) {
    UnkResult result;
    UnkInnerEntry* outer = table->Find(&key->outer);
    if (outer == &table->values[table->count])
        return result;
    UnkIdPair_l01af30* pair = outer->Find(&key->inner);
    if (pair == &outer->values[outer->count])
        return result;
    result.name = pair->name;
    AssignPath(result.path, pair->path);
    return result;
}
