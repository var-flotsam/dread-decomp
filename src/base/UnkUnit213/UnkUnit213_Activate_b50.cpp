// Unit 213 of tools/tu_map.py, beside UnkUnit213_Dtors.cpp (same layout): activates the pass once,
// creating its two pooled objects (+0x28 by fn_7100161960, +0x30 by fn_7100154020) for a pair of sizes
// that it keeps at +0x20. Names and layouts are placeholders.
#include "game/UnkTU174.h"
#include "game/UnkTU182.h"
#include "types.h"

struct UnkObj_7100161b90;
struct UnkObj_71001541a0;

struct UnkPass213Activate {
    u8 pad_0x0[0x20];
    u32 width;   // 0x20
    u32 height;  // 0x24
    UnkObj_7100161b90* field_0x28;
    UnkObj_71001541a0* field_0x30;
    bool active;  // 0x38
};

void fn_7100195548(UnkPass213Activate* self, u32 width, u32 height) {
    if (self->active)
        return;
    self->active = true;
    alignas(8) u32 first = 1;
    fn_7100161960(&self->field_0x28, width, nullptr, &first);
    alignas(8) u32 second = 1;
    fn_7100154020(&self->field_0x30, height, &second, nullptr, 1, 2);
    self->width = width;
    self->height = height;
}
