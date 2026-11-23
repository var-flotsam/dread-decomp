#pragma once

#include "types.h"

struct UnkPathValue;

struct UnkPathNode {
    void* key;
    UnkPathValue* value;
    UnkPathNode* next;
};
