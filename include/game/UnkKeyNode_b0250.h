#pragma once

#include "game/UnkTU39.h"
#include "types.h"

struct UnkKeyNode_b0250 {
    base::global::CStrId id;
    f32 value;
    f32 value2;
    UnkKeyNode_b0250* next;
    UnkKeyNode_b0250(const UnkKeyNode_b0250* src) : id(src->id) {
        value = src->value;
        next = nullptr;
    }
};
