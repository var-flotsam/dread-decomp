// Unit 1176 (hint 'CCentralUnitComponent'): moves an entry (two callback sets, 0x80 bytes) into the unit's
// array of them at an index, shifting the later entries up one place (growing the array first when it is
// full): CRntVector::Insert written out. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU341.h"
#include "types.h"

// The entry's destructor (src/game_lo/fn_7100778228.cpp).
struct UnkPair_7100778228;

struct UnkEntry_l77ab00 {
    UnkHalf_cu first;
    UnkHalf_cu second;
    UnkEntry_l77ab00(UnkEntry_l77ab00&& other)
        : first(static_cast<UnkHalf_cu&&>(other.first)), second(static_cast<UnkHalf_cu&&>(other.second)) {}
};
static_assert(sizeof(UnkEntry_l77ab00) == 0x80, "UnkEntry_l77ab00 layout");

using UnkEntries_l77ab00 = base::global::CRntVector<UnkEntry_l77ab00>;

static inline void Destroy(UnkEntry_l77ab00* entry) {
    fn_7100778228(reinterpret_cast<UnkPair_7100778228*>(entry));
}

// Allocates new_capacity entries and moves the entries over, freeing the old array.
static inline UnkEntry_l77ab00* MoveTo(UnkEntries_l77ab00* v, u32 new_capacity) {
    UnkEntry_l77ab00* p = static_cast<UnkEntry_l77ab00*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry_l77ab00), 8, v->flags));
    if (UnkEntry_l77ab00* src = v->data) {
        UnkEntry_l77ab00* dst = p;
        for (u32 i = 0; i < v->count; ++i) {
            new (dst) UnkEntry_l77ab00(static_cast<UnkEntry_l77ab00&&>(*src));
            Destroy(src);
            ++src;
            ++dst;
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_710077ab00(UnkEntries_l77ab00* self, UnkEntry_l77ab00* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkEntry_l77ab00* p;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        new (&p[index]) UnkEntry_l77ab00(static_cast<UnkEntry_l77ab00&&>(*value));
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkEntry_l77ab00* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkEntry_l77ab00* q = &self->data[i];
            new (q + 1) UnkEntry_l77ab00(static_cast<UnkEntry_l77ab00&&>(*q));
            Destroy(&self->data[i]);
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkEntry_l77ab00(static_cast<UnkEntry_l77ab00&&>(*value));
    }
    ++self->count;
}
