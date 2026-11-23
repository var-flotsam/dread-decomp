// The destructor of fn_7100e4a1d0 (src/game_aca57c/fn_7100e4fd1c.cpp) for a 0x38-byte record without
// the leading words: two string ids and an array of string ids (base::global::CRntVector's layout):
// fn_7100004a44.
#include <new>

#include "base/UnkResType.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkReleasedStrIdArray_004a44 {
    UnkStrIdTwoStep* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkReleasedStrIdArray_004a44() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkStrIdTwoStep();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkIdsRecord38 {
    UnkStrIdTwoStep id_0x0;
    UnkStrIdTwoStep id_0x8;
    UnkReleasedStrIdArray_004a44 ids;

    __attribute__((always_inline)) ~UnkIdsRecord38() {}
};
static_assert(sizeof(UnkIdsRecord38) == 0x38, "UnkIdsRecord38 layout");

void fn_7100004a44(UnkResFormat* self) { reinterpret_cast<UnkIdsRecord38*>(self)->~UnkIdsRecord38(); }

