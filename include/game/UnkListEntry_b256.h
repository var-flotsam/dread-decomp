#pragma once

#include "types.h"

struct UnkListEntry_b256 {
    void* value;
    UnkListEntry_b256* next;
};
