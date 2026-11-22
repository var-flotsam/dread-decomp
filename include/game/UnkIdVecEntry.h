#pragma once

#include "base/UnkCloneRecords.h"
#include "base/global/UnkTwoStepStrId.h"

struct UnkIdVecEntry {
    UnkMovedStrId key;
    base::UnkStrIdVec ids;

    __attribute__((always_inline)) UnkIdVecEntry(UnkIdVecEntry&& other)
        : key(static_cast<UnkMovedStrId&&>(other.key)), ids(other.ids) {}
    __attribute__((always_inline)) UnkIdVecEntry(UnkMovedStrId&& key, const base::UnkStrIdVec& ids)
        : key(static_cast<UnkMovedStrId&&>(key)), ids(ids) {}
};
