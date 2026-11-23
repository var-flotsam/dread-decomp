#pragma once

#include "types.h"

struct UnkTunables;

struct UnkProvider {
    void** vtable;
    UnkTunables* vf_0xd8() { return reinterpret_cast<UnkTunables* (*)(UnkProvider*)>(vtable[27])(this); }
};
