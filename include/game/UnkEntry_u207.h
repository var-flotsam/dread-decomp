#pragma once

#include "base/global/CStrId.h"
#include "types.h"

struct UnkEntry_u207 {
    void* target;
    base::global::CStrId id;
    UnkEntry_u207(const UnkEntry_u207& other) : target(other.target), id(other.id) {}
};
