#pragma once

#include "base/global/CRntVector.h"

// Three vectors and a flag; element types are unknown.
struct UnkItemLifeLists {
    base::global::CRntVector<void*> lists[3];
    bool field_0x78;

    UnkItemLifeLists() : lists{base::global::CRntVector<void*>(0xf), base::global::CRntVector<void*>(0xf), base::global::CRntVector<void*>(0xf)} {
        field_0x78 = false;
    }
};
