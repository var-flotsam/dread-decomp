#pragma once

#include "base/UnkDestructors.h"
#include "types.h"

// UnkListedPath and UnkFilePathList as src/base/file/CPackManagerDevice.cpp defines them.
// A listed path (a node of UnkFilePathList).
struct UnkListedPath {
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath* next;
};
