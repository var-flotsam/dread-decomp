// SLogicPathNode unit: moves an entry (0x50 bytes: a vtable, an array of points, a flag and three
// cleared words) into an array of them at an index, shifting the later entries up one place (growing
// the array first when it is full): CRntVector::Insert written out. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game_s2/UnkItem50.h"
#include "types.h"
struct UnkPathPoint {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    void Destroy() { reinterpret_cast<void (*)(UnkPathPoint*)>(vtable[1])(this); }
};

// An array of points (allocation flags 0xf) and a flag; destroyed through vtable slot 1.
struct UnkPathEntry_l05f468 {
    void** vtable;
    base::global::CRntVector<UnkPathPoint> points;
    bool field_0x30;
    u64_a4 field_0x34;
    u64_a4 field_0x3c;
    u64_a4 field_0x44;

    UnkPathEntry_l05f468()
        : vtable(g_unk_7101cac938 + 2), points(0xf), field_0x30(true), field_0x34(0), field_0x3c(0), field_0x44(0) {}
    UnkPathEntry_l05f468(UnkPathEntry_l05f468&& other)
        : vtable(g_unk_7101cac938 + 2), points(static_cast<base::global::CRntVector<UnkPathPoint>&&>(other.points)),
          field_0x30(other.field_0x30), field_0x34(0), field_0x3c(0), field_0x44(0) {}
    ~UnkPathEntry_l05f468() { reinterpret_cast<void (*)(UnkPathEntry_l05f468*)>(vtable[1])(this); }
};
static_assert(sizeof(UnkPathEntry_l05f468) == 0x50, "layout");

using UnkPathEntries_l05f468 = base::global::CRntVector<UnkPathEntry_l05f468>;

// Allocates new_capacity entries and moves the entries over, freeing the old array.
static inline UnkPathEntry_l05f468* MoveTo(UnkPathEntries_l05f468* v, u32 new_capacity) {
    UnkPathEntry_l05f468* p = static_cast<UnkPathEntry_l05f468*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkPathEntry_l05f468), 8, v->flags));
    if (UnkPathEntry_l05f468* src = v->data) {
        UnkPathEntry_l05f468* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkPathEntry_l05f468(static_cast<UnkPathEntry_l05f468&&>(*src));
            src->~UnkPathEntry_l05f468();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_710105f468(UnkPathEntries_l05f468* self, UnkPathEntry_l05f468* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkPathEntry_l05f468* p;
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
        new (&p[index]) UnkPathEntry_l05f468(static_cast<UnkPathEntry_l05f468&&>(*value));
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkPathEntry_l05f468* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        // The entries from the last down to the index move up one place (the index runs through -1 when it is 0).
        for (u32 i = n - 1; i >= index; --i) {
            UnkPathEntry_l05f468* q = &self->data[i];
            new (q + 1) UnkPathEntry_l05f468(static_cast<UnkPathEntry_l05f468&&>(*q));
            self->data[i].~UnkPathEntry_l05f468();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkPathEntry_l05f468(static_cast<UnkPathEntry_l05f468&&>(*value));
    }
    ++self->count;
}