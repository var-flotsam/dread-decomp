#pragma once

// How reflection walks a collection type C (CRntVector<T>, CRntDictionary<K, V>, ...): the iterator types
// its CCollectionType's begin and end return (base/reflection/CCollectionType.h). Each iterator type has a
// pointer type descriptor of its own (CPointerType, named "base::reflection::CollectionTypeMapper<C>::
// TElementIterator" and so on), whose value operations are ValueOps<iterator> (ValueOps.h).
//
// An element iterator is the element it is at, null at the end; every collection kind's is the same
// eight bytes. The element is untyped here because reflection reaches it only through the pointee's
// descriptor. A key-element iterator (an index, the element, and the key and element as they are read)
// is laid out per collection kind: a vector's is base::UnkCollectionIter (base/UnkCollectionIter.h), a keyed
// collection's is not written yet.
//
// The descriptor's own functions (its CType value slots and CCollectionType's accessors, from +0x108)
// are the mapper's static members, one instantiation per collection type. They are written for
// CRntVector<T> (base/global/CRntVector.h), whose key is the element's index; a keyed collection's are
// not written yet.

#include <new>
#include <utility>

#include "base/UnkCollectionIter.h"
#include "types.h"

namespace base::global {
template <class K, class V>
struct CRntSmallDictionary;
template <class K, class V>
struct CRntDictionary;
}  // namespace base::global

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
    // A vector's key-element iterator: the index, the element, and the pair as it is read.
    struct TKeyElementIterator : UnkCollectionIter<typename C::value_type> {
        using UnkCollectionIter<typename C::value_type>::UnkCollectionIter;
    };
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
    // The collection's descriptor: specialised per type, where the generated description names it.
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
    static void Erase(void* collection, const void* key) {
        // The index goes to a position counted from the collection's own address and back, as in the original.
        auto* origin = static_cast<typename C::value_type*>(collection);
        auto* position = origin + *static_cast<const u32*>(key);
        static_cast<C*>(collection)->EraseAt(position - origin);
    }
    static void SetCopy(void* collection, const void* key, const void* value) {
        static_cast<C*>(collection)->GrowAt(*static_cast<const u32*>(key)) =
            *static_cast<const typename C::value_type*>(value);
    }
    static void SetMove(void* collection, const void* key, void* value) {
        static_cast<C*>(collection)->GrowAt(*static_cast<const u32*>(key)) =
            std::move(*static_cast<typename C::value_type*>(value));
    }
    static void Begin(void* collection, void* it) {
        static_cast<TElementIterator*>(it)->m_element = static_cast<C*>(collection)->data;
    }
    static void End(void* collection, void* it) {
        C* c = static_cast<C*>(collection);
        static_cast<TElementIterator*>(it)->m_element = c->data + c->count;
    }
    static void ConstBegin(const void* collection, void* it) {
        const C* c = static_cast<const C*>(collection);
        *static_cast<TKeyElementIterator*>(it) = TKeyElementIterator(0, c->data);
    }
    static void ConstEnd(const void* collection, void* it) {
        const C* c = static_cast<const C*>(collection);
        *static_cast<TKeyElementIterator*>(it) = TKeyElementIterator(c->count, c->data + c->count);
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
    static constexpr auto equals = &CollectionTypeMapper<C>::Equals;
    static constexpr auto erase = &CollectionTypeMapper<C>::Erase;
    static constexpr auto set_copy = &CollectionTypeMapper<C>::SetCopy;
    static constexpr auto set_move = &CollectionTypeMapper<C>::SetMove;
    static constexpr auto const_begin = &CollectionTypeMapper<C>::ConstBegin;
    static constexpr auto const_end = &CollectionTypeMapper<C>::ConstEnd;
    static constexpr auto type_of = &CollectionTypeMapper<C>::TypeOf;
};

// The members that touch only a vector's header (its pointer, count and allocator), for a vector whose
// element type is not written yet.
template <class C>
struct VectorHeaderMapperDescription {
    using Mapper = CollectionTypeMapper<C>;
    static constexpr auto construct = &Mapper::Construct;
    static constexpr auto move_construct = &Mapper::MoveConstruct;
    static constexpr auto hash = &Mapper::Hash;
    static constexpr auto count = &Mapper::Count;
    static constexpr auto begin = &Mapper::Begin;
    static constexpr auto type_of = &Mapper::TypeOf;
};

// A small dictionary's mapper (base/global/CRntSmallDictionary.h): its element iterators are a vector's.
// The slots written so far are those that need none of its growth or copies; its erase is still by address.
template <class K, class V>
struct CollectionTypeMapper<global::CRntSmallDictionary<K, V>> {
    using C = global::CRntSmallDictionary<K, V>;

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

    // Reflection creates its dictionaries with allocation flags 0xe.
    static void Construct(void* obj) { new (obj) C(0xe); }
    static void MoveConstruct(void* dst, void* src) { new (dst) C(std::move(*static_cast<C*>(src))); }
    static void* MoveAssign(void* dst, void* src) { return &(*static_cast<C*>(dst) = std::move(*static_cast<C*>(src))); }
    static bool Equals(const void* a, const void* b) { return *static_cast<const C*>(a) == *static_cast<const C*>(b); }
    static s32 Hash(const void*) { return -1; }
    static const class CType& TypeOf(const void* obj);

