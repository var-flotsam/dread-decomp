#pragma once

// The descriptor of a collection type (CollectionTypeMapper<C>: CRntVector<T> and other
// containers): CType plus the key and element types, the iterator and const-iterator pointer
// types and the collection's accessor functions. The descriptors are globals built by static
// initialisers, through a constructor instantiated per type (like CEnumType).

#include "base/reflection/CClass.h"

namespace base::reflection {

template <class T>
struct CollectionTypeDescription;

class CCollectionType : public CType {
public:
    // One instantiation per collection type. `extra` and `extra2` are always null and unused;
    // `extra` deduces T, as in CClass's constructor.
    template <class T>
    CCollectionType(const base::global::CStrId& name, T* extra, void* extra2);
    ~CCollectionType() override;

    u32 m_kind;               // 1: indexed by position (the key is u32); 2: keyed (a map)
    const CType* m_key;       // the index type (u32 for arrays)
    const CType* m_element;
    const CType* m_iterator;        // pointer type of TElementIterator
    const CType* m_const_iterator;  // pointer type of TElementConstIterator
    // Accessors, read from the slot functions the generated descriptors name. Separate members,
    // not arrays: an all-constant array initialiser would be copied from a table instead of
    // loaded through the GOT. The two pairs after m_iterator and m_const_iterator are set in
    // the constructor's body, after those types.
    UnkFn m_count;       // the element count
    UnkFn m_clear;
    UnkFn m_add_copy;    // insert a value at a key (an array's index, or appended to a map);
    UnkFn m_add_move;    // copy and move by analogy with m_set_copy and m_set_move
    UnkFn m_erase;       // by key
    UnkFn m_at;          // the element at a position, unchecked
    UnkFn m_find;        // the element at a key, or null
    UnkFn m_set_copy;    // assign the value at a key
    UnkFn m_set_move;
    UnkFn m_begin;       // write an iterator to the first, or past the last, element
    UnkFn m_end;
    UnkFn m_const_begin;
    UnkFn m_const_end;
};
static_assert(sizeof(CCollectionType) == 0x170, "CCollectionType layout");

template <class T>
CCollectionType::CCollectionType(const base::global::CStrId& name, T*, void*)
    : CType(fn_7100001570("base::reflection::CCollectionType", 0x21), name, CollectionTypeDescription<T>::size,
            reinterpret_cast<UnkFn_0x58>(CollectionTypeDescription<T>::equals),
            reinterpret_cast<UnkFn_0x60>(CollectionTypeDescription<T>::hash),
            reinterpret_cast<UnkFn_0x68>(CollectionTypeDescription<T>::type_of), nullptr, nullptr,
            {CollectionTypeDescription<T>::construct,
             reinterpret_cast<UnkFn_0x30>(CollectionTypeDescription<T>::copy_construct),
             reinterpret_cast<UnkFn_0x30>(CollectionTypeDescription<T>::move_construct),
             reinterpret_cast<UnkFn_0x40>(CollectionTypeDescription<T>::destroy),
             reinterpret_cast<UnkFn_0x48>(CollectionTypeDescription<T>::copy_assign),
             reinterpret_cast<UnkFn_0x48>(CollectionTypeDescription<T>::move_assign)},
            false),
      m_kind(CollectionTypeDescription<T>::kind), m_key(CollectionTypeDescription<T>::key()),
      m_element(CollectionTypeDescription<T>::element()), m_iterator(nullptr), m_const_iterator(nullptr),
      m_count(CollectionTypeDescription<T>::count),
      m_clear(CollectionTypeDescription<T>::clear),
      m_add_copy(CollectionTypeDescription<T>::add_copy),
      m_add_move(CollectionTypeDescription<T>::add_move),
      m_erase(CollectionTypeDescription<T>::erase),
      m_at(CollectionTypeDescription<T>::at),
      m_find(CollectionTypeDescription<T>::find),
      m_set_copy(CollectionTypeDescription<T>::set_copy),
      m_set_move(CollectionTypeDescription<T>::set_move),
      m_begin(nullptr), m_end(nullptr), m_const_begin(nullptr), m_const_end(nullptr) {
    m_iterator = CollectionTypeDescription<T>::iterator();
    m_begin = CollectionTypeDescription<T>::begin;
    m_end = CollectionTypeDescription<T>::end;
    m_const_iterator = CollectionTypeDescription<T>::const_iterator();
    m_const_begin = CollectionTypeDescription<T>::const_begin;
    m_const_end = CollectionTypeDescription<T>::const_end;
}

}  // namespace base::reflection

// The reflected type of base::global::CRntVector<base::global::CStrId> (built by the static
// initialiser in src/game_s7/fn_71000e2800.cpp). A CCollectionType, declared as its base CType
// because the generated reflection sources declare it so.
extern base::reflection::CType g_unk_7101d5c998;
