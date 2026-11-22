#pragma once

#include "types.h"

template <class T>
struct __attribute__((packed)) UnkPacked {
    T v;
};
