// Unit 221: copies every entry's source matrices into its 3x4 matrices, moved by the drawable's offset
// when flag bit 2 is set (fn_710019b184 does one). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkBillboardDrawable.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

// Copies source `part` of `entry` into its matrix slot, moved by `offset`.
inline void MoveSlot(UnkBillboardDrawableEntry* entry, u32 part, const UnkVec3f& offset) {
    UnkMatrix44 local;
    UnkCopyMatrix(&local, *entry->sources[part]);
    UnkVec3f d = fn_7100077c04(local, offset);
    f32 dx = d.x, dy = d.y, dz = d.z;
    local.m[3] = dx + local.m[3];
    local.m[7] = dy + local.m[7];
    local.m[11] = dz + local.m[11];
    __builtin_memcpy(&entry->matrices[part * 12], &local, 48);
}

void fn_710019b304(UnkBillboardDrawable* self) {
    if (self->count == 0)
        return;
    if (self->flags & 4) {
        for (u32 i = 0; i < self->count; i++) {
            UnkBillboardDrawableEntry* entry = &self->entries[i];
            for (u32 part = 0; part < entry->count; part++)
                MoveSlot(entry, part, self->offset);
        }
    } else {
        for (u32 i = 0; i < self->count; i++) {
            UnkBillboardDrawableEntry* entry = &self->entries[i];
            for (u32 part = 0; part < entry->count; part++)
                __builtin_memcpy(&entry->matrices[part * 12], entry->sources[part], 48);
        }
    }
}
