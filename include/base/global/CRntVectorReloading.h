#pragma once

// base::global::CRntVector's layout (include/base/global/CRntVector.h) with the element count as the
// only member of an anonymous union. A union member carries no type-based alias information, so the
// compiler reloads the count after every store through the element pointer instead of keeping it in
// a register. Some functions of the original do exactly that: fn_710111c0fc
// (src/game/CSpawnGroupComponent.cpp) reloads the count after each key it copies, fn_7100da27f4
// (src/game/UnkUnit2530_RefPtrPushBack_b46.cpp) after the slot stores, fn_7100fa0b50
// (src/base/reflection/reflection_ownedids_ovb0066.cpp) after each element is destroyed. Code that
// keeps the count in a register uses CRntVector; this is a separate type so that CRntVector itself
// stays a plain struct.
//
// A layout only: no default member initialisers (the functions that use it receive an existing
// array) and no operations. A unit whose original has an operation of its own derives from it and
// adds that operation.

#include "base/global/CRntVector.h"
#include "types.h"

namespace base::global {

template <class T>
struct CRntVectorReloading {
    using value_type = T;

    T* data;
    union {
        u32 count;
    };
    u32 capacity;
    u32 step;  // capacity grows to the next multiple of this
    bool field_0x14;
    u32 flags;  // passed to the allocator
    UnkAllocator* allocator;

    T* begin() { return data; }
    T* end() { return data + count; }
};
static_assert(sizeof(CRntVectorReloading<void*>) == 0x28, "CRntVectorReloading layout");

}  // namespace base::global
