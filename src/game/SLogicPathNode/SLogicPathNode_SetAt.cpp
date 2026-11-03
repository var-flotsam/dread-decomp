// SLogicPathNode unit: two "set element `index`" functions of an array of 0x20-byte polymorphic
// elements (vtable g_unk_7101cac8f0, five words from +0x8, destroyed through slot 1) laid out as
// base::UnkVector (include/base/UnkVector.h). An index past the end grows the array to the next
// multiple of the step (the elements moved over, the old array freed) and resizes it to index + 1
// (fn_710106111c); then the element's first three words, or all five, are assigned. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101cac8f0[];

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

struct UnkPathPointArray {
    UnkPathPoint* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Reallocate(u32 new_capacity) {
        UnkPathPoint* p = static_cast<UnkPathPoint*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkPathPoint), 8, flags));
        if (data) {
            UnkPathPoint* src = data;
            UnkPathPoint* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                dst->vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
                dst->field_0x8 = src->field_0x8;
                dst->field_0xc = src->field_0xc;
                dst->field_0x10 = src->field_0x10;
                dst->field_0x14 = src->field_0x14;
                dst->field_0x18 = src->field_0x18;
                src->Destroy();
            }
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }
};

void fn_710106111c(UnkPathPointArray* self, u32 count);

static inline UnkPathPoint& GrowTo(UnkPathPointArray* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        if (self->capacity <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (self->capacity < grown)
                self->Reallocate(grown);
        }
        fn_710106111c(self, index + 1);
    }
    return self->data[index];
}

void fn_710105df58(UnkPathPointArray* self, const u32* index, const UnkPathPoint* value) {
    UnkPathPoint& e = GrowTo(self, *index);
    e.field_0x8 = value->field_0x8;
    e.field_0xc = value->field_0xc;
    e.field_0x10 = value->field_0x10;
}

void fn_710105e0c0(UnkPathPointArray* self, const u32* index, const UnkPathPoint* value) {
    UnkPathPoint& e = GrowTo(self, *index);
    e.field_0x8 = value->field_0x8;
    e.field_0xc = value->field_0xc;
    e.field_0x10 = value->field_0x10;
    e.field_0x14 = value->field_0x14;
    e.field_0x18 = value->field_0x18;
}
