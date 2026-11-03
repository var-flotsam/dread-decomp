// GUI::CMissionLog (unit 1706 of tools/tu_map.py): whether any entry of the array at 0x248 (count at 0x250,
// 0x38-byte entries, searched from the back) has the key `*key` at +0x8. Names and layouts are placeholders.
#include "types.h"

struct UnkMissionLogEntry {
    u64 field_0x0;
    u64 key;
    u8 pad_0x10[0x28];
};

struct UnkMissionLogEntries {
    u8 pad_0x0[0x248];
    UnkMissionLogEntry* data;
    u32 count;
};

bool fn_7100b59b18(const UnkMissionLogEntries* self, const u64* key) {
    u64 count = self->count;
    if (count == 0)
        return false;
    const UnkMissionLogEntry* data = self->data;
    u64 k = *key;
    do {
        if (data[count - 1].key == k)
            return true;
    } while (--count != 0);
    return false;
}
