// CMinimapManager::CTunableMinimapManager's unit: find the 0x118-byte entry of the list at +0xd643f0 whose key
// (+0x18) and kind (+0x20) are the given ones; null when none is. Names are placeholders.
#include "types.h"

struct UnkMinimapEntry_A6_b68 {
    u8 unk_0x0[0x18];
    u64 key;
    s32 kind;
    u8 unk_0x24[0x118 - 0x24];
};

struct UnkMinimapEntryList_A6_b68 {
    UnkMinimapEntry_A6_b68* data;
    u8 unk_0x8[0x8];
    u32 count;
};

struct UnkMinimapOwner_A6_b68 {
    u8 unk_0x0[0xd643f0];
    UnkMinimapEntryList_A6_b68 entries;
};

UnkMinimapEntry_A6_b68* fn_7100ea6164(UnkMinimapOwner_A6_b68* self, const u64* key, s32 kind) {
    UnkMinimapEntryList_A6_b68& list = self->entries;
    for (UnkMinimapEntry_A6_b68* it = list.data, *end = list.data + list.count; it != end; ++it) {
        if (it->key == *key && it->kind == kind)
            return it;
    }
    return nullptr;
}
