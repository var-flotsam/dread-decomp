#pragma once

#include "base/UnkKeyedStrIds.h"
#include "game/UnkTimedHead_ovb0071.h"
#include "types.h"

struct UnkTimedEntry_ovb0071;

struct UnkTimedEntry_ovb0071 {
    UnkTimedHead_ovb0071 head;
    UnkKeyedStrIds properties;  // 0x8

    UnkTimedEntry_ovb0071(UnkTimedEntry_ovb0071&& other)
        : head(other.head), properties(static_cast<UnkKeyedStrIds&&>(other.properties)) {}
};
