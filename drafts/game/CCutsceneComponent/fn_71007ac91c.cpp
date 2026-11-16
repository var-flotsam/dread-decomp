// CCutsceneComponent's unit: moves a 0x88-byte polymorphic entry into an array of them at an index, shifting the
// later entries up one place (growing the array first when it is full): CRntVector::Insert written out.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CCutsceneComponent.h"
#include "types.h"

struct UnkCutsceneEntry_A5_b68 {
    virtual void vf_0x0();
    virtual ~UnkCutsceneEntry_A5_b68();
    u8 pad_0x8[0x80];
};

using UnkCutsceneEntries_l7ac91c = base::global::CRntVector<UnkCutsceneEntry_A5_b68>;

// Allocates new_capacity entries and moves the entries over, freeing the old array.
static inline UnkCutsceneEntry_A5_b68* MoveTo(UnkCutsceneEntries_l7ac91c* v, u32 new_capacity) {
    UnkCutsceneEntry_A5_b68* p = static_cast<UnkCutsceneEntry_A5_b68*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkCutsceneEntry_A5_b68), 8, v->flags));
    if (UnkCutsceneEntry_A5_b68* src = v->data) {
        UnkCutsceneEntry_A5_b68* dst = p;
        for (u32 i = 0; i < v->count; ++i) {
            fn_71007abba4(dst, src);
            src->~UnkCutsceneEntry_A5_b68();
            ++src;
            ++dst;
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_71007ac91c(UnkCutsceneEntries_l7ac91c* self, UnkCutsceneEntry_A5_b68* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    UnkCutsceneEntry_A5_b68* slot;
    if (n == index) {
        UnkCutsceneEntry_A5_b68* p;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(self, new_capacity);
            u32 last = self->count;
            self->data = p;
            self->capacity = new_capacity;
            slot = &p[last];
        } else {
            slot = &self->data[index];
        }
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkCutsceneEntry_A5_b68* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkCutsceneEntry_A5_b68* q = &self->data[i];
            fn_71007abba4(q + 1, q);
            self->data[i].~UnkCutsceneEntry_A5_b68();
            if (i == 0)
                break;
        }
        slot = &self->data[index];
    }
    fn_71007abba4(slot, value);
    ++self->count;
}
