#pragma once

// UnkResFormat (base/UnkResType.h) with its members' own types, as its constructor
// (fn_7100004578) and the resource type's constructor (fn_7100004698) build and copy it.

#include "base/UnkCloneRecords.h"
#include "base/UnkResType.h"
#include "base/global/CStrId.h"

struct UnkResFormatMembers {
    base::global::CStrId class_name;
    base::global::CStrId extension;  // 0x8
    base::UnkStrIdVec sources;       // 0x10
};
static_assert(sizeof(UnkResFormatMembers) == sizeof(UnkResFormat), "UnkResFormatMembers layout");
