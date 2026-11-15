// Unit 2200: clears a pair of parallel arrays sharing a count (string ids, and records of two string ids
// and a word): destroys the ids, then the records, and sets the count to zero. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps.
struct UnkReleasedStrId_lc88920 {
    UnkStringData* data;

    ~UnkReleasedStrId_lc88920() {
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
    }
};

struct UnkIdPair_lc88920 {
    UnkReleasedStrId_lc88920 field_0x0;
    UnkReleasedStrId_lc88920 field_0x8;
    u32 field_0x10;
};

struct UnkIdArrays_lc88920 {
    UnkIdPair_lc88920* records;
    UnkReleasedStrId_lc88920* ids;
    u32 count;
};

void fn_7100c88920(UnkIdArrays_lc88920* self) {
    if (self->ids) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkReleasedStrId_lc88920();
    }
    if (self->records) {
        for (u64 i = 0; i < self->count; ++i)
            self->records[i].~UnkIdPair_lc88920();
    }
    self->count = 0;
}
