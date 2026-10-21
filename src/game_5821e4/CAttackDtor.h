#pragma once

// Destructor parts shared by CAttack's subclasses, written with explicit vtable stores (the
// vtables are the original's, reached through the GOT).

#include "base/global/CStrId.h"
#include "types.h"

struct UnkAttackOwner {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    s32 ref_count;
    void* field_0x10;
};

void fn_710117b3f0(void* owner, void* listener);  // removes a listener from its owner
void fn_71003606c0(void* attack);                 // CAttack's destructor

extern void* g_unk_7101a22d48[];  // the listener's vtable

// The embedded listener's destructor: unregister from the owner, then release it. Most use
// vtable 0x7101a22d48 and fn_710117b3f0; a few have their own.
struct UnkAttackListenerRaw {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    UnkAttackOwner* owner;

    void Destroy(void** vt = g_unk_7101a22d48, void (*unregister)(void*, void*) = fn_710117b3f0) {
        vtable = vt + 2;
        if (UnkAttackOwner* o = owner) {
            if (o->field_0x10)
                unregister(o->field_0x10, this);
            if (UnkAttackOwner* p = owner) {
                if (--p->ref_count == 0)
                    p->Delete();
            }
        }
    }
};

struct UnkAttackAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* Alloc(u64 size, u64 align, u32 flags);
    virtual void Free(void* p);
};

// A growable array of ref-counted pointers (UnkVector's layout): its destructor releases each
// element, then frees the storage.
struct UnkAttackRefArray {
    UnkAttackOwner** data;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    UnkAttackAllocator* allocator;

    void Destroy() {
        if (data) {
            for (int i = 0; i < count; i++) {
                if (UnkAttackOwner* p = data[i]) {
                    if (--p->ref_count == 0)
                        p->Delete();
                }
            }
            count = 0;
            allocator->Free(data);
            capacity = 0;
        }
    }
};

// A string id member; its release decrements and tests the count in two steps.
struct UnkAttackStrIdRaw {
    base::global::UnkStringData* data;

    void Release() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
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

struct UnkAttackRefPtrRaw {
    UnkAttackOwner* ptr;

    void Release() {
        if (UnkAttackOwner* p = ptr) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};
