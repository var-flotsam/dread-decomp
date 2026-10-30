// From unit 2734 of tools/tu_map.py (0x710119d484-0x710119ea2c, no reflected class): the
// constructor of the sub-area GUI cache (game/UnkSubAreaGuiCache.h), a slots object as in the render
// pass list's unit.
#include <new>

#include "game/UnkSubAreaGuiCache.h"

void fn_710119d484(UnkSubAreaGuiCache* self) {
    new (self) UnkRenderPassSlots();
}
