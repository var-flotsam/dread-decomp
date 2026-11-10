// base::animation::CAnimationResource's unit (unit 11 of tools/tu_map.py): the destructor of the
// resource type registry (base/UnkResType.h): a string id, an array of 0x38-byte type descriptions
// (each destroyed by fn_7100004a44) and two arrays of string ids (base::UnkVector's layout),
// destroyed in reverse order. Names and layouts are placeholders.
#include "base/UnkResType.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkAnimStrId_ovb0033 {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkAnimStrId_ovb0033() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

template <class T>
struct UnkAnimArray_ovb0033 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkAnimArray_ovb0033() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~T();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkAnimRecord_ovb0033 {
    u8 data[0x38];

    __attribute__((always_inline)) ~UnkAnimRecord_ovb0033() {
        fn_7100004a44(reinterpret_cast<unk_static_init::UnkResType*>(this));
    }
};

struct UnkAnimDesc_ovb0033 {
    UnkAnimStrId_ovb0033 name;                           // 0x0
    UnkAnimArray_ovb0033<UnkAnimRecord_ovb0033> records;  // 0x8
    UnkAnimArray_ovb0033<UnkAnimStrId_ovb0033> ids_a;     // 0x30
    UnkAnimArray_ovb0033<UnkAnimStrId_ovb0033> ids_b;     // 0x58

    __attribute__((always_inline)) ~UnkAnimDesc_ovb0033() {}
};
static_assert(sizeof(UnkAnimDesc_ovb0033) == 0x80, "UnkAnimDesc_ovb0033 layout");

void fn_7100004b94(void* registry) {
    static_cast<UnkAnimDesc_ovb0033*>(registry)->~UnkAnimDesc_ovb0033();
}
