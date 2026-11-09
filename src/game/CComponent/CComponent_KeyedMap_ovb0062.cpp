// CComponent's unit (tools/tu_map.py): two members of a flat map of 8-byte values (+0) keyed by
// string ids (+8), count at +0x10 and capacity at +0x14. fn_71010c6a10 is its copy assignment;
// fn_71010c6cdc its swap-erase (the identical copy of fn_71002332e0,
// src/base/UnkStrIdKeyedSwapErase_A1_b76.cpp). Names and layouts are placeholders.
#include "base/UnkCloneCalls.h"
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "types.h"

#include <new>

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkKeyedValues_71002eef6c;
using UnkValuesByKey = base::UnkKeyedArray<UnkKeyedValues_71002eef6c, u64, u64>;

struct UnkKeyedStrIdMap_ovb0062 {
    u64* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
};

void fn_71010c6b78(UnkKeyedStrIdMap_ovb0062* self);
void fn_71010c67b4(UnkKeyedStrIdMap_ovb0062* self, u32 capacity);  // the keyed-array reserve

UnkKeyedStrIdMap_ovb0062* fn_71010c6a10(UnkKeyedStrIdMap_ovb0062* self, const UnkKeyedStrIdMap_ovb0062* other) {
    if (self == other)
        return self;
    if (self->keys && self->count)
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].Reset();
    self->count = 0;
    if (other->count != self->capacity) {
        fn_71010c6b78(self);
        fn_71010c67b4(self, other->count);
    }
    __builtin_memcpy(self->values, other->values, (u64)other->count * 8);
    self->count = other->count;
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        // Copy-constructs the key; the pointer is stored as bytes, so the count is reloaded.
        UnkStringData* data = other->keys[i].data;
        __builtin_memcpy(&self->keys[i], &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    return self;
}

// A key as the erase moves it: the moved-from id is cleared through a union, then released in
// two steps.
union UnkSwapKeySlot_ovb0062 {
    UnkStringData* p;
};

struct UnkSwapKey_ovb0062 {
    UnkStringData* data;

    UnkSwapKey_ovb0062(UnkSwapKey_ovb0062&& other) : data(other.data) {
        reinterpret_cast<UnkSwapKeySlot_ovb0062*>(&other.data)->p = &g_unk_7101d47108->empty;
    }
    ~UnkSwapKey_ovb0062() {
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

struct UnkSwapMap_ovb0062 {
    u64* values;
    UnkSwapKey_ovb0062* keys;
    u32 count;

    __attribute__((always_inline)) u64* Erase(u64* it) {
        u32 index = it - values;
        keys[index].~UnkSwapKey_ovb0062();
        if (index != count - 1) {
            __builtin_memcpy(&values[index], &values[count - 1], sizeof(u64));
            new (&keys[index]) UnkSwapKey_ovb0062(static_cast<UnkSwapKey_ovb0062&&>(keys[count - 1]));
            keys[count - 1].~UnkSwapKey_ovb0062();
        }
        --count;
        return &values[index];
    }
};

// Declared with a void return in src/game/CPlaythrough/fn_71002eef6c.cpp; it answers the slot.
u64* fn_71010c6cdc(UnkValuesByKey* a, u64* value) {
    return reinterpret_cast<UnkSwapMap_ovb0062*>(a)->Erase(value);
}
