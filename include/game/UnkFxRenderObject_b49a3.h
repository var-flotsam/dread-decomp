#pragma once

#include "types.h"

// A render object: its item for the render manager follows the vtable.
struct UnkFxRenderObject_b49a3 {
    void** vtable;
    u8 item[0x8];
};
