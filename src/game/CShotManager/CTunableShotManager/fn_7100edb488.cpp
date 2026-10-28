// Constructor of a block of eight file-path ids (a copy of fn_7100eda600 for another type), each set
// to the pool's empty id, creating the pool first if needed.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::UnkStringData;

// CFilePathStrId's default constructor, inlined.
struct UnkEmptyPathId {
    UnkStringData* data;

    __attribute__((always_inline)) UnkEmptyPathId() {
        data = nullptr;
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
};

struct UnkPathIds_0x40 {
    UnkEmptyPathId field_0x0, field_0x8, field_0x10, field_0x18, field_0x20, field_0x28, field_0x30,
        field_0x38;

    __attribute__((always_inline)) UnkPathIds_0x40() {}
};

void fn_7100edb488(UnkPathIds_0x40* self) {
    new (self) UnkPathIds_0x40;
}
