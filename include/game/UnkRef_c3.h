#pragma once

#include "game/UnkRefObj_c3.h"
#include "types.h"

struct UnkRef_c3 {
    UnkRefObj_c3* p;
    ~UnkRef_c3() {
        if (p && --p->refs == 0)
            p->vf_0x10();
    }
};
