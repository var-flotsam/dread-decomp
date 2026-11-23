#pragma once

#include "types.h"

struct UnkHandleTarget;

struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};
