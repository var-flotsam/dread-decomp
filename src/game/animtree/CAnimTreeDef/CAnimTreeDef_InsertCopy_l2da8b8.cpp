// animtree::CAnimTreeDef's unit (tools/tu_map.py unit 401): copies a pair of string ids in at an index of
// an array of them (base::global::CRntVector's layout), moving the later pairs up one place. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/animtree/CAnimTreeDef.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdPair_l2da8b8 {
    CStrId first;
    CStrId second;

    // Takes over the other's ids into storage that holds none; the other is left with empty ones.
    void MoveFrom(UnkIdPair_l2da8b8& other) {
        CStrId::MoveConstructReloading(&first, other.first);
        CStrId::MoveConstructReloading(&second, other.second);
    }
};

struct UnkIdPairArray_l2da8b8 {
    UnkIdPair_l2da8b8* data;
    u32 count;
    u32 capacity;
    u32 step;

    void Reserve(u32 n) { fn_71002da674(reinterpret_cast<UnkAnimTreeIdPairs_b49a1*>(this), n); }
};

void fn_71002da8b8(UnkIdPairArray_l2da8b8* self, const UnkIdPair_l2da8b8* value, u32 index) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (n == index) {
        if (capacity <= n && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        // A capacity that does not grow past the index hangs, as in the original.
        if ((s32)capacity <= (s32)index)
            for (;;) {
            }
        self->Reserve(capacity);
        new (&self->data[self->count]) UnkIdPair_l2da8b8(*value);
    } else {
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        self->Reserve(capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkIdPair_l2da8b8* q = &self->data[i];
            q[1].MoveFrom(*q);
            self->data[i].~UnkIdPair_l2da8b8();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkIdPair_l2da8b8(*value);
    }
    ++self->count;
}
