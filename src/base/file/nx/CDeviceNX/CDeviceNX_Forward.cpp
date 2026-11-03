// base::file::nx::CDeviceNX's unit (tools/tu_map.py unit 38): counts a use on the held entry
// (+0x44) and forwards to fn_710002e224 with the device's part at +0x10, the entry's part at +0x10,
// the caller's flag and 1. The holder is read through a union, which keeps the entry reloaded
// after the count's store as in the original. Names and layouts are placeholders.
#include "types.h"

struct UnkDeviceEntry38 {
    u8 pad_0x0[0x44];
    s32 use_count;  // 0x44
};

union UnkDeviceEntryRef38 {
    UnkDeviceEntry38* entry;
    u8* bytes;
};

bool fn_710002e224(void* device, void* entry, bool flag, bool b);

static inline u8* Use38(UnkDeviceEntryRef38* ref) {
    ref->entry->use_count++;
    return ref->bytes + 0x10;
}

bool fn_710002e1fc(u8* self, UnkDeviceEntryRef38* ref, bool flag) {
    return fn_710002e224(self + 0x10, Use38(ref), flag, true);
}
