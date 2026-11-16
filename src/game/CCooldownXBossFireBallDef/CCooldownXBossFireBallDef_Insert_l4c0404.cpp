// CCooldownXBossFireBallDef's unit (tools/tu_map.py): copies an element into its array of 0x18-byte
// polymorphic elements (base/global/CRntVector.h) at an index, shifting the later ones up one place
// and growing the array first when it is full: CRntVector::InsertCopy written out. Names and layouts
// are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CCooldownXBossFireBallDef.h"
#include "types.h"

// As CCooldownXBossFireBallDef_Array_ovb0072.cpp defines the element (the same type).
// A flag, a word and a flag, copied as one 9-byte block (packed: the copy skips the tail padding).
struct __attribute__((packed)) UnkFireBallDefData_ovb0072 {
    bool field_0x0;
    u8 pad_0x1[3];
    f32 field_0x4;
    bool field_0x8;
};

struct UnkFireBallDef_ovb0072;
struct UnkFireBallDefVtable_ovb0072 {
    void* vf_0x0;
    void (*dtor)(UnkFireBallDef_ovb0072* self);
};

struct UnkFireBallDef_ovb0072 {
    const UnkFireBallDefVtable_ovb0072* vtable;
    UnkFireBallDefData_ovb0072 data;  // 0x8

    // The vtable is stored untyped: the original reloads the array after it.
    UnkFireBallDef_ovb0072() {
        const char* vt = g_unk_7101a3e2b0 + 0x10;
        __builtin_memcpy(&vtable, &vt, sizeof(vt));
        data.field_0x0 = true;
        data.field_0x4 = 0.0f;
        data.field_0x8 = false;
    }
    UnkFireBallDef_ovb0072(const UnkFireBallDef_ovb0072& other)
        : vtable(reinterpret_cast<const UnkFireBallDefVtable_ovb0072*>(g_unk_7101a3e2b0 + 0x10)), data(other.data) {}
    UnkFireBallDef_ovb0072(UnkFireBallDef_ovb0072&& other)
        : vtable(reinterpret_cast<const UnkFireBallDefVtable_ovb0072*>(g_unk_7101a3e2b0 + 0x10)), data(other.data) {}
    ~UnkFireBallDef_ovb0072() { vtable->dtor(this); }
};
static_assert(sizeof(UnkFireBallDef_ovb0072) == 0x18, "UnkFireBallDef_ovb0072 layout");

using UnkFireBallDefVector_ovb0072 = base::global::CRntVector<UnkFireBallDef_ovb0072>;

// Allocates new_capacity elements and moves the elements over, freeing the old array.
static inline UnkFireBallDef_ovb0072* MoveTo(UnkFireBallDefVector_ovb0072* v, u32 new_capacity) {
    UnkFireBallDef_ovb0072* p = static_cast<UnkFireBallDef_ovb0072*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkFireBallDef_ovb0072), 8, v->flags));
    if (UnkFireBallDef_ovb0072* src = v->data) {
        UnkFireBallDef_ovb0072* dst = p;
        for (u32 i = 0; i < v->count; ++i) {
            new (dst) UnkFireBallDef_ovb0072(static_cast<UnkFireBallDef_ovb0072&&>(*src));
            src->~UnkFireBallDef_ovb0072();
            ++src;
            ++dst;
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_71004c0404(UnkFireBallDefVector_ovb0072* self, const UnkFireBallDef_ovb0072* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkFireBallDef_ovb0072* p;
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
        new (&p[index]) UnkFireBallDef_ovb0072(*value);
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkFireBallDef_ovb0072* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkFireBallDef_ovb0072* q = &self->data[i];
            new (q + 1) UnkFireBallDef_ovb0072(static_cast<UnkFireBallDef_ovb0072&&>(*q));
            self->data[i].~UnkFireBallDef_ovb0072();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkFireBallDef_ovb0072(*value);
    }
    ++self->count;
}
