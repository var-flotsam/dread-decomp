// Unit 371 (tools/tu_map.py): initialises a timed scale record: the id and a value, a scale of
// `rate` (1 when not positive) with its inverse, cleared counters, a capacity of 10, a step of 15
// and the default allocator (fn_7100083990). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::global::CStrId;

// Named as src/game/CWaterPlatformUsableComponent/CWaterPlatformUsableComponent_b43.cpp names it.
struct UnkWaterPuzzle_b43a3 {
    const CStrId* id;
    f32 value;          // 0x8
    f32 scale;          // 0xc
    f32 inv_scale;      // 0x10
    bool flag_0x14;
    u64 field_0x18;
    u64 field_0x20;
    u32 capacity;       // 0x28
    bool enabled;       // 0x2c
    u32 step;           // 0x30
    base::UnkAllocator* allocator;  // 0x38
};
static_assert(sizeof(UnkWaterPuzzle_b43a3) == 0x40, "UnkWaterPuzzle_b43a3 layout");

void fn_71002b2afc(UnkWaterPuzzle_b43a3* self, const CStrId& id, f32 value, f32 rate) {
    self->scale = 1.0f;
    self->inv_scale = 1.0f;
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->capacity = 10;
    self->enabled = true;
    self->id = &id;
    self->value = value;
    self->flag_0x14 = false;
    self->step = 15;
    self->allocator = fn_7100083990();
    f32 s = rate;
    if (rate <= 0.0f)
        s = 1.0f;
    self->scale = s;
    self->inv_scale = 1.0f / s;
}
