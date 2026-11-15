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

// The plain types the engine hashes by value: the others (char, short, the 64-bit long long, the
// floating-point types, ...) hash to -1 as classes do.
template <class T>
constexpr bool HashedByValue = std::is_same_v<T, bool> || std::is_same_v<T, int> || std::is_same_v<T, unsigned> ||
                               std::is_same_v<T, long> || std::is_same_v<T, unsigned long>;

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
    // An enum, a pointer and the integers listed in HashedByValue hash to their value (a pointer to its
    // address's low word); anything else (a flag set, every class) to -1.
    static s32 Hash(const void* obj) {
        const T& value = *static_cast<const T*>(obj);
        if constexpr (std::is_enum_v<T> || HashedByValue<T>)
            return static_cast<s32>(value);
        else if constexpr (std::is_pointer_v<T>)
            return static_cast<s32>(reinterpret_cast<uintptr_t>(value));
        else
            return -1;
    }
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
