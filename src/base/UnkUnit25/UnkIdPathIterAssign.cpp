// From unit 25 of tools/tu_map.py (0x7100018a44-0x710001f9ac): the copy assignment of a reflected
// iterator value (base/UnkCollectionIter.h) with a u32 index and an element of a string id and a
// file-path id. Names and layouts are placeholders. The member types have external linkage: in an
// anonymous namespace the path id's loads come out in the other order.
#include "base/UnkCollectionIter.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

// A file-path id copied in: `data` is read before `other.data`, and the released id is reloaded
// for the release call (as CStrId's copy assignment, on the file-path pool).
struct UnkUnit25PathId {
    base::global::UnkStringData* data;

    UnkUnit25PathId& operator=(const UnkUnit25PathId& other) {
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

struct UnkUnit25IdPath {
    base::global::CStrId id;
    UnkUnit25PathId path;
};

void* fn_710001d498(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkUnit25IdPath, u32>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
