// GUI::CMissionLog (unit 1706 of tools/tu_map.py): appends an entry to one of two 0x38-byte-entry arrays (at
// 0x270 for entries of kind 2, else at 0x248) with fn_7100b593d0, then refreshes the log (fn_7100b59690).
// Names and layouts are placeholders.
#include "game/GUI/UnkMissionLogRecords.h"
#include "game/UnkMissionLog.h"
#include "types.h"

bool CMissionLog_StoreEntryListsInBlackboards(UnkMissionLog* self, bool flag);

void CMissionLog_AddRecordAndStoreLists(UnkMissionLog* self, const u32* entry) {
    u8* array = *entry != 2 ? self->entries_0x248 : self->entries_0x270;
    CMissionLog_EntriesPushBack((UnkElem38Array*)array, (const UnkElem38*)entry);
    CMissionLog_StoreEntryListsInBlackboards(self, false);
}
