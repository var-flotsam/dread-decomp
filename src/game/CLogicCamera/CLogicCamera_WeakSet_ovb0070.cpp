// CLogicCamera's unit (tools/tu_map.py unit 2655): a copy of fn_710023df08
// (src/game/CActor/CActor_WeakVariantSet_b62a1.cpp): points a two-state value at the weak handle of
// `target` (state 2) or clears it (state 0). Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkWeakVariant_ovb0070 {
    u8 field_0x0[0x30];
    u32 state;  // 0x30
    UnkWeakHandle* handle;  // 0x38
};

void fn_71010524c0(UnkWeakVariant_ovb0070* self, UnkWeakTarget* target) {
    if (target) {
        AssignRef(&self->handle, GetWeakHandle(target));
        self->state = 2;
    } else {
        ResetRef(&self->handle);
        self->state = 0;
    }
}
