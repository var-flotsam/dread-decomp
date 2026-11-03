// msapi::material unit: lookup of a 0x60-byte entry by its id.
#include "types.h"

struct UnkMaterialEntry_0x60 {
    u8 unk_0x0[0x8];
    u32 id;
    u8 unk_0xc[0x54];
};

struct UnkMaterialEntryOwner {
    u8 unk_0x0[0x358];
    UnkMaterialEntry_0x60* entries;
    u32 count;
};

UnkMaterialEntry_0x60* fn_71001a87f8(UnkMaterialEntryOwner* self, u32 id) {
    UnkMaterialEntry_0x60* r = nullptr;
    UnkMaterialEntry_0x60* end = self->entries + self->count;
    for (UnkMaterialEntry_0x60* it = self->entries; !r && it != end; ++it)
        r = it->id == id ? it : nullptr;
    return r;
}
