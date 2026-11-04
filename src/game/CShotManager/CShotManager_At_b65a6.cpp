// CShotManager::CTunableShotManager's unit: the address of entry `index` of an array of 0x40-byte
// entries. Names are placeholders.
#include "types.h"

struct UnkShotEntry_b65a6 {
    u8 unk_0x0[0x40];
};

UnkShotEntry_b65a6* fn_7100edcadc(UnkShotEntry_b65a6* entries, u64 index) { return &entries[(u32)index]; }
