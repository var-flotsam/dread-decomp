#pragma once

#include "types.h"

struct UnkTunableGroup;

// What a group lists first: objects whose first word points back at the group.
struct UnkTunableMember {
    UnkTunableGroup* owner;
};
