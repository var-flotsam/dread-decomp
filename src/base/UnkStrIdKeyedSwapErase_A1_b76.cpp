// Swap-erase from a flat map of 8-byte values (+0) and string-id keys (+8), count at +0x10: the
// erased key is released, the last value and key are moved into the freed slot (the moved-from key
// left empty, then released), and the slot's value is answered. fn_710021d6ec (unit 322) and its
// identical copy fn_71002332e0 (unit 341). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A key as the erase moves it (the b75 navmesh reserve's UnkKeyId): the moved-from id is cleared
// through a union, then released in two steps.
union UnkSwapKeySlotA1 {
    UnkStringData* p;
};

struct UnkSwapKeyA1 {
    UnkStringData* data;

    UnkSwapKeyA1(UnkSwapKeyA1&& other) : data(other.data) {
        reinterpret_cast<UnkSwapKeySlotA1*>(&other.data)->p = &g_unk_7101d47108->empty;
    }
    ~UnkSwapKeyA1() {
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

struct UnkSwapMapA1 {
    u64* values;
    UnkSwapKeyA1* keys;
    u32 count;

    __attribute__((always_inline)) u64* Erase(u64* it) {
        u32 index = it - values;
        keys[index].~UnkSwapKeyA1();
        if (index != count - 1) {
            __builtin_memcpy(&values[index], &values[count - 1], sizeof(u64));
            new (&keys[index]) UnkSwapKeyA1(static_cast<UnkSwapKeyA1&&>(keys[count - 1]));
            keys[count - 1].~UnkSwapKeyA1();
        }
        --count;
        return &values[index];
    }
};

u64* fn_710021d6ec(UnkSwapMapA1* self, u64* it) {
    return self->Erase(it);
}

u64* fn_71002332e0(UnkSwapMapA1* self, u64* it) {
    return self->Erase(it);
}
