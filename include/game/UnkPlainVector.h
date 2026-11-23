#pragma once

#include "base/UnkCollectionIter.h"

// The same for an array of vectors of plain values (an owned vector is freed).
struct UnkPlainVector : base::global::CRntVector<u32> {
    ~UnkPlainVector() { Free(); }
};
