// Unit 2635: clears the flag at +0x30 of every entry of the inline pointer array at +0x54 (count at
// +0x50). Names and layouts are placeholders.
#include "types.h"

struct UnkFlaggedEntry_lfd0e74 {
    u8 pad_0x0[0x30];
    bool flag;  // 0x30
};

#pragma pack(push, 4)
struct UnkFlaggedList_lfd0e74 {
    u8 pad_0x0[0x50];
    u32 count;                            // 0x50
    UnkFlaggedEntry_lfd0e74* entries[1];  // 0x54, `count` long

    UnkFlaggedEntry_lfd0e74** begin() { return entries; }
    UnkFlaggedEntry_lfd0e74** end() { return begin() + count; }
};
#pragma pack(pop)

void fn_7100fd0e74(void* bank) {
    UnkFlaggedList_lfd0e74* self = static_cast<UnkFlaggedList_lfd0e74*>(bank);
    for (UnkFlaggedEntry_lfd0e74* entry : *self)
        entry->flag = false;
}
