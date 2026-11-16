// SLogicPathNode unit: inserts a copy of a point at an index of an array of 0x20-byte polymorphic
// points, moving the later ones up one place (growing the array first when it is full):
// CRntVector::Insert written out. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/SLogicPathNode.h"
#include "types.h"
extern char g_unk_7101cac8f0[];

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

// Allocates new_capacity points and moves the points over, freeing the old array.
static inline UnkPathPointEntry* MoveTo(UnkPathPointEntryArray* v, u32 new_capacity) {
    UnkPathPointEntry* p = static_cast<UnkPathPointEntry*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkPathPointEntry), 8, v->flags));
    if (UnkPathPointEntry* src = v->data) {
        UnkPathPointEntry* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            CopyPoint(dst, src);
            src->Destroy();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_710106146c(UnkInsertTarget* target, const UnkArgObj* obj, u32 value) {
    UnkPathPointEntryArray* self = reinterpret_cast<UnkPathPointEntryArray*>(target);
    const UnkPathPointEntry* point = reinterpret_cast<const UnkPathPointEntry*>(obj);
    u32 index = value;
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkPathPointEntry* p;
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
        CopyPoint(&p[index], point);
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkPathPointEntry* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        // The points from the last down to the index move up one place (the index runs through -1 when it is 0).
        for (u32 i = n - 1; i >= index; --i) {
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
        CopyPoint(&self->data[index], point);
    }
    ++self->count;
}