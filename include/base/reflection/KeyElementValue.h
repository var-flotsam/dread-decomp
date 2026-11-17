#pragma once

// The pair a collection's key-element iterators yield, the key and the element as they are read
// (CollectionTypeMapper<C>::TKeyElementIterator::value_type, and TKeyElementConstIterator's): a reflected
// class of its own for each collection type C, described by C. tools/gen_reflection.py writes each
// one's description as a specialisation of refl_Description, and the functions it points to that have a
// known shape as specialisations of the members below
// (DREAD_REFLECTED_VALUE_MEMBERS, base/reflection/ReflectedValue.h); a member declared and not specialised isn't
// reconstructed yet.

#include "base/reflection/ReflectedValue.h"
#include "types.h"

namespace base::reflection {

// The pair TKeyElementIterator yields.
template <class C>
struct KeyElementValue {
    DREAD_REFLECTED_VALUE_MEMBERS;
};

// The pair TKeyElementConstIterator yields: the same key and element, which can't be assigned.
template <class C>
struct ConstKeyElementValue {
    DREAD_REFLECTED_VALUE_MEMBERS;
};

}  // namespace base::reflection
