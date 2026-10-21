// Destructor body: tears down the object's trees, pooled arrays, strings and string ids in
// reverse member order, then runs the base-class destructor.
#include "types.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id member; its release decrements and tests the count in two steps.
struct UnkStrIdField {
    UnkStringData* data;

    void Release() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkAllocator {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10();
    virtual void vf18();
    virtual void vf20();
    virtual void vf28();
    virtual void vf30();
    virtual void vf38();
    virtual void* Alloc(u64 size, u64 align, u32 flags);
    virtual void Free(void* p);
};

struct UnkTree;
void fn_71004f3ea4(void* root, UnkTree* tree);
void fn_71009d3dc4(void* root, UnkTree* tree);

struct UnkTree {
    void* root;
    void* field_0x8;
    void* field_0x10;

    void Clear() {
        if (root) {
            fn_71009d3dc4(root, this);
            root = nullptr;
        }
        field_0x8 = nullptr;
        field_0x10 = nullptr;
    }
};

struct UnkPool {
    UnkTree* data;
    void* field_0x8;
    u32 count;
    u8 pad_0x14[0xc];
    UnkAllocator* allocator;
};

struct UnkObj_71009cf440 {
    void* vtable;
    u8 pad_0x8[0x90];
    UnkStrIdField field_0x98;
    UnkStrIdField field_0xa0;
    u8 pad_0xa8[0x10];
    UnkStrIdField field_0xb8;
    u8 field_0xc0[0x30];
    u8 field_0xf0[0x30];
    UnkStrIdField field_0x120;
    u8 pad_0x128[0x10];
    UnkPool field_0x138;
    u8 pad_0x160[0x10];
    UnkStrIdField field_0x170;
    UnkPool field_0x178;
    UnkStrIdField field_0x1a0;
    u8 pad_0x1a8[0x10];
    UnkTree field_0x1b8;
    u8 field_0x1d0[0x10];
};

extern void* g_unk_7101a89358[];
void fn_71009cef20(void* p);
void fn_71004f11f8(UnkPool* p);
void fn_710007ecd4(void* p);
void fn_71009cd080(UnkObj_71009cf440* self);

void fn_71009cf440(UnkObj_71009cf440* self) {
    self->vtable = g_unk_7101a89358 + 2;
    fn_71009cef20(self->field_0x1d0);
    if (self->field_0x1b8.root) {
        fn_71004f3ea4(self->field_0x1b8.root, &self->field_0x1b8);
        self->field_0x1b8.root = nullptr;
    }
    self->field_0x1b8.field_0x8 = nullptr;
    self->field_0x1b8.field_0x10 = nullptr;
    self->field_0x1a0.Release();
    fn_71004f11f8(&self->field_0x178);
    if (self->field_0x178.field_0x8)
        self->field_0x178.allocator->Free(self->field_0x178.field_0x8);
    if (self->field_0x178.data)
        self->field_0x178.allocator->Free(self->field_0x178.data);
    self->field_0x170.Release();
    if (self->field_0x138.data) {
        for (u64 i = 0; i < self->field_0x138.count; ++i) {
            self->field_0x138.data[i].Clear();
        }
    }
    self->field_0x138.count = 0;
    if (self->field_0x138.field_0x8)
        self->field_0x138.allocator->Free(self->field_0x138.field_0x8);
    if (self->field_0x138.data)
        self->field_0x138.allocator->Free(self->field_0x138.data);
    self->field_0x120.Release();
    fn_710007ecd4(self->field_0xf0);
    fn_710007ecd4(self->field_0xc0);
    self->field_0xb8.Release();
    self->field_0xa0.Release();
    self->field_0x98.Release();
    fn_71009cd080(self);
}
