#pragma once

#include "game/UnkGameBlackboard.h"
#include "types.h"

// The two properties as the unit's readers see them (a named definition); here they are built as
// UnkPropertyEx<0x160>.
struct UnkMapNamedDef_ovb0203 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
