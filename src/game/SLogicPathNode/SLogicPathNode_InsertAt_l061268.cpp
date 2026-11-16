// SLogicPathNode unit: inserts a point at an index of an array of 0x20-byte polymorphic points, moving
// the later ones up one place (growing the array first when it is full); inserting at the end appends
// through fn_7100e4ea08. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/SLogicPathNode.h"
#include "game/UnkTU2563.h"
#include "types.h"

// Two words of a point, copied as one.
struct UnkPathPointTail {
    u32 field_0x0;
    u32 field_0x4;
};
struct UnkPathPointEntry {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    UnkPathPointTail tail;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    void Destroy() { reinterpret_cast<void (*)(UnkPathPointEntry*)>(vtable[1])(this); }
};

struct UnkPathPointEntryArray {
    UnkPathPointEntry* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Reallocate(u32 new_capacity) {
        UnkPathPointEntry* p = static_cast<UnkPathPointEntry*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkPathPointEntry), 8, flags));
        if (data) {
            UnkPathPointEntry* src = data;
            UnkPathPointEntry* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                dst->vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
                dst->field_0x8 = src->field_0x8;
                dst->field_0xc = src->field_0xc;
                dst->tail.field_0x0 = src->tail.field_0x0;
                dst->tail.field_0x4 = src->tail.field_0x4;
                dst->field_0x18 = src->field_0x18;
                src->Destroy();
            }
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }
};

static inline void CopyPoint(UnkPathPointEntry* dst, const UnkPathPointEntry* src) {
    dst->vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
    dst->field_0x8 = src->field_0x8;
    dst->field_0xc = src->field_0xc;
    dst->tail.field_0x0 = src->tail.field_0x0;
    dst->tail.field_0x4 = src->tail.field_0x4;
    dst->field_0x18 = src->field_0x18;
}

void fn_7101061268(UnkPathPointEntryArray* self, const UnkPathPointEntry* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_7100e4ea08(reinterpret_cast<UnkPolyVector*>(self), reinterpret_cast<const UnkPolyValues*>(value));
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0) {
        u32 grown = (n / self->step + 1) * self->step;
        if (cap < grown)
            self->Reallocate(grown);
    }
    // The points from the last down to the index move up one place (the index runs through -1 when it is 0).
    for (u32 i = self->count - 1; i >= index; --i) {
        UnkPathPointEntry* q = &self->data[i];
        q[1].vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
        q[1].field_0x8 = q->field_0x8;
        q[1].field_0xc = q->field_0xc;
        q[1].tail = q->tail;
        q[1].field_0x18 = q->field_0x18;
        self->data[i].Destroy();
        if (i == 0)
            break;
    }
    CopyPoint(&self->data[index], value);
    ++self->count;
}
