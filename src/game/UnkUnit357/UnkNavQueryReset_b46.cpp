// The navigation query's unit (unit 357 of tools/tu_map.py): resets the query's result: the flag at
// +0x80 and the point at +0x84 cleared, the cell at +0x8c invalid (0xffff), and the inline list of
// 12-byte hits at +0x94 (count at +0x90) destroyed one by one (fn_710028760c) and emptied. Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

// Signature as src/game/CAIComponent.cpp declares it.
struct UnkElem_7100286624;
void fn_710028760c(UnkElem_7100286624* self);

struct UnkNavHit_b46a1 {
    u8 data[0xc];
};

struct UnkNavQueryResult_b46a1 {
    u8 pad_0x0[0x80];
    bool field_0x80;
    u8 pad_0x81[3];
    CVector2D point;
    u16 cell;
    u8 pad_0x8e[2];
    u32 count;
    UnkNavHit_b46a1 hits[1];
};

void fn_71002869a8(UnkNavQueryResult_b46a1* self) {
    self->field_0x80 = false;
    self->point.x = 0.0f;
    self->point.y = 0.0f;
    self->cell = 0xffff;
    for (u64 i = 0; i < self->count; ++i)
        fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(&self->hits[i]));
    self->count = 0;
}
