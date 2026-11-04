// navmesh::SNavMeshPath's unit (the name from reflection): the last of a path's 0x120-byte entries whose two
// cell ids (+0x16, +0x32) are set (not -1) and whose flag (+0x60) is set. fn_71002acce0 searches the bare entry
// array (count at +0x8, entries from +0x10) and copies the found entry's payload (+0x14) into `out` when given;
// fn_71002ae0f0 asks the path (whose array is at +0x10) through vtable slot 0x20 first and only tells whether one
// exists. Names and layouts are placeholders.
#include "game/UnkObject_0x100.h"
#include "types.h"

UnkObject_0x100* fn_71002b5c90(UnkObject_0x100* self, const UnkObject_0x100* other);

struct UnkNavPathEntry_A2_b69 {
    u8 pad_0x0[0x16];
    s16 cell;  // +0x16
    u8 pad_0x18[0x1a];
    s16 next_cell;  // +0x32
    u8 pad_0x34[0x2c];
    u8 valid;  // +0x60
    u8 pad_0x61[0x120 - 0x61];

    UnkObject_0x100* Payload() { return reinterpret_cast<UnkObject_0x100*>(reinterpret_cast<char*>(this) + 0x14); }
};
static_assert(sizeof(UnkNavPathEntry_A2_b69) == 0x120, "UnkNavPathEntry_A2_b69 layout");

struct UnkNavPathEntries_A2_b69 {
    u8 pad_0x0[0x8];
    u32 count;  // +0x8
    u8 pad_0xc[0x4];
    UnkNavPathEntry_A2_b69 entries[1];  // +0x10
};

static inline __attribute__((always_inline)) UnkNavPathEntry_A2_b69* FindLastValid(UnkNavPathEntries_A2_b69* self) {
    for (u32 i = self->count; i != 0; --i) {
        UnkNavPathEntry_A2_b69* e = &self->entries[i - 1];
        if (e->cell != -1 && e->next_cell != -1 && e->valid)
            return e;
    }
    return nullptr;
}

bool fn_71002acce0(UnkNavPathEntries_A2_b69* self, UnkObject_0x100* out) {
    for (u32 i = self->count; i != 0; --i) {
        UnkNavPathEntry_A2_b69* e = &self->entries[i - 1];
        if (e->cell != -1 && e->next_cell != -1 && e->valid) {
            if (out)
                fn_71002b5c90(out, e->Payload());
            return true;
        }
    }
    return false;
}

struct UnkNavMeshPath_A2_b69 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkNavPathEntries_A2_b69 path;  // +0x10

    bool vf_0x20() { return reinterpret_cast<bool (*)(UnkNavMeshPath_A2_b69*)>(vtable[4])(this); }
};

bool fn_71002ae0f0(UnkNavMeshPath_A2_b69* self) {
    if (!self->vf_0x20())
        return false;
    return FindLastValid(&self->path) != nullptr;
}