    static u32 Count(const void* collection) { return static_cast<const C*>(collection)->count; }
    static void* At(void* collection, u32 index) { return &static_cast<C*>(collection)->values[index]; }
    static void* Find(void* collection, const void* key) {
        C* c = static_cast<C*>(collection);
        V* value = c->Find(*static_cast<const K*>(key));
        return value == c->End() ? nullptr : value;
    }
    static void SetCopy(void* collection, const void* key, const void* value) {
        C* c = static_cast<C*>(collection);
        V* v = c->Find(*static_cast<const K*>(key));
        if (v != c->End())
            *v = *static_cast<const V*>(value);
    }
    static void SetMove(void* collection, const void* key, void* value) {
        C* c = static_cast<C*>(collection);
        V* v = c->Find(*static_cast<const K*>(key));
        if (v != c->End())
            *v = std::move(*static_cast<V*>(value));
    }
    static void Begin(void* collection, void* it) {
        static_cast<TElementIterator*>(it)->m_element = static_cast<C*>(collection)->values;
    }
    static void End(void* collection, void* it) {
        C* c = static_cast<C*>(collection);
        static_cast<TElementIterator*>(it)->m_element = c->values + c->count;
    }
};

// The members of a small dictionary C's description that touch only its header, for a dictionary whose
// value type is not written yet.
template <class C>
struct SmallDictionaryHeaderMapperDescription {
    using Mapper = CollectionTypeMapper<C>;
    static constexpr auto construct = &Mapper::Construct;
    static constexpr auto move_construct = &Mapper::MoveConstruct;
    static constexpr auto hash = &Mapper::Hash;
    static constexpr auto count = &Mapper::Count;
    static constexpr auto begin = &Mapper::Begin;
    static constexpr auto type_of = &Mapper::TypeOf;
};

// And those that need its value whole.
template <class C>
struct SmallDictionaryMapperDescription : SmallDictionaryHeaderMapperDescription<C> {
    using Mapper = CollectionTypeMapper<C>;
    static constexpr auto at = &Mapper::At;
    static constexpr auto end = &Mapper::End;
};

// And those that compare keys, for a dictionary whose key type has an operator==.
template <class C>
struct SmallDictionaryKeyedMapperDescription : SmallDictionaryMapperDescription<C> {
    using Mapper = CollectionTypeMapper<C>;
    static constexpr auto move_assign = &Mapper::MoveAssign;
    static constexpr auto equals = &Mapper::Equals;
    static constexpr auto find = &Mapper::Find;
    static constexpr auto set_copy = &Mapper::SetCopy;
    static constexpr auto set_move = &Mapper::SetMove;
};

// A dictionary's mapper (base/global/CRntDictionary.h): an element iterator is at a node, null at the end.
// The slots written so far are those that need none of its insertion, erase or destruction.
template <class K, class V>
struct CollectionTypeMapper<global::CRntDictionary<K, V>> {
    using C = global::CRntDictionary<K, V>;

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

    // Reflection constructs no dictionary in place: the object holding it already has.
    static void Construct(void*) {}
    // Dictionaries compare equal only to themselves.
    static bool Equals(const void* a, const void* b) { return a == b; }
    static s32 Hash(const void*) { return -1; }
    static const class CType& TypeOf(const void* obj);

    static u32 Count(const void* collection) { return static_cast<const C*>(collection)->count; }
    static void* Find(void* collection, const void* key) {
        return static_cast<C*>(collection)->Find(*static_cast<const K*>(key));
    }
    static void Clear(void* collection) { static_cast<C*>(collection)->Clear(); }
    static void SetCopy(void* collection, const void* key, const void* value) {
        if (V* v = static_cast<C*>(collection)->Find(*static_cast<const K*>(key)))
            *v = *static_cast<const V*>(value);
    }
    static void SetMove(void* collection, const void* key, void* value) {
        if (V* v = static_cast<C*>(collection)->Find(*static_cast<const K*>(key)))
            *v = std::move(*static_cast<V*>(value));
    }
    static void Begin(void* collection, void* it) {
        static_cast<TElementIterator*>(it)->m_element = static_cast<C*>(collection)->first;
    }
    static void End(void*, void* it) { static_cast<TElementIterator*>(it)->m_element = nullptr; }
};

// The members of a dictionary C's description that its mapper writes; they need neither its key nor its
// value whole.
template <class C>
struct DictionaryMapperDescription {
    using Mapper = CollectionTypeMapper<C>;
    static constexpr auto construct = &Mapper::Construct;
    static constexpr auto equals = &Mapper::Equals;
    static constexpr auto hash = &Mapper::Hash;
    static constexpr auto count = &Mapper::Count;
    static constexpr auto begin = &Mapper::Begin;
    static constexpr auto end = &Mapper::End;
    static constexpr auto type_of = &Mapper::TypeOf;
};

// And those that look a key up or destroy entries, for a dictionary whose key and value are whole.
template <class C>
struct DictionaryFindMapperDescription : DictionaryMapperDescription<C> {
    using Mapper = CollectionTypeMapper<C>;
    static constexpr auto clear = &Mapper::Clear;
    static constexpr auto find = &Mapper::Find;
    static constexpr auto set_copy = &Mapper::SetCopy;
    static constexpr auto set_move = &Mapper::SetMove;
};

}  // namespace base::reflection
