#pragma once

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// A node of an object's named tree (0x98 bytes): its payload, name and child nodes.
struct UnkNamedNode {
    u8 pad_0x0[0x60];
    void* payload;
    base::global::CStrId name;
    base::global::CRntVector<UnkNamedNode*> children;
};
