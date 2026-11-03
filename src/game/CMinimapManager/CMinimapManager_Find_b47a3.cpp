// CMinimapManager::CTunableMinimapManager unit (tools/tu_map.py): finds the 0x118-byte entry whose key at +8
// equals the given one in the array at +0xd643f0 (count at +0xd64400). Names and layouts are placeholders.
#include "types.h"

struct UnkMinimapEntry_b47a3 {
    u8 unk_0x0[8];
    u64 key;
    u8 unk_0x10[0x118 - 0x10];
};

struct UnkMinimapEntries_b47a3 {
    UnkMinimapEntry_b47a3* data;
    u8 unk_0x8[8];
    u32 count;
};

struct UnkMinimapManager_b47a3 {
    u8 unk_0x0[0xd643f0];
    UnkMinimapEntries_b47a3 entries;
};

UnkMinimapEntry_b47a3* fn_7100ea6120(UnkMinimapManager_b47a3* self, const u64* key) {
    UnkMinimapEntries_b47a3& v = self->entries;
    for (UnkMinimapEntry_b47a3 *it = v.data, *end = v.data + v.count; it != end; ++it) {
        if (it->key == *key)
            return it;
    }
    return nullptr;
}
