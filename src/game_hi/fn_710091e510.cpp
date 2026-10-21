// Destructor body: releases the two callback lists (each with a ref-counted owner), the pooled
// arrays and string ids, resets the five embedded sub-objects, then runs the base destructor.
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

struct UnkCallback;

struct UnkCallbackOps {
    u8 pad_0x0[0x40];
    void (*destroy)(UnkCallback* cb);
};

struct UnkCallback {
    void* field_0x0;
    UnkCallbackOps* ops;
};

struct UnkRefCounted {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    s32 ref_count;
};

extern void* g_unk_7101a82960[];
extern void* g_unk_7101a33fa0[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a82c58[];
extern void* g_unk_7101a82c00[];

// A list of callbacks (0x40 bytes) whose derived type also holds a ref-counted owner.
struct UnkCallbackList {
    void* vtable;
    UnkCallback* data;
    u32 count;
    u32 capacity;
    u8 pad_0x18[0x10];
    UnkAllocator* allocator;
    u8 pad_0x30[0x8];
    UnkRefCounted* owner;

    void Destroy() {
        vtable = g_unk_7101a33fa0 + 2;
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
        vtable = g_unk_7101a20998 + 2;
        if (data) {
            for (u64 i = 0; i < count; ++i) {
                UnkCallback& cb = data[i];
                if (cb.ops) {
                    cb.ops->destroy(&cb);
                    cb.ops = nullptr;
                }
            }
            count = 0;
            allocator->Free(data);
            capacity = 0;
        }
    }
};

struct UnkPool {
    void* data;
    void* field_0x8;
    u32 count;
    u8 pad_0x14[0xc];
    UnkAllocator* allocator;
};

void fn_71004f11f8(UnkPool* p);
void fn_710002f67c(void* p);
void fn_71000a1c60(void* p);

// An embedded sub-object (0x70 bytes) with its own vtable.
struct UnkSub {
    void* vtable;
    u8 pad_0x8[0x10];
    u8 field_0x18[0x18];
    void* field_0x30[8];

    void Destroy() {
        field_0x30[7] = nullptr;
        field_0x30[6] = nullptr;
        field_0x30[5] = nullptr;
        field_0x30[4] = nullptr;
        field_0x30[3] = nullptr;
        field_0x30[2] = nullptr;
        field_0x30[1] = nullptr;
        field_0x30[0] = nullptr;
        vtable = g_unk_7101a82c00 + 2;
        fn_71000a1c60(field_0x18);
    }
};

struct UnkSub2 {
    void* vtable;
    u8 pad_0x8[0x8];
    void* field_0x10;
    void* field_0x18;
    u32 field_0x20;

    void Destroy() {
        vtable = g_unk_7101a82c58 + 2;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        field_0x20 = 0;
    }
};

struct UnkObj_710091e510 {
    void* vtable;
    u8 pad_0x8[0x48];
    UnkSub field_0x50;
    UnkSub field_0xc0;
    UnkSub field_0x130;
    UnkSub field_0x1a0;
    UnkSub field_0x210;
    UnkSub2 field_0x280;
    u8 pad_0x2a8[0xb0];
    u8 field_0x358[0xc0];
    UnkStrIdField field_0x418;
    u8 pad_0x420[0x8];
    UnkCallbackList field_0x428;
    u8 pad_0x468[0x8];
    UnkStrIdField field_0x470;
    UnkPool field_0x478;
    UnkCallbackList field_0x4a0;
};

void fn_71010c4194(UnkObj_710091e510* self);

void fn_710091e510(UnkObj_710091e510* self) {
    self->vtable = g_unk_7101a82960 + 2;
    self->field_0x4a0.Destroy();
    fn_71004f11f8(&self->field_0x478);
    if (self->field_0x478.field_0x8)
        self->field_0x478.allocator->Free(self->field_0x478.field_0x8);
    if (self->field_0x478.data)
        self->field_0x478.allocator->Free(self->field_0x478.data);
    self->field_0x470.Release();
    self->field_0x428.Destroy();
    self->field_0x418.Release();
    fn_710002f67c(self->field_0x358);
    self->field_0x280.Destroy();
    self->field_0x210.Destroy();
    self->field_0x1a0.Destroy();
    self->field_0x130.Destroy();
    self->field_0xc0.Destroy();
    self->field_0x50.Destroy();
    fn_71010c4194(self);
}
