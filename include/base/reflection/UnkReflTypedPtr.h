#pragma once

// A pointer paired with its dynamic type descriptor, as a reflected pointer type's helper
// returns it for the object a T* points to (fn_710009bda4, fn_710000bc10 and their copies;
// tools/gen_clones.py, kind "pointee_typed"). Names and roles are placeholders inferred from
// the code.

#include "types.h"

// Refines a type descriptor for an object (applied twice by the original).
char* fn_710009dcd8(char* type, void* obj);

namespace base::reflection {

class CType;

// get(): the pointee's type descriptor. get_static(): the same through a second function
// that the linker folded into the first (the original calls it once per branch, so the two
// calls can't have been one function at compile time).
template <class Tag>
struct UnkTypedPtrTraits;

// Not trivially copyable, so it is returned in memory. Clearing the pointer through `bits`
// keeps the two branches' stores apart, as in the original.
struct UnkReflTypedPtr {
    char* type;
    union {
        void* ptr;
        u64 bits;
    };

    UnkReflTypedPtr() {}
    UnkReflTypedPtr(const UnkReflTypedPtr& other) {
        ptr = other.ptr;
        type = other.type;
    }
};

template <class Tag>
char* refl_DynamicType(void* obj) {
    char* t = UnkTypedPtrTraits<Tag>::get();
    t = fn_710009dcd8(t, obj);
    return fn_710009dcd8(t, obj);
}

template <class Tag>
UnkReflTypedPtr refl_PointeeTyped(void* const* p) {
    void* obj = *p;
    UnkReflTypedPtr r;
    char* t;
    if (obj) {
        t = refl_DynamicType<Tag>(obj);
        r.ptr = obj;
    } else {
        t = UnkTypedPtrTraits<Tag>::get_static();
        r.bits = 0;
    }
    r.type = t;
    return r;
}

}  // namespace base::reflection
