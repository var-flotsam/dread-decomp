// Looks up the id registered for `key`, applies a box to it (unless a global flag blocks it),
// and on success moves the matching (id, b) entries of the pending list to its end, dropping
// them from the count.
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkSlots.h"
#include "types.h"

struct UnkSlotBox {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    UnkSlotBox(const UnkSlotBox& other);
};
UnkSlotBox fn_7100030878(const UnkSlotBox* box);

bool fn_7100199d88(void* table, const u64* key, u32* id);
bool fn_71001996e0(void* table, u32 id, u32 b, UnkSlotBox box, u32 mode);

// Applies the box to the slot `id`/`kind` of the table.
static bool ApplySlot(UnkSlots* self, u32 id, u32 b, const UnkSlotBox* box, u32 mode) {
    return fn_71001996e0(self->info, id, b, fn_7100030878(box), mode);
}

bool fn_71001c28f8(UnkSlots* self, u64 key, u32 b, const UnkSlotBox* box, u32 mode) {
    u32 id = 0;
    if (!fn_7100199d88(self->info, &key, &id))
        return false;
    if (g_unk_710b372a98->field_0x318367c)
        return false;
    u32 found = id;
    if (!ApplySlot(self, found, b, box, mode))
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        UnkSlotEntry* e = self->entries[i];
        if (e->field_0x28 == found && e->field_0x2c == b) {
            --self->count;
            self->entries[i] = self->entries[self->count];
            self->entries[self->count] = e;
        }
    }
    return true;
}
