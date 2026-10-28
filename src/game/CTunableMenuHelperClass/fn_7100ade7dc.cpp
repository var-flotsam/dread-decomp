// Constructor of a pair of empty string ids (each creating the string pool if there is none).
// Two identical copies.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkEmptyId {
    UnkStringData* data;

    __attribute__((always_inline)) UnkEmptyId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkEmptyIdPair {
    UnkEmptyId first;
    UnkEmptyId second;

    __attribute__((always_inline)) UnkEmptyIdPair() {}
};

void fn_7100ade7dc(UnkEmptyIdPair* self) { new (self) UnkEmptyIdPair; }
void fn_7100c8fa60(UnkEmptyIdPair* self) { new (self) UnkEmptyIdPair; }
