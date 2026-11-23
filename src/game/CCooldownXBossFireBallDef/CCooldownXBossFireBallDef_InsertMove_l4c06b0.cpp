// CCooldownXBossFireBallDef's unit (tools/tu_map.py): moves an element into its array of 0x18-byte
// polymorphic elements (base/global/CRntVector.h) at an index, shifting the later ones up one place
// and growing the array first when it is full: CRntVector::InsertMove written out.
#include <new>

#include "base/global/CRntVector.h"
#include "reflected/CCooldownXBossFireBallDef.h"
#include "types.h"

using FireBallDefVector = base::global::CRntVector<CCooldownXBossFireBallDef>;

// Allocates new_capacity elements and moves the elements over, freeing the old array.
static inline CCooldownXBossFireBallDef* MoveTo(FireBallDefVector* v, u32 new_capacity) {
    CCooldownXBossFireBallDef* p = static_cast<CCooldownXBossFireBallDef*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(CCooldownXBossFireBallDef), 8, v->flags));
    if (CCooldownXBossFireBallDef* src = v->data) {
        CCooldownXBossFireBallDef* dst = p;
        for (u32 i = 0; i < v->count; ++i) {
            new (dst) CCooldownXBossFireBallDef(static_cast<CCooldownXBossFireBallDef&&>(*src));
            src->~CCooldownXBossFireBallDef();
            ++src;
            ++dst;
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_71004c06b0(FireBallDefVector* self, CCooldownXBossFireBallDef* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        CCooldownXBossFireBallDef* p;
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
        new (&p[index]) CCooldownXBossFireBallDef(static_cast<CCooldownXBossFireBallDef&&>(*value));
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                CCooldownXBossFireBallDef* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            CCooldownXBossFireBallDef* q = &self->data[i];
            new (q + 1) CCooldownXBossFireBallDef(static_cast<CCooldownXBossFireBallDef&&>(*q));
            self->data[i].~CCooldownXBossFireBallDef();
            if (i == 0)
                break;
        }
        new (&self->data[index]) CCooldownXBossFireBallDef(static_cast<CCooldownXBossFireBallDef&&>(*value));
    }
    ++self->count;
}
