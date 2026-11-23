#pragma once

#include "base/global/CFilePathStrId.h"
#include "types.h"

// CFilePathStrId's default constructor, inlined.
struct UnkEmptyPathId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkEmptyPathId() {
        data = nullptr;
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
};
