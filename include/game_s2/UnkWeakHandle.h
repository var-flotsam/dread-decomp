#pragma once

// Reference-counted objects and the weak handles an object hands out: a 0x18-byte handle (vtable
// g_unk_7101a209e0) that is created on first use and kept in a slot the object reports through
// its vtable (slot 5). The count is in a union (no type-based alias info: the original reloads the
// slots after count updates).

#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a209e0[];

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};

struct UnkWeakTarget;

struct UnkWeakHandle : UnkRefObject {
    UnkWeakTarget* target;
};

// An object that can be pointed at through a weak handle.
struct UnkWeakTarget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkWeakHandle** vf_0x28();  // the object's weak handle slot
};

template <class T>
static inline void AssignRef(T** slot, T* p) {
    if (*slot != p) {
        if (p)
            ++p->ref_count;
        if (T* old = *slot) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        *slot = p;
    }
}

// Drops what `slot` refers to.
template <class T>
static inline void ResetRef(T** slot) {
    if (T* p = *slot) {
        if (--p->ref_count == 0)
            p->vf_0x10();
        *slot = nullptr;
    }
}

// The weak handle of `target`, created on first use.
__attribute__((always_inline)) static inline UnkWeakHandle* GetWeakHandle(UnkWeakTarget* target) {
    UnkWeakHandle** slot = target->vf_0x28();
    if (!slot)
        return nullptr;
    UnkWeakHandle* handle = *slot;
    if (!handle) {
        handle = static_cast<UnkWeakHandle*>(fn_71000839a0(0x18, 0x3e));
        *reinterpret_cast<void***>(handle) = &g_unk_7101a209e0[2];
        handle->ref_count = 0;
        handle->target = target;
        AssignRef(slot, handle);
    }
    return handle;
}

// Makes `slot` refer to the weak handle of whatever `other` refers to (nothing when it refers to
// nothing or the target has no handle slot).
static inline void CopyWeakRef(UnkWeakHandle** slot, UnkWeakHandle* other) {
    UnkWeakTarget* target = other ? other->target : nullptr;
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    AssignRef(slot, handle);
}

// A counted reference to an object (what the constructors that return one by value hand out);
// dropping the last one calls vf_0x10.
struct UnkRefPtr {
    UnkRefObject* field_0x0;

    ~UnkRefPtr() {
        if (field_0x0) {
            if (--field_0x0->ref_count == 0)
                field_0x0->vf_0x10();
        }
    }
};

// The counted reference to the weak handle of the actor that the reflected property `member` reads
// as (directly or through a pointer value), or null (fn_7100ad7cc0 reads the property). Returned in
// memory. Defined in src/game/CActor.cpp (unit 341).
UnkRefPtr fn_710023b580(void* member);
void fn_710117b3f0(UnkWeakTarget* target, void* link);
