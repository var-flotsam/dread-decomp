#pragma once

#include "game/UnkTU2553.h"
#include "types.h"

// fn_71005077f8's id type (src/game/CEmmyGrabSamusAttack/fn_71005077f8.cpp): a data pointer with a
// user copy constructor, returned through x8.
struct UnkStrId_71005077f8 {
    base::global::UnkStringData* data;
    UnkStrId_71005077f8(const base::global::UnkStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkStrId_71005077f8(const UnkStrId_71005077f8& other);
};
