// Unit 1141 of tools/tu_map.py (no name hint). fn_7100714dd0: clears the word at +0x70 and sets the
// three-bit flag set to "all bits", like fn_71007142e8 (src/game_lo/fn_71007142e8.cpp, whose flag type
// and lazily created statics are repeated here unchanged). Names and layouts are placeholders.
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "types.h"

struct UnkStruct_71007142e8;

struct UnkUnit1141_b68a4 {
    u8 pad_0x0[0x70];
    u32 field_0x70;
};

void fn_7100714dd0(UnkUnit1141_b68a4* self) {
    self->field_0x70 = 0;
    fn_71007143c8(reinterpret_cast<UnkStruct_71007142e8*>(self), UnkFlagsAll_71007142e8());
}
