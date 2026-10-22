// Constructor of a record that starts with a 4x4 identity matrix (floats 0, 5, 10 and 15 are one)
// followed by a string id left as the empty string. The zero floats are written as 8-byte words.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

typedef u64 u64_a4 __attribute__((aligned(4)));

struct UnkMatrixRecord {
    f32 m00;
    u8 pad_0x4[0x3c];
    UnkStringData* field_0x40;

    UnkMatrixRecord() {
        char* p = reinterpret_cast<char*>(this);
        *reinterpret_cast<u64_a4*>(p + 0x24) = 0x3f80000000000000ull;
        *reinterpret_cast<u64_a4*>(p + 0x4) = 0;
        *reinterpret_cast<u64_a4*>(p + 0xc) = 0;
        *reinterpret_cast<u64_a4*>(p + 0x1c) = 0;
        *reinterpret_cast<u64_a4*>(p + 0x2c) = 0;
        *reinterpret_cast<u64_a4*>(p + 0x34) = 0;
        *reinterpret_cast<u64_a4*>(p + 0x14) = 0x3f800000ull;
        *reinterpret_cast<f32*>(p + 0x3c) = 1.0f;
        m00 = 1.0f;
        field_0x40 = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        field_0x40 = &g_unk_7101d47108->empty;
    }
};

// NON_MATCHING(minor): the zero words are stored in ascending instead of descending offset order
void fn_7100569e58(UnkMatrixRecord* self) {
    new (self) UnkMatrixRecord();
}
