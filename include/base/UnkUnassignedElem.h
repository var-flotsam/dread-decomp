#pragma once

#include "types.h"

template <u64 Size>
struct UnkUnassignedElem {
    u64 words[Size / 8];
    UnkUnassignedElem& operator=(const UnkUnassignedElem&) { return *this; }
};
