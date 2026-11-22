// Unit 2200: clears a pair of parallel arrays sharing a count (string ids, and records of two string ids
// and a word): destroys the ids, then the records, and sets the count to zero. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkCutSceneTable_lc886c0.h"
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

void fn_7100c88920(UnkCutSceneTable_lc886c0* table) {
    UnkIdArrays_lc88920* self = reinterpret_cast<UnkIdArrays_lc88920*>(table);
    if (self->ids) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkStrIdTwoStep();
    }
    if (self->records) {
        // Stepped as a byte offset alongside the index.
        u64 offset = 0;
        for (u64 i = 0; i < self->count; offset += sizeof(UnkIdPair_lc88920), ++i)
            reinterpret_cast<UnkIdPair_lc88920*>(reinterpret_cast<u8*>(self->records) + offset)->~UnkIdPair_lc88920();
    }
    self->count = 0;
}
