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
//
// The descriptor's own functions (its CType value slots and CCollectionType's accessors, from +0x108)
// are the mapper's static members, one instantiation per collection type. They are written for
// CRntVector<T> (base/global/CRntVector.h), whose key is the element's index; a keyed collection's are
// not written yet. Erase, SetCopy, SetMove, ConstBegin and ConstEnd are not written yet either.

#include <new>
#include <utility>

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

    // The CType slots. Reflection creates its arrays with allocation flags 0xf.
    static void Construct(void* obj) { new (obj) C(0xf); }
    static void CopyConstruct(void* dst, const void* src) { new (dst) C(*static_cast<const C*>(src)); }
    static void MoveConstruct(void* dst, void* src) { new (dst) C(std::move(*static_cast<C*>(src))); }
    static void Destroy(void* obj) { static_cast<C*>(obj)->Free(); }
    static void* CopyAssign(void* dst, const void* src) {
        return &(*static_cast<C*>(dst) = *static_cast<const C*>(src));
    }
    static void* MoveAssign(void* dst, void* src) { return &(*static_cast<C*>(dst) = std::move(*static_cast<C*>(src))); }
    static bool Equals(const void* a, const void* b) { return *static_cast<const C*>(a) == *static_cast<const C*>(b); }
    static s32 Hash(const void*) { return -1; }
    // The collection's descriptor (specialised per type).
    static const class CType& TypeOf(const void* obj);

    // CCollectionType's accessors.
    static u32 Count(const void* collection) { return static_cast<const C*>(collection)->count; }
    static void Clear(void* collection) { static_cast<C*>(collection)->Clear(); }
    static void AddCopy(void* collection, const void* key, const void* value) {
        static_cast<C*>(collection)->InsertCopy(*static_cast<const typename C::value_type*>(value), *static_cast<const u32*>(key));
    }
    static void AddMove(void* collection, const void* key, void* value) {
        static_cast<C*>(collection)->InsertMove(std::move(*static_cast<typename C::value_type*>(value)), *static_cast<const u32*>(key));
    }
    static void* At(void* collection, u32 index) { return &static_cast<C*>(collection)->data[index]; }
    static void* Find(void* collection, const void* key) {
        C* c = static_cast<C*>(collection);
        u32 index = *static_cast<const u32*>(key);
        return index < c->count ? &c->data[index] : nullptr;
    }
    static void Begin(void* collection, void* it) {
        static_cast<TElementIterator*>(it)->m_element = static_cast<C*>(collection)->data;
    }
    static void End(void* collection, void* it) {
        C* c = static_cast<C*>(collection);
        static_cast<TElementIterator*>(it)->m_element = c->data + c->count;
    }
};

// The mapper's functions under the members a collection's description gives its descriptor's constructor
// (CCollectionType.h). A description derives from this and adds the rest: size, kind, the key, element and
// iterator types, and the slots not written yet.
template <class C>
struct CollectionMapperDescription {
    static constexpr auto construct = &CollectionTypeMapper<C>::Construct;
    static constexpr auto copy_construct = &CollectionTypeMapper<C>::CopyConstruct;
    static constexpr auto move_construct = &CollectionTypeMapper<C>::MoveConstruct;
    static constexpr auto destroy = &CollectionTypeMapper<C>::Destroy;
    static constexpr auto copy_assign = &CollectionTypeMapper<C>::CopyAssign;
    static constexpr auto move_assign = &CollectionTypeMapper<C>::MoveAssign;
    static constexpr auto hash = &CollectionTypeMapper<C>::Hash;
    static constexpr auto count = &CollectionTypeMapper<C>::Count;
    static constexpr auto clear = &CollectionTypeMapper<C>::Clear;
    static constexpr auto add_copy = &CollectionTypeMapper<C>::AddCopy;
    static constexpr auto add_move = &CollectionTypeMapper<C>::AddMove;
    static constexpr auto at = &CollectionTypeMapper<C>::At;
    static constexpr auto find = &CollectionTypeMapper<C>::Find;
    static constexpr auto begin = &CollectionTypeMapper<C>::Begin;
    static constexpr auto end = &CollectionTypeMapper<C>::End;
};

}  // namespace base::reflection
