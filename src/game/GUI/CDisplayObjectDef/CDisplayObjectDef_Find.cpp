// A search in the GUI::CDisplayObjectDef unit: fn_7100b06ba4 walks the pointer array at +0x60 (count at
// +0x68), bumping the key's lookup counter (+0x44) per entry, and returns the first entry whose id (+0x28)
// equals the key's (+0x30), or null. Names and layouts are placeholders.
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32;

struct UnkDisplayObjectDefEntry {
    u8 pad_0x0[0x28];
    u64 id;  // 0x28
};

struct UnkDisplayObjectDefKey {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
    u8 pad_0x38[0xc];
    u32 lookups;  // 0x44
};

struct UnkDisplayObjectDefList {
    u8 pad_0x0[0x60];
    UnkDisplayObjectDefEntry** data;  // 0x60
    u32 count;                        // 0x68
};

UnkDisplayObjectDefEntry* fn_7100b06ba4(UnkDisplayObjectDefList* self, UnkDisplayObjectDefKey** key) {
    if (self->count == 0)
        return nullptr;
    UnkDisplayObjectDefEntry** it = self->data;
    do {
        UnkDisplayObjectDefEntry* e = *it;
        ++*(a_u32*)&(*key)->lookups;  // may alias: every pointer is reloaded after the bump
        if (e->id == (*key)->id)
            return *it;
        ++it;
    } while (it != self->data + self->count);
    return nullptr;
}
