// Constructor of a pair of empty string ids (each creating the string pool if there is none).
// Two identical copies.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkEmptyIdPair {
    UnkEmptyStrId first;
    UnkEmptyStrId second;

    __attribute__((always_inline)) UnkEmptyIdPair() {}
};

void fn_7100ade7dc(UnkEmptyIdPair* self) { new (self) UnkEmptyIdPair; }
void fn_7100c8fa60(UnkEmptyIdPair* self) { new (self) UnkEmptyIdPair; }
