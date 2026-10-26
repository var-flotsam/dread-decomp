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

    u32 field_0xe0;
    const CType* m_key;      // the index type (u32 for arrays)
    const CType* m_element;
    const CType* m_iterator;        // pointer type of TElementIterator
    const CType* m_const_iterator;  // pointer type of TElementConstIterator
    // Accessors (+0x108..+0x148), then two set after the iterator type and two after the
    // const-iterator type. Separate members, not arrays: an all-constant array initialiser
    // would be copied from a table instead of loaded through the GOT.
    UnkFn field_0x108;
    UnkFn field_0x110;
    UnkFn field_0x118;
    UnkFn field_0x120;
    UnkFn field_0x128;
    UnkFn field_0x130;
    UnkFn field_0x138;
    UnkFn field_0x140;
    UnkFn field_0x148;
    UnkFn field_0x150;
    UnkFn field_0x158;
    UnkFn field_0x160;
    UnkFn field_0x168;
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
      field_0xe0(UnkReflCollectionTraits<T>::field_0xe0), m_key(UnkReflCollectionTraits<T>::key()),
      m_element(UnkReflCollectionTraits<T>::element()), m_iterator(nullptr), m_const_iterator(nullptr),
      field_0x108(UnkReflCollectionTraits<T>::fn_0x108),
      field_0x110(UnkReflCollectionTraits<T>::fn_0x110),
      field_0x118(UnkReflCollectionTraits<T>::fn_0x118),
      field_0x120(UnkReflCollectionTraits<T>::fn_0x120),
      field_0x128(UnkReflCollectionTraits<T>::fn_0x128),
      field_0x130(UnkReflCollectionTraits<T>::fn_0x130),
      field_0x138(UnkReflCollectionTraits<T>::fn_0x138),
      field_0x140(UnkReflCollectionTraits<T>::fn_0x140),
      field_0x148(UnkReflCollectionTraits<T>::fn_0x148),
      field_0x150(nullptr), field_0x158(nullptr), field_0x160(nullptr), field_0x168(nullptr) {
    m_iterator = UnkReflCollectionTraits<T>::iterator();
    field_0x150 = UnkReflCollectionTraits<T>::fn_0x150;
    field_0x158 = UnkReflCollectionTraits<T>::fn_0x158;
    m_const_iterator = UnkReflCollectionTraits<T>::const_iterator();
    field_0x160 = UnkReflCollectionTraits<T>::fn_0x160;
    field_0x168 = UnkReflCollectionTraits<T>::fn_0x168;
}

}  // namespace base::reflection

// The reflected type of base::global::CRntVector<base::global::CStrId> (built by the static
// initialiser in src/game_s7/fn_71000e2800.cpp). A CCollectionType, declared as its base CType
// because the generated reflection sources declare it so.
extern base::reflection::CType g_unk_7101d5c998;
