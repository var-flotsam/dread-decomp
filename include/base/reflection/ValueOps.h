#pragma once

// A reflected type's value operations: the functions its descriptor's CType slots point to
// (+0x28..+0x68, base/reflection/CClass.h), one instantiation per type. The roles and their names
// are ClassDescription's (base/reflection/ReflectedClass.h). The descriptor itself is a global
// that the type's static initialiser builds, so TypeOf is specialised per type to return it.
//
// A type's description derives from ValueOpsDescription<T>, which names these functions under
// the members the descriptor constructors read (construct, copy_construct, ..., type_of).

#include <new>
#include <type_traits>
#include <utility>

#include "types.h"

namespace base::reflection {

class CType;

template <class T>
struct ValueOps {
    static void* Construct(void* obj) { return new (obj) T(); }
    static void* CopyConstruct(void* dst, const void* src) { return new (dst) T(*static_cast<const T*>(src)); }
    static void* MoveConstruct(void* dst, void* src) { return new (dst) T(std::move(*static_cast<T*>(src))); }
    static void Destroy(void* obj) { static_cast<T*>(obj)->~T(); }
    static void* CopyAssign(void* dst, const void* src) {
        if (dst != src)
            *static_cast<T*>(dst) = *static_cast<const T*>(src);
        return dst;
    }
    static void* MoveAssign(void* dst, void* src) {
        if (dst != src)
            *static_cast<T*>(dst) = std::move(*static_cast<T*>(src));
        return dst;
    }
    static bool Equals(const void* a, const void* b) {
        return *static_cast<const T*>(a) == *static_cast<const T*>(b);
    }
    // An enum hashes to its value.
    static s32 Hash(const void* obj) { return static_cast<s32>(*static_cast<const T*>(obj)); }
    // The type's descriptor (specialised per type).
    static const CType& TypeOf(const void* obj);
};

template <class T>
struct ValueOpsDescription {
    static constexpr auto construct = &ValueOps<T>::Construct;
    static constexpr auto copy_construct = &ValueOps<T>::CopyConstruct;
    static constexpr auto move_construct = &ValueOps<T>::MoveConstruct;
    static constexpr auto destroy = &ValueOps<T>::Destroy;
    static constexpr auto copy_assign = &ValueOps<T>::CopyAssign;
    static constexpr auto move_assign = &ValueOps<T>::MoveAssign;
    static constexpr auto equals = &ValueOps<T>::Equals;
    static constexpr auto hash = &ValueOps<T>::Hash;
    static constexpr auto type_of = &ValueOps<T>::TypeOf;
};

}  // namespace base::reflection
