#pragma once

// The two storage types every unit's static initialiser starts with (src/game_5821e4/UnkStaticInit.h has the
// helpers that build them): the header ids each unit repeats, and a file path id's storage. Here so that a
// unit's statics layout can live in a header. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

namespace unk_static_init {

// The header statics every unit repeats.
struct HeaderIds {
    u32 field_0x0;
    base::global::UnkStrIdBase field_0x8;
    base::global::UnkStrIdBase field_0x10;
};

// Storage of a file path id (its destructor is registered separately).
struct UnkFilePathStrIdBase {
    base::global::UnkStringData* data;
};

}  // namespace unk_static_init
