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
    SlotFn m_count;       // the element count
    SlotFn m_clear;
    SlotFn m_add_copy;    // insert a value at a key (an array's index, or appended to a map);
    SlotFn m_add_move;    // copy and move by analogy with m_set_copy and m_set_move
    SlotFn m_erase;       // by key
    SlotFn m_at;          // the element at a position, unchecked
    SlotFn m_find;        // the element at a key, or null
    SlotFn m_set_copy;    // assign the value at a key
    SlotFn m_set_move;
    SlotFn m_begin;       // write an iterator to the first, or past the last, element
    SlotFn m_end;
    SlotFn m_const_begin;
    SlotFn m_const_end;
};
static_assert(sizeof(CCollectionType) == 0x170, "CCollectionType layout");

template <class T>
CCollectionType::CCollectionType(const base::global::CStrId& name, T*, void*)
    : CType(StringCrc64("base::reflection::CCollectionType", 0x21), name, CollectionTypeDescription<T>::size,
            reinterpret_cast<EqualsFn>(CollectionTypeDescription<T>::equals),
            reinterpret_cast<HashFn>(CollectionTypeDescription<T>::hash),
            reinterpret_cast<TypeOfFn>(CollectionTypeDescription<T>::type_of), nullptr, nullptr,
            {reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::construct),
             reinterpret_cast<CopyConstructFn>(CollectionTypeDescription<T>::copy_construct),
             reinterpret_cast<CopyConstructFn>(CollectionTypeDescription<T>::move_construct),
             reinterpret_cast<DestroyFn>(CollectionTypeDescription<T>::destroy),
             reinterpret_cast<AssignFn>(CollectionTypeDescription<T>::copy_assign),
             reinterpret_cast<AssignFn>(CollectionTypeDescription<T>::move_assign)},
            false),
      m_kind(CollectionTypeDescription<T>::kind), m_key(CollectionTypeDescription<T>::key()),
      m_element(CollectionTypeDescription<T>::element()), m_iterator(nullptr), m_const_iterator(nullptr),
      m_count(reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::count)),
      m_clear(reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::clear)),
      m_add_copy(reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::add_copy)),
      m_add_move(reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::add_move)),
      m_erase(reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::erase)),
      m_at(reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::at)),
      m_find(reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::find)),
      m_set_copy(reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::set_copy)),
      m_set_move(reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::set_move)),
      m_begin(nullptr), m_end(nullptr), m_const_begin(nullptr), m_const_end(nullptr) {
    m_iterator = CollectionTypeDescription<T>::iterator();
    m_begin = reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::begin);
    m_end = reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::end);
    m_const_iterator = CollectionTypeDescription<T>::const_iterator();
    m_const_begin = reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::const_begin);
    m_const_end = reinterpret_cast<SlotFn>(CollectionTypeDescription<T>::const_end);
}

}  // namespace base::reflection

// The reflected type of base::global::CRntVector<base::global::CStrId> (built by the static
// initialiser in src/game_s7/fn_71000e2800.cpp). A CCollectionType, declared as its base CType
// because the generated reflection sources declare it so.
extern base::reflection::CType g_unk_7101d5c998;
