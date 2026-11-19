// Unit 811 (CEmmyAIComponent hint): a point (w = 1) scaled by a value of the object at +0x29ee0; the
// base point depends on the AI state (a table entry 0 or 1, else zero). Names and layouts are placeholders.
#include "game/UnkEmmyScaledPoint.h"
#include "game/UnkTU811.h"
#include "types.h"

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
UnkEmmyPoint4 fn_7100551e4c(UnkEmmyScaledAI* self) {
    f32 x = 0.0f, y = 0.0f, z = 0.0f;
    if (fn_710054ad2c(reinterpret_cast<UnkEmmyAIView*>(self)) ||
        fn_710054aecc(reinterpret_cast<UnkEmmyAI2_b62a3*>(self)) ||
        fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self))) {
        const UnkEmmyBasePoint* p = fn_7100572110(0);
        x = p->x; y = p->y; z = p->z;
    } else if (fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self))) {
        const UnkEmmyBasePoint* p = fn_7100572110(1);
        x = p->x; y = p->y; z = p->z;
    }
    f32 s = self->source->vf_0xd8()->scale;
    return UnkEmmyPoint4(x * s, y * s, z * s);
}
