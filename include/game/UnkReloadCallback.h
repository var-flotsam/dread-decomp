#pragma once

#include "types.h"

// A load's callback: a context and a function.
struct UnkReloadCallback {
    void* context;
    void* fn;
};
