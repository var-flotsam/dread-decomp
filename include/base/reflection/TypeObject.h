#pragma once

// The type object a static initialiser builds for T (an enum's CEnumType, a flag set's CFlagsetType), named by
// T: TypeObject<T>::object. A source that names one declares its specialisation, which carries the placeholder
// name of the object it links to:
//
//     template <> base::reflection::CType base::reflection::TypeObject<EFoo>::object asm("g_unk_7101d4d5d0");
//
// Without an initialiser that is a declaration, not a definition (the object is the original's, built by its
// static initialiser in src/reflection_types/), so a use reads `&TypeObject<EFoo>::object` and still links to
// the same object.

#include "base/reflection/CClass.h"

namespace base::reflection {

template <class T>
struct TypeObject {
    static CType object;
};

}  // namespace base::reflection
