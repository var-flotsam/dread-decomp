#pragma once

// The pair a collection's key-element iterators yield, the key and the element as they are read
// (CollectionTypeMapper<C>::TKeyElementIterator::value_type, and TKeyElementConstIterator's): a reflected
// class of its own for each collection type C, described by C. tools/gen_reflection.py writes each
// one's description as a specialisation of refl_Description, and the functions it points to that have a
// known shape as specialisations of the members below; a member declared and not specialised isn't
// reconstructed yet.

#include "base/reflection/ReflectedClass.h"
#include "types.h"

// The value operations a pair's descriptor points to (CType+0x28..+0x68, ReflectedClass.h).
#define DREAD_KEY_ELEMENT_VALUE_MEMBERS_                                                                \
    static void refl_Construct(void* obj);                                                              \
    static void refl_CopyConstruct(void* dst, const void* src);                                         \
    static void refl_MoveConstruct(void* dst, const void* src);                                         \
    static void refl_Destroy(void* obj);                                                                \
    static void* refl_CopyAssign(void* dst, const void* src);                                           \
    static void* refl_MoveAssign(void* dst, const void* src);                                           \
    static bool refl_Equals(const void* a, const void* b);                                              \
    static s32 refl_Hash();                                                                             \
    static const CType& refl_TypeOf(const void* obj);                                                   \
    struct refl_Description

namespace base::reflection {

// The pair TKeyElementIterator yields.
template <class C>
struct KeyElementValue {
    DREAD_KEY_ELEMENT_VALUE_MEMBERS_;
};

// The pair TKeyElementConstIterator yields: the same key and element, which can't be assigned.
template <class C>
struct ConstKeyElementValue {
    DREAD_KEY_ELEMENT_VALUE_MEMBERS_;
};

}  // namespace base::reflection
