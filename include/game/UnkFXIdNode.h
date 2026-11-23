#pragma once

#include "base/global/CStrId.h"

struct UnkFXIdNode {
    base::global::CStrId id;
    // Union members: Clang gives union accesses no type-based alias info, so the links are
    // reloaded after each store as in the original.
    union {
        UnkFXIdNode* next;
    };
    union {
        UnkFXIdNode* prev;
    };
};
