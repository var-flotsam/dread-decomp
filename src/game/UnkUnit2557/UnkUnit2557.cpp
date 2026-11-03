// Unit 2557 (tools/tu_map.py; no reflected class). Names are placeholders inferred from the code.
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkUnit2557Obj;

bool fn_7100219970(UnkRenderManager* self);
void fn_7100e10150(UnkUnit2557Obj* self);
void fn_7100e10aac(UnkUnit2557Obj* self);

// Switches the object on or off when the render manager's state differs from `on`.
void fn_7100e100f8(UnkUnit2557Obj* self, bool on) {
    if (fn_7100219970(g_unk_7101d47300) == on)
        return;
    if (on)
        fn_7100e10150(self);
    else
        fn_7100e10aac(self);
}
