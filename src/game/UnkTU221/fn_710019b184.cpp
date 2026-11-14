#include "base/UnkMatrix44.h"
#include "game/UnkBillboardDrawable.h"
#include "game/UnkTU221.h"
#include "game/UnkXformNode.h"
#include "types.h"
#include "game/UnkTU54.h"


// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710019b184(UnkBillboardDrawable* self, u32 index, u32 part, const UnkMatrix44* matrix) {
    if (self->count <= index)
        return;
    UnkBillboardDrawableEntry* entry = &self->entries[index];
    if (entry->count <= part)
        return;
    u32 flags = self->flags;
    self->entries[index].sources[part] = matrix;
    if (flags & 4) {
        UnkMatrix44 local;
        UnkCopyMatrix(&local, *matrix);
        UnkVec3f d = fn_7100077c04(local, self->offset);
        f32 dx = d.x, dy = d.y, dz = d.z;
        local.m[3] = dx + local.m[3];
        local.m[7] = dy + local.m[7];
        local.m[11] = dz + local.m[11];
        __builtin_memcpy(&entry->matrices[part * 12], &local, 48);
    } else {
        __builtin_memcpy(&entry->matrices[part * 12], matrix, 48);
    }
}
