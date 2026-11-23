#pragma once

#include "base/UnkAssignedString.h"

struct UnkValueString {
    u32 value;
    UnkAssignedString string;

    UnkValueString& operator=(const UnkValueString& other) {
        value = other.value;
        string = other.string;
        return *this;
    }
};
