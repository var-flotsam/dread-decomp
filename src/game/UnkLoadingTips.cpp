// The loading-screen tips (unit 2559): fn_7100e17d14 (src/game_s7/fn_7100e17d14.cpp) registers each
// tip with fn_7100e1b2e0, which appends a 0x18-byte record to the list: the tip's id, its
// category, the two value lists folded into bit masks and a float. Names and layouts are
// placeholders.
#include <initializer_list>
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdInline;

// A tip. Its implicit copy copies the id, then the other fields as one block.
struct UnkTip {
    CStrId id;
    u32 category;
    u32 mask_a;
    u32 mask_b;
    f32 field_0x14;

    UnkTip() : id(UnkStrIdInline(), nullptr, false) {}
};
static_assert(sizeof(UnkTip) == 0x18, "UnkTip placeholder layout");

// The list (an UnkVector-shaped array of tips at +0).
struct UnkTipList {
    UnkTip* data;
    u32 count;
    u32 capacity;
    u32 step;
};
// Grows the array to `capacity`.
void fn_7100e1b76c(UnkTipList* self, u32 capacity);

// One bit per listed value.
static inline u32 Mask(std::initializer_list<u32> values) {
    u32 mask = 0;
    for (u32 v : values)
        mask |= 1u << v;
    return mask;
}

void fn_7100e1b2e0(UnkTipList* self, const CStrId& id, u32 category, std::initializer_list<u32> a,
                   std::initializer_list<u32> b, f32 unk) {
    UnkTip tip;
    tip.id = id;
    tip.mask_a = Mask(a);
    tip.mask_b = Mask(b);
    tip.category = category;
    tip.field_0x14 = unk;
    u32 n = self->count;
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_7100e1b76c(self, new_capacity);
    new (&self->data[self->count]) UnkTip(tip);
    ++self->count;
}
