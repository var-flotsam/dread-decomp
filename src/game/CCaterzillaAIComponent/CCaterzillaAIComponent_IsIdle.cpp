// CCaterzillaAIComponent: a state query over two flags and a byte at +0x146dc (+0x0 and +0x3d).
// Names are placeholders.
#include "types.h"

bool fn_71003f6ff8(char* self) {
    if (self[0x146f9])
        return false;
    const char* p = self + 0x146dc;
    if (!p[0])
        return false;
    return p[0x3d] == 0;
}
