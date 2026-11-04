// CPowerBombMovement's unit: true for kinds 0, 1, 2 and 5. Names are placeholders.
#include "types.h"

bool fn_7100ab1b4c(void* self, u32 kind) {
    switch (kind) {
    case 0:
    case 1:
    case 2:
    case 5:
        return true;
    default:
        return false;
    }
}
