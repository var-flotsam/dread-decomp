// CLightsDef's unit (tools/tu_map.py): copies a 0x60-byte record in at an index of the unit's array
// (base::global::CRntVector's layout), moving the later records up one place. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CLightsDef.h"
#include "game/UnkLightsArray_b42a3.h"
#include "game/UnkLightsElem_b42a3.h"
#include "game/UnkTU2528.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The element's move constructor (src/game_aca57c/fn_7100d9b18c.cpp).
struct UnkIdRecord60;

// A string id released in two steps (the count is decremented, then reread).
struct UnkLightsId_lda1730 {
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

struct UnkLightsRecord_lda1730 {
    u8 pad_0x0[0x8];
    UnkLightsId_lda1730 field_0x8;
    UnkLightsId_lda1730 field_0x10;
    u8 pad_0x18[0x48];

    UnkLightsRecord_lda1730(UnkLightsRecord_lda1730&& other) {
        fn_7100d9b18c(reinterpret_cast<UnkIdRecord60*>(this), reinterpret_cast<UnkIdRecord60*>(&other));
    }
    UnkLightsRecord_lda1730(const UnkLightsRecord_lda1730& other) {
        fn_7100d9bffc(reinterpret_cast<UnkLightsElem_b42a3*>(this),
                      reinterpret_cast<const UnkLightsElem_b42a3*>(&other));
    }
    ~UnkLightsRecord_lda1730() {
        field_0x10.Release();
        field_0x8.Release();
    }
};

static inline UnkLightsRecord_lda1730* Records(UnkLightsArray_b42a3* self) {
    return reinterpret_cast<UnkLightsRecord_lda1730*>(self->data);
}

// Copies `value` in at `index`, shifting the later records up (base::global::CRntVector::Insert, with the
// reallocation out of line).
void fn_7100da1730(UnkLightsArray_b42a3* self, const UnkLightsRecord_lda1730* value, u32 index) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    UnkLightsRecord_lda1730* slot;
    if (n == index) {
        if (capacity <= n && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        // A capacity that does not grow past the index hangs, as in the original.
        if ((s32)capacity <= (s32)index)
            for (;;) {
            }
        fn_7100da1508(self, capacity);
        // The end slot, taken from the array's own element type.
        slot = reinterpret_cast<UnkLightsRecord_lda1730*>(&self->data[self->count]);
    } else {
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        fn_7100da1508(self, capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkLightsRecord_lda1730* q = &Records(self)[i];
            new (q + 1) UnkLightsRecord_lda1730(static_cast<UnkLightsRecord_lda1730&&>(*q));
            Records(self)[i].~UnkLightsRecord_lda1730();
            if (i == 0)
                break;
        }
        slot = &Records(self)[index];
    }
    new (slot) UnkLightsRecord_lda1730(*value);
    ++self->count;
}
