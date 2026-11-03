// CEmmyProtoAIComponent's unit (tools/tu_map.py unit 791): runs fn_71005417b0, then clears the two
// words at +0x720 and +0x728 of the game manager (include/game/UnkGameManager.h, inside its
// placeholder padding). Names are placeholders.
#include "game/UnkGameManager.h"

// The two words, seen through a layout of this file's own.
struct UnkGameManagerWords791 {
    u8 pad_0x0[0x720];
    void* field_0x720;
    void* field_0x728;
};

// fn_71005417b0 (defined in src/lifted as `char* fn_71005417b0(char*)`), called for its effect only:
// bound to that symbol with a void result.
void RunUnit791(void* self) asm("_Z13fn_71005417b0Pc");

void fn_71004fb4c4(void* self) {
    RunUnit791(self);
    UnkGameManagerWords791* manager = reinterpret_cast<UnkGameManagerWords791*>(g_unk_7101d47310);
    manager->field_0x720 = 0;
    manager->field_0x728 = 0;
}
