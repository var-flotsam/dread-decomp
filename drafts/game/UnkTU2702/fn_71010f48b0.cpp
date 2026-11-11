// Unit 2702 (near CAIComponent): releases the ten string ids of a record, last to first,
// leaving each one empty. Names and layouts are placeholders.
// Draft: original reuses the pool load across ids 0x90..0x50 only and reloads it for 0x40..0x0;
// this version threads all ten.

#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdEntry {
    base::global::CStrId id;
    u8 pad_0x8[0x8];
};

struct UnkIdRecord {
    UnkIdEntry entries[10];
};

void fn_71010f48b0(UnkIdRecord* self) {
    for (int i = 9; i >= 0; --i)
        self->entries[i].id.Reset();
}
