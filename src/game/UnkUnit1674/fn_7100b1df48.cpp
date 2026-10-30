// From unit 1674 of tools/tu_map.py (0x7100b1df48-0x7100b1e70c, no reflected class): the constructor of a slots
// object (game/UnkRenderPassSlots.h), out of line as in the render pass list's unit.
#include <new>

#include "game/UnkRenderPassSlots.h"

void fn_7100b1df48(UnkRenderPassSlots* self) {
    new (self) UnkRenderPassSlots();
}
