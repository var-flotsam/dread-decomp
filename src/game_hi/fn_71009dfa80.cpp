// Destructor body of a large owner object: releases its listeners, ref-counted pointers, node
// list, handler and resource members and calls the destructors of its many embedded members.
#include "base/global/CRntString.h"
#include "types.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id member of the pool *Pool; its release decrements and tests the count in two steps.
template <UnkStringPool** Pool>
struct UnkStrIdField {
    UnkStringData* data;

    void Release() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
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

struct UnkRefCounted {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    s32 ref_count;
};

struct UnkRefPtr {
    UnkRefCounted* ptr;

    void Release() {
        if (UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

void fn_710117b3f0(void* owner, void* listener);
void fn_71002aeb9c(void* root, void* tree);
void fn_710002f67c(void* p);
void fn_7100027044(void* p);
void fn_7100872908(void* p, void* handler);
void fn_71009a1f40(void* p);
void fn_71009b0800(void* p);
void fn_71009b349c(void* p);
void fn_71009a61c8(void* p);
void fn_71009a3fd8(void* p);
void fn_71009978b0(void* p);
void fn_71009afff4(void* p);
void fn_71009a3670(void* p);
void fn_710098e69c(void* p);
void fn_7100991580(void* p);
void fn_710099a084(void* p);
void fn_710098ffe0(void* p);
void fn_7100991d0c(void* p);
void fn_71009b3b20(void* p);
void fn_7100988dfc(void* p);
void fn_71010d7874(void* p);

extern void* g_unk_7101a8a048[];
extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a87000[];
extern void* g_unk_7101a8a498[];
extern void* g_unk_7101a46040[];
extern void* g_unk_7101a22858[];
extern void* g_unk_7101a22878[];

struct UnkListenerOwner : UnkRefCounted {
    void* field_0x10;
};

struct UnkListener {
    void* vtable;
    u8 pad_0x8[0x10];
    UnkListenerOwner* owner;

    void Destroy() {
        vtable = g_unk_7101a22d48 + 2;
        if (UnkListenerOwner* o = owner) {
            if (o->field_0x10)
                fn_710117b3f0(o->field_0x10, this);
            if (UnkListenerOwner* p = owner) {
                if (--p->ref_count == 0)
                    p->Delete();
            }
        }
    }
};

struct UnkTree {
    void* root;
    void* field_0x8;
    void* field_0x10;

    void Destroy() {
        if (root) {
            fn_71002aeb9c(root, this);
            root = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

// Primary and secondary vtables, a ref-counted pointer at +0x1b8.
struct UnkMulti {
    void* vtable;
    u8 pad_0x8[0x180];
    void* vtable2;
    u8 pad_0x190[0x28];
    UnkRefPtr field_0x1b8;

    void Destroy() {
        vtable = g_unk_7101a87000 + 2;
        vtable2 = g_unk_7101a87000 + 48;
        fn_71009a1f40(this);
        field_0x1b8.Release();
        fn_71009b0800(this);
    }
};

struct UnkRecord {
    void* vtable;
    u8 pad_0x8[0x8];
    void* field_0x10;
    void* field_0x18;
    u32 field_0x20;

    void Destroy() {
        vtable = g_unk_7101a8a498 + 2;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        field_0x20 = 0;
    }
};

struct UnkPair {
    UnkRefPtr a;
    UnkRefPtr b;
    u8 pad_0x10[0x38];

    void Destroy() {
        b.Release();
        a.Release();
    }
};

// A fixed-capacity array of pairs (count, then 4-aligned storage).
struct UnkPairArray {
    u32 count;
    u8 storage[32 * sizeof(UnkPair)];

    UnkPair* data() { return reinterpret_cast<UnkPair*>(storage); }

    void Destroy() {
        for (u64 i = 0; i < count; ++i)
            data()[i].Destroy();
        count = 0;
    }
};

struct UnkNode {
    void* value;
    UnkNode* next;
};

struct UnkNodeList {
    void* vtable;
    u32 count;
    UnkNode* head;
    UnkNode* tail;
    UnkAllocator* allocator;

    void Destroy() {
        vtable = g_unk_7101a46040 + 2;
        for (UnkNode* n = head; n;) {
            UnkNode* next = n->next;
            allocator->Free(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkHandlerTarget {
    u8 pad_0x0[0x1d8];
    void* field_0x1d8;
};

struct UnkHandlerSource {
    u8 pad_0x0[0x30];
    UnkHandlerTarget* field_0x30;
};

struct UnkArrayElem {
    u8 data[0x10];
    ~UnkArrayElem() {}
};

struct UnkHandlerDerived {
    void* vtable;
    u8 field_0x8[0x30];
    u8 pad_0x38[0x8];
    UnkArrayElem* field_0x40;
    UnkHandlerSource* field_0x48;

    void Destroy() {
        vtable = g_unk_7101a22858 + 2;
        if (field_0x48 && field_0x48->field_0x30 && field_0x48->field_0x30->field_0x1d8)
            fn_7100872908(field_0x48->field_0x30->field_0x1d8, this);
        field_0x48 = nullptr;
        vtable = g_unk_7101a22878 + 2;
        if (field_0x40) {
            delete[] field_0x40;
            field_0x40 = nullptr;
        }
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(field_0x8));
    }
};

void fn_710001737c(void* p);

struct UnkResData {
    u8 pad_0x0[0x54];
    s32 ref_count;
};

struct UnkResource {
    UnkStrIdField<&g_unk_7101d47108> id;
    UnkStrIdField<&g_unk_7101d47200> path;
    UnkResData* res;

    __attribute__((always_inline)) void Destroy() {
        fn_7100027044(this);
        if (UnkResData* r = res) {
            if (--r->ref_count == 0)
                fn_710001737c(r);
        }
        path.Release();
        id.Release();
    }
};

struct UnkObj_71009dfa80 {
    void* vtable;
    u8 pad_0x8[0x2270];
    UnkResource field_0x2278;
    u8 pad_0x2290[0x18];
    UnkHandlerDerived field_0x22a8;
    u8 pad_0x22f8[0x58];
    UnkNodeList field_0x2350;
    u8 pad_0x2378[0x18];
    UnkRefPtr field_0x2390;
    UnkRefPtr field_0x2398;
    u8 pad_0x23a0[0x38];
    UnkPairArray field_0x23d8;
    u8 pad_0x2cdc[0x1c];
    UnkRecord field_0x2cf8;
    u8 pad_0x2d20[0x88];
    u8 field_0x2da8[0x10];
    u8 pad_0x2db8[0x210];
    u8 field_0x2fc8[0x10];
    u8 pad_0x2fd8[0x198];
    u8 field_0x3170[0x10];
    u8 pad_0x3180[0x2a0];
    u8 field_0x3420[0x10];
    u8 pad_0x3430[0x1a0];
    u8 field_0x35d0[0x10];
    u8 pad_0x35e0[0xba8];
    UnkMulti field_0x4188;
    u8 pad_0x4348[0x28];
    u8 field_0x4370[0x10];
    u8 pad_0x4380[0x178];
    u8 field_0x44f8[0x10];
    u8 pad_0x4508[0x180];
    u8 field_0x4688[0x10];
    u8 pad_0x4698[0x178];
    u8 field_0x4810[0x10];
    u8 pad_0x4820[0x178];
    u8 field_0x4998[0x10];
    u8 pad_0x49a8[0x178];
    u8 field_0x4b20[0x10];
    u8 pad_0x4b30[0x178];
    u8 field_0x4ca8[0x10];
    u8 pad_0x4cb8[0x338];
    u8 field_0x4ff0[0x10];
    u8 pad_0x5000[0x1d8];
    UnkRefPtr field_0x51d8;
    UnkRefPtr field_0x51e0;
    u8 pad_0x51e8[0x58];
    UnkTree field_0x5240;
    u8 pad_0x5258[0x8];
    u8 field_0x5260[0x10];
    u8 pad_0x5270[0x100];
    UnkListener field_0x5370;
    u8 pad_0x5390[0x10];
    u8 field_0x53a0[0x10];
};
static_assert(__builtin_offsetof(UnkObj_71009dfa80, field_0x53a0) == 0x53a0, "layout");

void fn_71009dfa80(UnkObj_71009dfa80* self) {
    self->vtable = g_unk_7101a8a048 + 2;
    fn_710002f67c(self->field_0x53a0);
    self->field_0x5370.Destroy();
    fn_710002f67c(self->field_0x5260);
    self->field_0x5240.Destroy();
    self->field_0x51e0.Release();
    self->field_0x51d8.Release();
    fn_71009b349c(self->field_0x4ff0);
    fn_71009a61c8(self->field_0x4ca8);
    fn_71009a3fd8(self->field_0x4b20);
    fn_71009978b0(self->field_0x4998);
    fn_71009afff4(self->field_0x4810);
    fn_71009a3670(self->field_0x4688);
    fn_710098e69c(self->field_0x44f8);
    fn_7100991580(self->field_0x4370);
    self->field_0x4188.Destroy();
    fn_710099a084(self->field_0x35d0);
    fn_710098ffe0(self->field_0x3420);
    fn_7100991d0c(self->field_0x3170);
    fn_71009b3b20(self->field_0x2fc8);
    fn_7100988dfc(self->field_0x2da8);
    self->field_0x2cf8.Destroy();
    self->field_0x23d8.Destroy();
    self->field_0x2398.Release();
    self->field_0x2390.Release();
    self->field_0x2350.Destroy();
    self->field_0x22a8.Destroy();
    self->field_0x2278.Destroy();
    fn_71010d7874(self);
}
