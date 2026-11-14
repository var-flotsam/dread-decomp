#pragma once

// The descriptor of a collection type (CollectionTypeMapper<C>: CRntVector<T> and other
// containers): CType plus the key and element types, the iterator and const-iterator pointer
// types and the collection's accessor functions. The descriptors are globals built by static
// initialisers, through a constructor instantiated per type (like CEnumType).

#include "base/reflection/CClass.h"

namespace base::reflection {

template <class T>
struct UnkReflCollectionTraits;

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
    UnkFn m_size;        // the element count
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
    : CType(fn_7100001570("base::reflection::CCollectionType", 0x21), name, UnkReflCollectionTraits<T>::size,
            reinterpret_cast<UnkFn_0x58>(UnkReflCollectionTraits<T>::fn_0x58),
            reinterpret_cast<UnkFn_0x60>(UnkReflCollectionTraits<T>::fn_0x60),
            reinterpret_cast<UnkFn_0x68>(UnkReflCollectionTraits<T>::fn_0x68), nullptr, nullptr,
            {UnkReflCollectionTraits<T>::fn_0x28,
             reinterpret_cast<UnkFn_0x30>(UnkReflCollectionTraits<T>::fn_0x30),
             reinterpret_cast<UnkFn_0x30>(UnkReflCollectionTraits<T>::fn_0x38),
             reinterpret_cast<UnkFn_0x40>(UnkReflCollectionTraits<T>::fn_0x40),
             reinterpret_cast<UnkFn_0x48>(UnkReflCollectionTraits<T>::fn_0x48),
             reinterpret_cast<UnkFn_0x48>(UnkReflCollectionTraits<T>::fn_0x50)},
            false),
      m_kind(UnkReflCollectionTraits<T>::field_0xe0), m_key(UnkReflCollectionTraits<T>::key()),
      m_element(UnkReflCollectionTraits<T>::element()), m_iterator(nullptr), m_const_iterator(nullptr),
      m_size(UnkReflCollectionTraits<T>::fn_0x108),
      m_clear(UnkReflCollectionTraits<T>::fn_0x110),
      m_add_copy(UnkReflCollectionTraits<T>::fn_0x118),
      m_add_move(UnkReflCollectionTraits<T>::fn_0x120),
      m_erase(UnkReflCollectionTraits<T>::fn_0x128),
      m_at(UnkReflCollectionTraits<T>::fn_0x130),
      m_find(UnkReflCollectionTraits<T>::fn_0x138),
      m_set_copy(UnkReflCollectionTraits<T>::fn_0x140),
      m_set_move(UnkReflCollectionTraits<T>::fn_0x148),
      m_begin(nullptr), m_end(nullptr), m_const_begin(nullptr), m_const_end(nullptr) {
    m_iterator = UnkReflCollectionTraits<T>::iterator();
    m_begin = UnkReflCollectionTraits<T>::fn_0x150;
    m_end = UnkReflCollectionTraits<T>::fn_0x158;
    m_const_iterator = UnkReflCollectionTraits<T>::const_iterator();
    m_const_begin = UnkReflCollectionTraits<T>::fn_0x160;
    m_const_end = UnkReflCollectionTraits<T>::fn_0x168;
}

}  // namespace base::reflection

// The reflected type of base::global::CRntVector<base::global::CStrId> (built by the static
// initialiser in src/game_s7/fn_71000e2800.cpp). A CCollectionType, declared as its base CType
// because the generated reflection sources declare it so.
extern base::reflection::CType g_unk_7101d5c998;
