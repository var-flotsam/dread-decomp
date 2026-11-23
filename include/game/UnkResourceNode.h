#pragma once

#include "base/UnkCountedResource.h"
#include "game/UnkResourceKey.h"

struct UnkResourceNode {
    UnkKeyObject* key;
    UnkCountedResource* value;
    UnkResourceNode* next;
};
