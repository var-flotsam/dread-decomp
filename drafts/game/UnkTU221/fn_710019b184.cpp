#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "types.h"

UnkVec3f fn_7100077c04(const UnkMatrix44& m, const UnkVec3f& v);

struct UnkMat34_b0236b {
    f32 m[12];
};

struct UnkEntry_b0236b {
    f32* matrices;  // 0x0
    u8 pad_0x8[8];
    u32 count;  // 0x10
    UnkMat34_b0236b** sources;  // 0x18
    u8 pad_0x20[8];
};

struct UnkTable_b0236b {
    u8 pad_0x0[0x14];
    u32 count;  // 0x14
    UnkEntry_b0236b* entries;  // 0x18
    UnkVec3f offset;  // 0x20
    u8 pad_0x2c[0xf8];
    u32 flags;  // 0x124
};

void fn_710019b184(UnkTable_b0236b* self, u32 index, u32 slot, UnkMat34_b0236b* matrix) {
    if (self->count <= index)
        return;
    UnkEntry_b0236b* entry = &self->entries[index];
    if (entry->count <= slot)
        return;
    u32 flags = self->flags;
    self->entries[index].sources[slot] = matrix;
    if (flags & 4) {
        UnkMatrix44 local;
        UnkCopyMatrix(&local, *reinterpret_cast<const UnkMatrix44*>(matrix));
        UnkVec3f d = fn_7100077c04(local, self->offset);
        f32 dx = d.x, dy = d.y, dz = d.z;
        local.m[3] = dx + local.m[3];
        local.m[7] = dy + local.m[7];
        local.m[11] = dz + local.m[11];
        __builtin_memcpy(&entry->matrices[slot * 12], &local, 48);
    } else {
        __builtin_memcpy(&entry->matrices[slot * 12], matrix, 48);
    }
}
