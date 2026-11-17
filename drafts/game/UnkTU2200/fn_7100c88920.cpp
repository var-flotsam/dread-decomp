// Unit 2200: clears a pair of parallel arrays sharing a count (string ids, and records of two string ids
// and a word): destroys the ids, then the records, and sets the count to zero. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using namespace base::global;

struct UnkIdPair_lc88920 {
    UnkStrIdTwoStep field_0x0;
    UnkStrIdTwoStep field_0x8;
    u32 field_0x10;
};

struct UnkIdArrays_lc88920 {
    UnkIdPair_lc88920* records;
    UnkStrIdTwoStep* ids;
    u32 count;
};

void fn_7100c88920(UnkIdArrays_lc88920* self) {
    if (self->ids) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkStrIdTwoStep();
    }
    if (self->records) {
        for (u64 i = 0; i < self->count; ++i)
            self->records[i].~UnkIdPair_lc88920();
    }
    self->count = 0;
}
