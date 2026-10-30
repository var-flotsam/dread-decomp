// From unit 314 of tools/tu_map.py (0x710021812c-0x71002182bc; its static initialiser only builds
// the header ids): the constructor of the render pass list's slots object (game/UnkRenderPassSlots.h).
#include <new>

#include "game/UnkRenderPassSlots.h"

// The constructor, out of line (the caller allocates the object).
void fn_710021812c(UnkRenderPassSlots* self) {
    new (self) UnkRenderPassSlots();
}
