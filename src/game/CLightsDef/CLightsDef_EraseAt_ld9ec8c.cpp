// CLightsDef's unit (tools/tu_map.py): removes the 0x60-byte record at an index of the unit's array
// (base::global::CRntVector's layout): it is destroyed and the later records move down. Names and layouts
// are placeholders.
#include <new>

#include "base/global/CRntVector.h"
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
struct UnkLightsId_ld9ec8c {
    UnkStringData* data;

    void Release() {
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

struct UnkLightsRecord_ld9ec8c {
    u8 pad_0x0[0x8];
    UnkLightsId_ld9ec8c field_0x8;
    UnkLightsId_ld9ec8c field_0x10;
    u8 pad_0x18[0x48];

    UnkLightsRecord_ld9ec8c(UnkLightsRecord_ld9ec8c&& other) {
        fn_7100d9b18c(reinterpret_cast<UnkIdRecord60*>(this), reinterpret_cast<UnkIdRecord60*>(&other));
    }
    ~UnkLightsRecord_ld9ec8c() {
        field_0x10.Release();
        field_0x8.Release();
    }
};

static inline UnkLightsRecord_ld9ec8c* Records(UnkLightsArray_b42a3* self) {
    return reinterpret_cast<UnkLightsRecord_ld9ec8c*>(self->data);
}

// As src/game/CLightsDef/UnkEraseAtIter_A6_b67.cpp declares it: the same array.
struct UnkCollection_Lights_A6_b67;

// Destroys the record at `index` and moves the later ones down by one.
void fn_7100d9ec8c(UnkCollection_Lights_A6_b67* collection, u32 index) {
    UnkLightsArray_b42a3* self = reinterpret_cast<UnkLightsArray_b42a3*>(collection);
    if (index < self->count) {
        Records(self)[index].~UnkLightsRecord_ld9ec8c();
        for (u64 i = index; i < self->count - 1; ++i) {
            UnkLightsRecord_ld9ec8c* at = &Records(self)[i];
            new (at) UnkLightsRecord_ld9ec8c(static_cast<UnkLightsRecord_ld9ec8c&&>(at[1]));
            (&Records(self)[i])[1].~UnkLightsRecord_ld9ec8c();
        }
        self->count = self->count - 1;
    }
}
