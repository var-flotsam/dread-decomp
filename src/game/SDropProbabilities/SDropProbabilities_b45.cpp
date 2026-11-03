// SDropProbabilities' unit (tools/tu_map.py; the name from reflection): one of two float settings
// of the droppable tunable g_unk_71727c10c8 (+0x68 when the state at +0x84 is 2, else +0x64).
// Names are placeholders.
#include "types.h"

// The tunable (CTunableDroppableBillboard's instance), declared as src/game/CLogicDroppedItemManager.cpp
// does; this layout adds the two settings read here to the counts that file reads.
struct UnkDroppableTunable {
    u8 pad_0x0[0x64];
    f32 field_0x64;
    f32 field_0x68;
    u8 pad_0x6c[0x24];
    u32 field_0x90;
    u32 field_0x94;
};
extern UnkDroppableTunable g_unk_71727c10c8;

struct UnkDropOwner_b45a2 {
    u8 pad_0x0[0x84];
    s32 state;
};

f32 fn_71007c7e4c(UnkDropOwner_b45a2* self) {
    return self->state == 2 ? g_unk_71727c10c8.field_0x68 : g_unk_71727c10c8.field_0x64;
}
