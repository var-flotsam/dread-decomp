// CHydrogigaPolypsAttack's unit (tools/tu_map.py unit 877): switches the polyp model's glow (fn_71005b63fc): the
// alpha of the material entry found by the first hash (fn_7100146d0c) is set to 1 or 0 after reading its
// colour (fn_71001934e0), and three more entries get the lit colour (2, 0, 0, 1) or the idle one
// (0.36, 0.5, 1, 1), each converted to a vector (fn_7100030878) and set through fn_7100193410. Names and
// layouts are placeholders.
#include "base/color/CColor4F.h"
#include "game/UnkTU211.h"
#include "types.h"

using base::color::CColor4F;

struct UnkEntryListA1_134;
void fn_71001934e0(UnkEntryListA1_134* list, u32 index, u32 slot, CColor4F* out, u32 flag);

struct UnkPolypsModelHolder_ovb0090 {
    u8 pad_0x0[0x78];
    UnkEntryListA1_134* list;  // 0x78
};

struct UnkPolypsModel_ovb0090 {
    u8 pad_0x0[0x160];
    UnkPolypsModelHolder_ovb0090* holder;  // 0x160
};

// Finds the material entry named by `hash` (fn_7100146d0c).
bool fn_7100146d0c(UnkPolypsModel_ovb0090* model, u64 hash, s32* index);

static inline void SetColor(UnkPolypsModel_ovb0090* model, u64 hash, s32* index, bool on, const CColor4F* lit,
                            const CColor4F* off) {
    if (fn_7100146d0c(model, hash, index)) {
        s32 i = *index;
        const CColor4F* color = on ? lit : off;
        UnkEntryListA1_134* list = model->holder->list;
        fn_7100193410(list, i, 0, fn_7100030878(color), 1);
    }
}

void fn_71005b63fc(void* self, UnkPolypsModel_ovb0090* model, bool on) {
    s32 index = -1;
    CColor4F color(1.0f, 1.0f, 1.0f, 1.0f);
    if (fn_7100146d0c(model, 0x3bf7c05ac2b83cc9, &index)) {
        s32 i = index;
        fn_71001934e0(model->holder->list, i, 3, &color, 1);
        color.a = on ? 1.0f : 0.0f;
        i = index;
        UnkEntryListA1_134* list = model->holder->list;
        fn_7100193410(list, i, 3, fn_7100030878(&color), 1);
    }
    CColor4F off(0.36f, 0.5f, 1.0f, 1.0f);
    CColor4F lit(2.0f, 0.0f, 0.0f, 1.0f);
    SetColor(model, 0xffd16593b3fbfec8, &index, on, &lit, &off);
    SetColor(model, 0x75879c5c4d04a1ba, &index, on, &lit, &off);
    SetColor(model, 0xbdc7728a2dac7228, &index, on, &lit, &off);
}
