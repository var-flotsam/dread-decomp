#pragma once

// How reflection walks a collection type C (CRntVector<T>, CRntDictionary<K, V>, ...): the iterator types
// its CCollectionType's begin and end return (base/reflection/CCollectionType.h). Each iterator type has a
// pointer type descriptor of its own (CPointerType, named "base::reflection::CollectionTypeMapper<C>::
// TElementIterator" and so on), whose value operations are ValueOps<iterator> (ValueOps.h).
//
// An element iterator is the element it is at, null at the end; every collection kind's is the same
// eight bytes. The element is untyped here because reflection reaches it only through the pointee's
// descriptor. A key-element iterator (an index, the element, and the key and element as they are read)
// is laid out per collection kind and is not written yet.

#include "types.h"

namespace base::reflection {

template <class C>
struct CollectionTypeMapper {
    struct TElementIterator {
        void* m_element = nullptr;
        bool operator==(const TElementIterator& other) const { return m_element == other.m_element; }
    };
    struct TElementConstIterator {
        const void* m_element = nullptr;
        bool operator==(const TElementConstIterator& other) const { return m_element == other.m_element; }
    };
    struct TKeyElementIterator;
    struct TKeyElementConstIterator;
};

}  // namespace base::reflection
