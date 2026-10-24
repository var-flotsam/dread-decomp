// Looks up the id registered for `key`, applies a box to it (unless a global flag blocks it),
// and on success moves the matching (id, b) entries of the pending list to its end, dropping
// them from the count.
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

struct UnkBox_71001c28f8 {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    UnkBox_71001c28f8(const UnkBox_71001c28f8& other);
};
UnkBox_71001c28f8 fn_7100030878(const UnkBox_71001c28f8* box);

bool fn_7100199d88(void* table, const u64* key, u32* id);
bool fn_71001996e0(void* table, u32 id, u32 b, UnkBox_71001c28f8 box, u32 mode);

struct UnkStruct_71001c28f8_Entry {
    u8 pad_0x0[0x28];
    u32 field_0x28;
    u32 field_0x2c;
};

struct UnkStruct_71001c28f8 {
    u8 pad_0x0[0x78];
    void* field_0x78;
    u8 pad_0x80[0x288];
    UnkStruct_71001c28f8_Entry* field_0x308[9];
    union {  // no type-based alias info: reloaded after each store to the list
        u32 field_0x350;
    };

    bool Apply(u32 id, u32 b, const UnkBox_71001c28f8* box, u32 mode) {
        return fn_71001996e0(field_0x78, id, b, fn_7100030878(box), mode);
    }
};

bool fn_71001c28f8(UnkStruct_71001c28f8* self, u64 key, u32 b, const UnkBox_71001c28f8* box, u32 mode) {
    u32 id = 0;
    if (!fn_7100199d88(self->field_0x78, &key, &id))
        return false;
    if (g_unk_710b372a98->field_0x318367c)
        return false;
    u32 found = id;
    if (!self->Apply(found, b, box, mode))
        return false;
    for (u32 i = 0; i < self->field_0x350; ++i) {
        UnkStruct_71001c28f8_Entry* e = self->field_0x308[i];
        if (e->field_0x28 == found && e->field_0x2c == b) {
            --self->field_0x350;
            self->field_0x308[i] = self->field_0x308[self->field_0x350];
            self->field_0x308[self->field_0x350] = e;
        }
    }
    return true;
}
