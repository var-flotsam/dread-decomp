// Unit of 0x7100ec78a0 (tools/tu_map.py, no name hint): when the flag at +8 is set, reads +0x30 of the game
// manager's blackboard section named by g_unk_71729ce198 (0 when there is none). Names are placeholders.
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2571.h"
#include "types.h"

struct UnkBlackboardReader_b47a3 {
    u8 unk_0x0[8];
    bool enabled;
};

u32 fn_7100ec78a0(UnkBlackboardReader_b47a3* self) {
    if (!self->enabled)
        return 0;
    CBlackboard::CSection* section = fn_71010472e0(&g_unk_7101d47310->field_0x968, g_unk_71729ce198);
    if (!section)
        return 0;
    return section->data.properties.count;
}
