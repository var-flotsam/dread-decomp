#pragma once

#include "game_s2/UnkWeakHandle.h"

// A display object as the scripts hold it: full userdata with the "__DisplayObjectType" metatable
// holding a counted weak handle on the object.
struct UnkLuaDisplayObject_ovb0109 {
    UnkWeakHandle* handle;

    UnkWeakTarget* Get() const { return handle ? handle->target : nullptr; }
};
