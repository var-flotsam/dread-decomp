// CPowerBombMovement unit (tools/tu_map.py): the reset. The bomb movement's base reset runs, then the
// flag at +0x4ec is cleared, +0x4e8 set to 0.1, and the counter and the five vectors from +0x4f4 zeroed.
// Names and layouts are placeholders.
#include "game/CBombMovement.h"
#include "types.h"

struct CGoliathXBurstProjectionBombMovement;

struct UnkPowerBombVec_laadd14 {
    f32 x;
    f32 y;
    f32 z;

    void Zero() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
};

struct UnkPowerBombMovement_laadd14 {
    u8 pad_0x0[0x4c0];
    u32 field_0x4c0;
    u8 pad_0x4c4[0x24];
    f32 field_0x4e8;
    bool field_0x4ec;
    u8 pad_0x4ed[0x7];
    UnkPowerBombVec_laadd14 field_0x4f4[5];
};
static_assert(__builtin_offsetof(UnkPowerBombMovement_laadd14, field_0x4f4) == 0x4f4, "layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100aadd14(UnkPowerBombMovement_laadd14* self) {
    fn_7100aaa024(reinterpret_cast<CGoliathXBurstProjectionBombMovement*>(self));
    self->field_0x4ec = false;
    self->field_0x4e8 = 0.1f;
    self->field_0x4c0 = 0;
    for (UnkPowerBombVec_laadd14& v : self->field_0x4f4)
        v.Zero();
}
