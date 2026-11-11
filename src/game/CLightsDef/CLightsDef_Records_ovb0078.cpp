// CLightsDef's unit (tools/tu_map.py): the unit's array (base::UnkVector's layout) of 0x60-byte records
// with two string ids at +0x8 and +0x10 (src/game/CLightsDef/CLightsDef_b42.cpp has its setters):
// its reallocation (fn_7100da1508), its shrinking resize (fn_7100da0870) and the move-insert
// (fn_7100da18e4), base::UnkVector's code with the record moved by fn_7100d9b18c and its ids released
// in two steps. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CLightsDef.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// As src/game/CLightsDef/CLightsDef_b42.cpp declares them.
struct UnkLightsElem_b42a3 {
    u8 unk_0x0[0x60];
};

struct UnkLightsArray_b42a3 {
    UnkLightsElem_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// The element's move constructor (src/game_aca57c/fn_7100d9b18c.cpp).
struct UnkIdRecord60;
void fn_7100d9b18c(UnkIdRecord60* self, UnkIdRecord60* other);

// A string id released in two steps (the count is decremented, then reread).
struct UnkLightsId_ovb0078 {
    UnkStringData* data;

    __attribute__((always_inline)) void Release() {
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

// The element as the reallocation sees it: moved by fn_7100d9b18c, its two ids released after.
struct UnkLightsRecord_ovb0078 {
    u8 pad_0x0[0x8];
    UnkLightsId_ovb0078 field_0x8;
    UnkLightsId_ovb0078 field_0x10;
    u8 pad_0x18[0x48];

    UnkLightsRecord_ovb0078(UnkLightsRecord_ovb0078&& other) {
        fn_7100d9b18c(reinterpret_cast<UnkIdRecord60*>(this), reinterpret_cast<UnkIdRecord60*>(&other));
    }
    ~UnkLightsRecord_ovb0078() {
        field_0x10.Release();
        field_0x8.Release();
    }
};

void fn_7100da1508(UnkLightsArray_b42a3* self, u32 capacity) {
    reinterpret_cast<base::UnkVector<UnkLightsRecord_ovb0078>*>(self)->Reserve(capacity);
}

// As src/game/CLightManager/fn_7100e0ccf8.cpp declares it.
struct UnkClearedArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Shrinks the array to `count` records (it never grows it).
void fn_7100da0870(UnkClearedArray* self, u32 count) {
    if (self->count > count) {
        for (u64 i = count; i < self->count; ++i)
            static_cast<UnkLightsRecord_ovb0078*>(self->data)[i].~UnkLightsRecord_ovb0078();
    }
    self->count = count;
}

static inline UnkLightsRecord_ovb0078* Records(UnkLightsArray_b42a3* self) {
    return reinterpret_cast<UnkLightsRecord_ovb0078*>(self->data);
}

// Moves `value` in at `index`, shifting the later records up (base::UnkVector::Insert, with the
// reallocation out of line).
void fn_7100da18e4(UnkLightsArray_b42a3* self, UnkLightsRecord_ovb0078* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        fn_7100da1508(self, new_capacity);
        index = self->count;
    } else {
        if (cap <= n && self->step != 0)
            cap = (n / self->step + 1) * self->step;
        fn_7100da1508(self, cap);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkLightsRecord_ovb0078* q = &Records(self)[i];
            new (q + 1) UnkLightsRecord_ovb0078(static_cast<UnkLightsRecord_ovb0078&&>(*q));
            Records(self)[i].~UnkLightsRecord_ovb0078();
            if (i == 0)
                break;
        }
    }
    new (&Records(self)[index]) UnkLightsRecord_ovb0078(static_cast<UnkLightsRecord_ovb0078&&>(*value));
    ++self->count;
}
