// UnkTU367: resize a vector of 0x38-byte entries (grow capacity via allocator, default-init new entries).
// Names and layouts are placeholders.
#include "types.h"

struct UnkAllocator367 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* Alloc(u64 size, u32 align, u32 tag);
    virtual void Free(void* p);
};

struct UnkEntry367 {
    u8 field_0x0;
    u8 pad_0x1;
    u16 field_0x2;
    u8 field_0x4;
    u8 pad_0x5[3];
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 pad_0x14[4];
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
};

struct UnkEntryVec367 {
    UnkEntry367* data;
    u32 size;
    u32 capacity;
    u8 pad_0x10[8];
    u32 tag;
    u8 pad_0x1c[4];
    UnkAllocator367* allocator;
};

void fn_71002b0e78(UnkEntryVec367* self, u32 newSize) {
    if (self->capacity < newSize) {
        UnkEntry367* fresh = (UnkEntry367*)self->allocator->Alloc((u64)newSize * sizeof(UnkEntry367), 8, self->tag);
        UnkEntry367* src = self->data;
        if (src) {
            UnkEntry367* dst = fresh;
            for (u32 i = 0; i < self->size; i++) {
                __builtin_memcpy(dst, src, 8);
                dst->field_0x8 = src->field_0x8;
                dst->field_0xc = src->field_0xc;
                dst->field_0x10 = src->field_0x10;
                __builtin_memcpy(&dst->field_0x18, &src->field_0x18, 32);
                src++;
                dst++;
            }
            self->allocator->Free(self->data);
        }
        self->data = fresh;
        self->capacity = newSize;
    }
    for (u32 i = self->size; i < newSize; i++) {
        UnkEntry367* e = &self->data[i];
        e->field_0x0 = 0xff;
        e->field_0x2 = 0xffff;
        e->field_0x4 = 0xff;
        e->field_0x8 = 0;
        e->field_0xc = 0;
        e->field_0x10 = 0;
        e->field_0x18 = 0;
        e->field_0x20 = 0;
        e->field_0x28 = 0;
        e->field_0x30 = 0;
    }
    self->size = newSize;
}
