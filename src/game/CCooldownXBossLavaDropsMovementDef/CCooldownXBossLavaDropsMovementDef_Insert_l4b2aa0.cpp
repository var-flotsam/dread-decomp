// CCooldownXBossLavaDropsMovementDef's unit (tools/tu_map.py): inserts an element (0x18 bytes: vtable
// unk_7101a3e068 and three words) into a base::global::CRntVector of them at an index, shifting the
// later ones up (growing the array first when it is full): CRntVector::Insert written out. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CCooldownXBossLavaDropsMovementDef.h"
#include "types.h"

struct UnkLavaDropsData_l4b0dd8 {
    u32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkLavaDropsElem_l4b0dd8;
struct UnkLavaDropsElemVtable_l4b0dd8 {
    void* vf_0x0;
    void (*dtor)(UnkLavaDropsElem_l4b0dd8* self);
};

struct UnkLavaDropsElem_l4b0dd8 {
    const UnkLavaDropsElemVtable_l4b0dd8* vtable;
    UnkLavaDropsData_l4b0dd8 data;  // 0x8

    UnkLavaDropsElem_l4b0dd8(const UnkLavaDropsElem_l4b0dd8& other)
        : vtable(reinterpret_cast<const UnkLavaDropsElemVtable_l4b0dd8*>(g_unk_7101a3e068 + 0x10)), data(other.data) {}
    UnkLavaDropsElem_l4b0dd8(UnkLavaDropsElem_l4b0dd8&& other)
        : vtable(reinterpret_cast<const UnkLavaDropsElemVtable_l4b0dd8*>(g_unk_7101a3e068 + 0x10)), data(other.data) {}
    ~UnkLavaDropsElem_l4b0dd8() { vtable->dtor(this); }
};
static_assert(sizeof(UnkLavaDropsElem_l4b0dd8) == 0x18, "UnkLavaDropsElem_l4b0dd8 layout");

using UnkLavaDropsVector_l4b0dd8 = base::global::CRntVector<UnkLavaDropsElem_l4b0dd8>;

// Allocates new_capacity elements and moves the elements over, freeing the old array.
static inline UnkLavaDropsElem_l4b0dd8* MoveTo(UnkLavaDropsVector_l4b0dd8* v, u32 new_capacity) {
    UnkLavaDropsElem_l4b0dd8* p = static_cast<UnkLavaDropsElem_l4b0dd8*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkLavaDropsElem_l4b0dd8), 8, v->flags));
    if (UnkLavaDropsElem_l4b0dd8* src = v->data) {
        UnkLavaDropsElem_l4b0dd8* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkLavaDropsElem_l4b0dd8(static_cast<UnkLavaDropsElem_l4b0dd8&&>(*src));
            src->~UnkLavaDropsElem_l4b0dd8();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_71004b2aa0(UnkLavaDropsVector_l4b0dd8* self, UnkLavaDropsElem_l4b0dd8* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkLavaDropsElem_l4b0dd8* p;
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
        new (&p[index]) UnkLavaDropsElem_l4b0dd8(static_cast<UnkLavaDropsElem_l4b0dd8&&>(*value));
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkLavaDropsElem_l4b0dd8* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkLavaDropsElem_l4b0dd8* q = &self->data[i];
            new (q + 1) UnkLavaDropsElem_l4b0dd8(static_cast<UnkLavaDropsElem_l4b0dd8&&>(*q));
            self->data[i].~UnkLavaDropsElem_l4b0dd8();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkLavaDropsElem_l4b0dd8(static_cast<UnkLavaDropsElem_l4b0dd8&&>(*value));
    }
    ++self->count;
}
