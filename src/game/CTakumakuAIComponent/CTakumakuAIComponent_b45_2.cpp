// CTakumakuAIComponent's unit (tools/tu_map.py): with a target (+0x146c8), moves the point 60 units
// down in x and sets its y to one of the target's two values (+0x614 while the id at +0x14708 is 0
// or 0x7fffffff, else +0x618). Names are placeholders.
#include "types.h"

struct UnkTakumakuPoint_b45a2 {
    f32 x;
    f32 y;
};

struct UnkTakumakuTarget2_b45a2 {
    u8 pad_0x0[0x614];
    f32 field_0x614;
    f32 field_0x618;
};

struct UnkTakumakuAI2_b45a2 {
    u8 pad_0x0[0x146c8];
    // A union member: Clang gives it no type-based alias info, so the original reloads it after
    // the store to the point.
    union {
        UnkTakumakuTarget2_b45a2* target;
    };
    u8 pad_0x146d0[0x38];
    s32 id;  // 0x14708
};

void fn_71006d5da4(UnkTakumakuAI2_b45a2* self, UnkTakumakuPoint_b45a2* point) {
    if (!self->target)
        return;
    point->x -= 60.0f;
    if (self->id != 0 && self->id != 0x7fffffff)
        point->y = self->target->field_0x618;
    else
        point->y = self->target->field_0x614;
}
