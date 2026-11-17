// SDropProbabilities' unit (tools/tu_map.py; the name from reflection): one of two float settings
// of the droppable tunable g_unk_71727c10c8 (fTimeSpawningMelee when the state at +0x84 is 2, else fTimeSpawning).
// Names are placeholders.
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkDroppableTunable.h"
#include "types.h"

struct UnkDropOwner_b45a2 {
    u8 pad_0x0[0x84];
    s32 state;
};

f32 fn_71007c7e4c(UnkDropOwner_b45a2* self) {
    return self->state == 2 ? g_unk_71727c10c8.fTimeSpawningMelee : g_unk_71727c10c8.fTimeSpawning;
}
