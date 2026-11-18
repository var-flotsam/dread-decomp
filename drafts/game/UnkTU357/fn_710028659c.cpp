// Unit 357 (tools/tu_map.py), the navigation query's unit: puts a query in its starting state (no
// cells, nothing found, its ranges at -1 and 1). Names and layouts are placeholders.
#include <string.h>

#include "base/math/CVector.h"
#include "game/UnkTU357.h"
#include "types.h"

using base::math::CVector2D;

struct UnkNavQuery_28659c {
    u64 field_0x0;
    u64 targets[12];  // 0x8
    u64 field_0x68;
    u64 field_0x70;
    u16 start_cell;  // 0x78
    u8 start_side;
    u8 pad_0x7b;
    u32 field_0x7c;
    bool enabled;  // 0x80
    u8 pad_0x81[3];
    CVector2D goal;  // 0x84
    u16 goal_cell;   // 0x8c
    u8 pad_0x8e[2];
    u32 hit_count;  // 0x90
    u8 hits[0xc0];
    f32 field_0x154;
    f32 field_0x158;
    f32 field_0x15c;
    f32 field_0x160;
    f32 range_min;  // 0x164
    f32 field_0x168;
    f32 range_max;  // 0x16c
    f32 field_0x170;
    u8 pad_0x174[4];
    u64 field_0x178[4];
    u64 field_0x198;
    bool field_0x1a0;
    u8 pad_0x1a1[3];
    CVector2D field_0x1a4;
    f32 field_0x1ac;
    bool field_0x1b0;
};
static_assert(__builtin_offsetof(UnkNavQuery_28659c, hit_count) == 0x90, "UnkNavQuery_28659c layout");
static_assert(__builtin_offsetof(UnkNavQuery_28659c, field_0x154) == 0x154, "UnkNavQuery_28659c layout");
static_assert(__builtin_offsetof(UnkNavQuery_28659c, field_0x1b0) == 0x1b0, "UnkNavQuery_28659c layout");

void fn_710028659c(UnkNavQuery* query) {
    auto* self = reinterpret_cast<UnkNavQuery_28659c*>(query);
    self->start_cell = 0xffff;
    self->goal_cell = 0xffff;
    self->field_0x68 = 0;
    self->field_0x70 = 0;
    self->field_0x0 = 0;
    self->start_side = 0xff;
    self->field_0x7c = 0;
    self->enabled = false;
    self->goal = CVector2D{0.0f, 0.0f};
    self->hit_count = 0;
    memset(self->targets, 0, sizeof(self->targets));
    self->field_0x154 = 0.0f;
    self->field_0x158 = 0.0f;
    self->field_0x15c = 0.0f;
    self->field_0x160 = 0.0f;
    self->range_min = -1.0f;
    self->field_0x168 = 0.0f;
    self->range_max = 1.0f;
    self->field_0x170 = 0.0f;
    self->field_0x1a4 = CVector2D{0.0f, 0.0f};
    self->field_0x198 = 0;
    self->field_0x1a0 = false;
    self->field_0x1ac = -1.0f;
    self->field_0x1b0 = false;
    memset(self->field_0x178, 0, sizeof(self->field_0x178));
}
