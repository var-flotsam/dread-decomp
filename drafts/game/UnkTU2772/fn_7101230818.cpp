// nn::atk unit 2772: reads one entry from a packed record: the entry's leading byte and up to three data bytes,
// or 0x7f and zeroed data when the record has no entry table. Names and layouts are placeholders.
#include <string.h>

#include "types.h"

struct UnkAtkRecord_l230818 {
    u32 flags;  // 0x0
    u32 offsets[1];  // 0x4
};

struct UnkAtkEntry_l230818 {
    u8 head;    // 0x0
    u8 count;   // 0x1
    u8 data[3];  // 0x2
};

void fn_7101230818(UnkAtkRecord_l230818* self, u8* head, u8* data, u8 size) {
    u32 flags = self->flags;
    if (!(flags & 0x100)) {
        *head = 0x7f;
        if (size)
            memset(data, 0, size);
        return;
    }
    u32 index = (flags & 1) + ((flags >> 1) & 1) + ((flags >> 2) & 1) + ((flags >> 3) & 1) + ((flags >> 4) & 1) +
                ((flags >> 5) & 1) + ((flags >> 6) & 1) + ((flags >> 7) & 1);
    u32 offset = self->offsets[index];
    UnkAtkEntry_l230818* entry = reinterpret_cast<UnkAtkEntry_l230818*>(reinterpret_cast<u8*>(self) + offset);
    *head = entry->head;
    u32 count = entry->count < 3 ? entry->count : 3;
    for (u32 i = 0; i < count; ++i)
        data[i] = entry->data[i];
}
