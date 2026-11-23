#pragma once

#include "game/UnkTU2640.h"

struct UnkPartFfca00 {
    u64 unk_0x0[5];
    UnkPartFfca00& operator=(const UnkPartFfca00& other) {
        fn_7100ffbad8(this, &other);
        return *this;
    }
};
