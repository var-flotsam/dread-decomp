#pragma once

// Shared handles: an object hands out the slot (its vf_0x28) that holds its handle, a small
// ref-counted object pointing back at it, created the first time someone asks. Names and
// layouts are placeholders.

#include <new>

#include "types.h"

void* fn_71000839a0(u64 size, u32 flags);  // allocation

namespace unk_handle {

// A ref-counted object; the count is a union member, so the pointer being replaced is reloaded
// after the increment.
struct UnkRefCounted {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkRefPtr {
    UnkRefCounted* ptr;
};

// Assigns a ref-counted pointer unless it already holds `p`.
inline void SetRef(UnkRefPtr& ref, UnkRefCounted* p) {
    if (ref.ptr != p) {
        if (p)
            ++p->ref_count;
        if (UnkRefCounted* old = ref.ptr) {
            if (--old->ref_count == 0)
                old->Delete();
        }
        ref.ptr = p;
    }
}

// The handle created for an object that has none yet (vtable 0x7101a209e0).
struct UnkHandle : UnkRefCounted {
    void* target;

    UnkHandle(void* t) {
        ref_count = 0;
        target = t;
    }
};

// The object's handle, created if it has none yet; null when the object has no slot. T has
// the slot getter, GetHandleSlot, at vtable offset 0x28.
template <class T>
__attribute__((always_inline)) inline UnkRefCounted* Handle(T* obj) {
    UnkRefCounted* handle = nullptr;
    if (UnkRefPtr* slot = obj->GetHandleSlot()) {
        handle = slot->ptr;
        if (!handle) {
            handle = new (fn_71000839a0(0x18, 0x3e)) UnkHandle(obj);
            SetRef(*slot, handle);
        }
    }
    return handle;
}

}  // namespace unk_handle
