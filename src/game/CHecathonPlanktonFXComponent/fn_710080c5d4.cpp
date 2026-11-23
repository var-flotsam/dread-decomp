// CHecathonPlanktonFXComponent (tools/tu_map.py unit 1271): starts the effect. Every one of the ten
// emitters (created when missing) is switched on, the component is marked active and, on the first
// start of a pending fade, records `value` and the fade step; `value` is stored last.
// Names and layouts are placeholders.
#include <new>

#include "game/CHecathonPlanktonFXComponent.h"
#include "game/UnkFixedVector_l808234.h"
#include "game/UnkPlanktonPoint_l808234.h"
#include "types.h"

struct UnkModel;

struct UnkPlanktonEmitter_l808234 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    f32 field_0x10 = -1.0f;
    u32 field_0x14;  // unset in the original
    UnkModel* model = nullptr;  // 0x18
    u64 field_0x20 = 0;
    UnkFixedVector_l808234<UnkPlanktonPoint_l808234, 34> points;  // 0x28
    u32 field_0x1c4 = 0;
    bool field_0x1c8 = false;
    bool field_0x1c9 = false;
    bool field_0x1ca = false;

    UnkPlanktonEmitter_l808234() {}
};
static_assert(sizeof(UnkPlanktonEmitter_l808234) == 0x1d0, "emitter size");

struct UnkPlanktonFX_A3_b71 {
    u8 pad_0x0[0x50];
    UnkFixedVector_l808234<UnkPlanktonEmitter_l808234, 10> emitters;  // 0x50
    u8 pad_0x1274[0x10];
    f32 field_0x1284;
    bool active;  // 0x1288
    bool field_0x1289;
    bool field_0x128a;
    f32 field_0x128c;
    f32 field_0x1290;
};
static_assert(__builtin_offsetof(UnkPlanktonFX_A3_b71, field_0x1290) == 0x1290, "layout");

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_710080c5d4(UnkPlanktonFX_A3_b71* self, f32 value) {
    for (u32 i = 0; i < 10; ++i)
        self->emitters.At(i).field_0x1c9 = true;
    self->active = true;
    if (!self->field_0x1289 && self->field_0x128a) {
        self->field_0x1289 = true;
        self->field_0x1290 = value;
        self->field_0x1284 = 0.01f;
    }
    self->field_0x128c = value;
}
