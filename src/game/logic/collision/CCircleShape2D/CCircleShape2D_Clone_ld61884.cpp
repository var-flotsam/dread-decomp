// game::logic::collision::CCircleShape2D unit (tools/tu_map.py): the clone of a circle shape, a new 0xb0-byte
// shape (the shape base fn_7100d6bfd8, then the circle's vtable) with the same translation and radius.
// Names and layouts are placeholders.
#include <new>

#include "game/logic/collision/CShape.h"
#include "types.h"

extern char g_unk_7101aa8a20[];

struct UnkCircleShape2D_ld61884 {
    char* vtable;
    f32 transform[3][4];  // 0x8
    u8 pad_0x38[0xa8 - 0x38];
    f32 radius;  // 0xa8
    bool field_0xac;

    UnkCircleShape2D_ld61884(f32 x, f32 y, f32 z, f32 r) {
        fn_7100d6bfd8(this);
        vtable = g_unk_7101aa8a20 + 0x10;
        field_0xac = false;
        transform[0][3] = x;
        transform[1][3] = y;
        transform[2][3] = z;
        radius = r;
    }
};
static_assert(sizeof(UnkCircleShape2D_ld61884) == 0xb0, "layout");

UnkCircleShape2D_ld61884* fn_7100d61884(const UnkCircleShape2D_ld61884* self) {
    return new UnkCircleShape2D_ld61884(self->transform[0][3], self->transform[1][3], self->transform[2][3],
                                        self->radius);
}
