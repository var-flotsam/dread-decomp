#pragma once

#include "types.h"

// The tunable object a header names: its class is vtable slot 0.
struct UnkTunableObject {
    virtual char* GetClass();
};
