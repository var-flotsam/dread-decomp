// navmesh::SNavMeshPath's unit (unit 366 of tools/tu_map.py, the name from reflection): when vtable slot 0x20
// answers true, tells whether one of the path's entries (count at +0x18, 0x120-byte entries from +0x20) has the
// key {u16, u8} `key` names (at +0x4 and +0x6 of an entry). Names and layouts are placeholders.
#include "types.h"

struct UnkNavKey_A2_b68 {
    u16 id;
    u8 sub;
};

struct UnkNavEntry_A2_b68 {
    u8 pad_0x0[0x4];
    u16 id;
    u8 sub;
    u8 pad_0x7[0x120 - 0x7];
};

struct UnkNavPath_A2_b68 {
    void** vtable;
    u8 pad_0x8[0x10];
    u32 count;
    u8 pad_0x1c[0x4];
    UnkNavEntry_A2_b68 entries[1];

    bool vf_0x20() { return reinterpret_cast<bool (*)(UnkNavPath_A2_b68*)>(vtable[4])(this); }
};

bool fn_71002ae068(UnkNavPath_A2_b68* self, const UnkNavKey_A2_b68* key) {
    if (!self->vf_0x20())
        return false;
    u32 count = self->count;
    if (count == 0)
        return false;
    u16 id = key->id;
    u8 sub = key->sub;
    for (u32 i = 0; i < count; ++i) {
        if (self->entries[i].id == id && self->entries[i].sub == sub)
            return true;
    }
    return false;
}
