// Unit 221: the billboard drawable's constructor: a table of `slotCount` entries, each given one 3x4
// matrix and one source slot, an identity matrix and a grey colour. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkBillboardDrawable.h"
#include "game/UnkTU221.h"
#include "types.h"

#include <new>

// The full layouts this constructor writes (include/game/UnkBillboardDrawable.h has only the members the
// other functions read).
struct UnkBillboardDrawableSlot {
    f32* matrices;  // 0x0
    u32 field_0x8;
    s32 field_0xc;
    u32 count;  // 0x10
    const UnkMatrix44** sources;  // 0x18
    u32 field_0x20;

    UnkBillboardDrawableSlot()
        : matrices(nullptr), field_0x8(0), field_0xc(-1), count(0), sources(nullptr), field_0x20(0) {}
};
static_assert(sizeof(UnkBillboardDrawableSlot) == 0x28, "UnkBillboardDrawableSlot placeholder layout");

struct UnkBillboardDrawableFields {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 count;  // 0x14
    UnkBillboardDrawableSlot* entries;  // 0x18
    f32 offset[3];  // 0x20
    f32 field_0x2c[11];
    f32 field_0x58[3];
    f32 matrix[16];  // 0x64: starts as the identity
    f32 field_0xa4[24];
    f32 color[4];  // 0x104
    f32 field_0x114[4];
    u32 flags;  // 0x124
};
static_assert(sizeof(UnkBillboardDrawableFields) == 0x128, "UnkBillboardDrawableFields placeholder layout");

void fn_710019a4e8(UnkBillboardDrawable* drawable, u32 a, u32 slotCount) {
    UnkBillboardDrawableFields* self = reinterpret_cast<UnkBillboardDrawableFields*>(drawable);
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->count = slotCount;
    UnkBillboardDrawableSlot* entries = static_cast<UnkBillboardDrawableSlot*>(
        fn_7100083ba0(slotCount * sizeof(UnkBillboardDrawableSlot), 0x1e));
    new (entries) UnkBillboardDrawableSlot[slotCount];
    self->entries = entries;

    for (u32 i = 0; i < 3; i++)
        self->offset[i] = 0.0f;
    for (u32 i = 0; i < 11; i++)
        self->field_0x2c[i] = 0.0f;
    for (u32 i = 0; i < 3; i++)
        self->field_0x58[i] = 0.0f;
    for (u32 row = 0; row < 4; row++)
        for (u32 col = 0; col < 4; col++)
            self->matrix[row * 4 + col] = row == col ? 1.0f : 0.0f;
    __builtin_memset(self->field_0xa4, 0, sizeof(self->field_0xa4));
    self->color[0] = 0.25f;
    self->color[1] = 0.25f;
    self->color[2] = 0.25f;
    self->color[3] = 1.0f;
    for (u32 i = 0; i < 4; i++)
        self->field_0x114[i] = 0.0f;
    self->flags = 0;

    for (u32 i = 0; i < self->count; i++) {
        self->entries[i].field_0x20 = 0;
        self->entries[i].count = 1;
        self->entries[i].sources = static_cast<const UnkMatrix44**>(fn_7100083ba0(sizeof(UnkMatrix44*), 0x1e));
        self->entries[i].matrices = static_cast<f32*>(fn_7100083990()->vf_0x40(12 * sizeof(f32), 8, 0x32));
        self->entries[i].sources[0] = nullptr;
    }
}
