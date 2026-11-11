// Draft (93.2%): GrowAt of the 16-byte entries; only the fill loop's register choice differs.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkEntry16_ovb0180 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;

    UnkEntry16_ovb0180() : field_0x0(0.0f), field_0x4(-1.0f), field_0x8(-1.0f), field_0xc(-1.0f) {}
    UnkEntry16_ovb0180(const UnkEntry16_ovb0180& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
    UnkEntry16_ovb0180& operator=(const UnkEntry16_ovb0180& other) {
        __builtin_memcpy(this, &other, sizeof(*this));
        return *this;
    }
};
// base::UnkVector::GrowAt on a copy of the array whose data pointer is a union member: the original
// reloads it after each element is initialised.
struct UnkGrowEntries16_ovb0180 {
    union {
        UnkEntry16_ovb0180* data;
    };
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reallocate(u32 new_capacity) {
        UnkEntry16_ovb0180* p = static_cast<UnkEntry16_ovb0180*>(
            allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry16_ovb0180), 8, flags));
        if (data) {
            UnkEntry16_ovb0180* dst = p;
            UnkEntry16_ovb0180* src = data;
            for (u32 i = 0; i < count; ++i, ++dst, ++src)
                new (dst) UnkEntry16_ovb0180(*src);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }
};

UnkEntry16_ovb0180* fn_71002cc378(UnkGrowEntries16_ovb0180* v, u32 index) {
    if (v->field_0x14 && v->count <= index) {
        u32 cap = v->capacity;
        if (cap <= index && v->step != 0) {
            u32 grown = (index / v->step + 1) * v->step;
            if (cap < grown) {
                v->Reallocate(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            v->Reallocate(n);
        for (u32 i = v->count; i < n; ++i)
            new (&v->data[i]) UnkEntry16_ovb0180();
        v->count = n;
    }
    return &v->data[index];
}
