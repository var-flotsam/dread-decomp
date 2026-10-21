// Empties the six id lists of a set (two string-id lists and four file-path-id lists).
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// A string id over the given pool whose destructor decrements and then re-tests the count.
template <base::global::UnkStringPool** Pool>
struct UnkPooledId {
    base::global::UnkStringData* data;

    ~UnkPooledId() {
        if (base::global::UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

using UnkId = UnkPooledId<&g_unk_7101d47108>;
using UnkPathId = UnkPooledId<&g_unk_7101d47200>;

struct UnkIdSet {
    base::UnkVector<UnkId> field_0x0;
    base::UnkVector<UnkId> field_0x28;
    base::UnkVector<UnkPathId> field_0x50;
    base::UnkVector<UnkPathId> field_0x78;
    base::UnkVector<UnkPathId> field_0xa0;
    base::UnkVector<UnkPathId> field_0xc8;
};

void fn_7100ee7134(UnkIdSet* self) {
    self->field_0x28.Clear();
    self->field_0x0.Clear();
    self->field_0x50.Clear();
    self->field_0x78.Clear();
    self->field_0xc8.Clear();
    self->field_0xa0.Clear();
}
