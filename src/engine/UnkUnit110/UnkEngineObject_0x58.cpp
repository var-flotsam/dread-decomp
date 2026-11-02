// From unit 110 of tools/tu_map.py (0x71001259f4-0x7100132c04): the constructor of the 0x58-byte
// object the engine start creates (src/engine/UnkEngineStart.cpp, g_unk_7101d4d818): two flags and
// the render pass list's slots layout (game/UnkRenderPassSlots.h) at +0x10. Names are placeholders.
#include <new>

#include "game/UnkRenderPassSlots.h"

struct UnkEngineObject_0x58 {
    bool field_0x0;
    bool field_0x1;
    u8 unk_0x2[0xe];
    UnkRenderPassSlots slots;
};
static_assert(sizeof(UnkEngineObject_0x58) == 0x58, "UnkEngineObject_0x58 layout");

void fn_7100131e48(UnkEngineObject_0x58* self) {
    new (&self->slots) UnkRenderPassSlots();
    self->field_0x0 = false;
    self->field_0x1 = true;
}
