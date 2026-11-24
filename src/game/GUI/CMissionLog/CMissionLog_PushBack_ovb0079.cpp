// GUI::CMissionLog's unit (tools/tu_map.py): appends a record (a value, a string id and an array of
// ids, copied with base::UnkStrIdVec's copy constructor) to an array of 0x38-byte records, reserving
// through fn_7100b5c28c as base::UnkStrIdVec::PushBack does (the copy of fn_7100b5bfdc's element
// type). Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "types.h"

using base::global::CStrId;

struct UnkMissionLogRecord_ovb0079 {
    u32 field_0x0;
    CStrId id;
    base::UnkStrIdVec ids;
};

void CMissionLog_EntriesPushBack(UnkElem38Array* self, const UnkElem38* value) {
    u32 n = self->count;
    u32 c = self->capacity;
    if (c <= n && self->step != 0)
        c = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    fn_7100b5c28c(self, c);
    new (&self->data[self->count])
        UnkMissionLogRecord_ovb0079(*reinterpret_cast<const UnkMissionLogRecord_ovb0079*>(value));
    ++self->count;
}
