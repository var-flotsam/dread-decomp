// CEmmyMovement's unit (tools/tu_map.py unit 814): fn_7100571660. In state 1 with a sub-state of
// 2, 3 or 4, the value at +0x9e8 picks the sub-state (at least 0.7: 2; below -0.7: 4; else 3),
// which is requested (fn_71010cc214) when it differs. Names and layouts are placeholders.
#include "game/CEnemyMovement.h"
#include "types.h"

struct UnkEffectSink;

struct UnkEmmyMovement_A3_b66 {
    u8 pad_0x0[0x9e8];
    f32 field_0x9e8;
};

static inline u32 Band_A3_b66(f32 v) {
    u32 band = v < 0.7f ? 1 : 0;
    if (v < -0.7f)
        band = 2;
    return band;
}

void fn_7100571660(UnkEmmyMovement_A3_b66* self) {
    if (fn_71010cc3a4(self) != 1)
        return;
    u32 sub = fn_71010cc38c(self);
    if (sub - 2 > 2)
        return;
    UnkEffectSink* sink = reinterpret_cast<UnkEffectSink*>(self);
    switch (Band_A3_b66(self->field_0x9e8)) {
    case 2:
        if (sub != 4)
            fn_71010cc214(sink, 4);
        break;
    case 1:
        if (sub != 3)
            fn_71010cc214(sink, 3);
        break;
    case 0:
        if (sub != 2)
            fn_71010cc214(sink, 2);
        break;
    }
}
