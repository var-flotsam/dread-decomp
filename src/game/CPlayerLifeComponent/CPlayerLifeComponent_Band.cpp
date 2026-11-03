// CPlayerLifeComponent unit (2716 in tools/tu_map.py): which band the value at +0x54 falls in:
// 2 up to 25, 1 up to 50, 0 above. Names are placeholders.
#include "types.h"

struct UnkPlayerLifeBand {
    u8 pad_0x0[0x54];
    f32 field_0x54;
};

u32 fn_7101165c40(UnkPlayerLifeBand* self) {
    f32 value = self->field_0x54;
    if (value <= 25.0f)
        return 2;
    return value <= 50.0f;
}
