#pragma once

#include "game/UnkCounted_A5_b67.h"

struct UnkCountedRef_A5_b67 {
    UnkCounted_A5_b67* p;
    UnkCountedRef_A5_b67(const UnkCountedRef_A5_b67& other) : p(nullptr) {
        if (UnkCounted_A5_b67* q = other.p) {
            ++q->count;
            p = q;
        }
    }
};
