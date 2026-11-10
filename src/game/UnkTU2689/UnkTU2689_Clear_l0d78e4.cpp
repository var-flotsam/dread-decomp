// Unit 2689: clears three allocator-backed arrays (the first with 4-byte elements that are
// destructed one by one) and frees their storage. Names and layouts are placeholders.
#include "types.h"
#include "base/UnkEmptyDtorElem.h"

struct UnkAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void Free(void* p);
};

struct UnkBufferSet {
    void* data0;
    u32 count0;
    u32 cap0;
    u8 pad_0x10[0x10];
    UnkAllocator* alloc0;
    u8 pad_0x28[0x20];
    void* data48;
    u32 count48;
    u32 cap48;
    u8 pad_0x58[0x10];
    UnkAllocator* alloc68;
    u8 pad_0x70_[0];
    u8* data70;
    u32 count78;
    u32 cap7c;
    u8 pad_0x80[0x10];
    UnkAllocator* alloc90;
    u32 field_0x98;
};

void fn_71010d78e4(UnkBufferSet* self) {
    self->field_0x98 = 0;
    u8* data = self->data70;
    if (data) {
        if (self->count78) {
            fn_71002874dc(data);
            for (u64 i = 1; i < self->count78; i++) {
                fn_71002874dc(self->data70 + i * 4);
            }
            asm volatile("" ::: "memory");
            data = self->data70;
        }
        UnkAllocator* alloc = self->alloc90;
        self->count78 = 0;
        alloc->Free(data);
        self->cap7c = 0;
    }
    void* data48 = self->data48;
    if (data48) {
        UnkAllocator* alloc = self->alloc68;
        self->count48 = 0;
        alloc->Free(data48);
        self->cap48 = 0;
    }
    void* data0 = self->data0;
    if (data0) {
        UnkAllocator* alloc = self->alloc0;
        self->count0 = 0;
        alloc->Free(data0);
        self->cap0 = 0;
    }
}
