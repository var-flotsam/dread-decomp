#pragma once

// base::reflection::CEnumType: the descriptor of an enum. The name comes from the kind string
// its constructor hashes ("base::reflection::CEnumType", length 0x1b). Layout: CType, the
// underlying integer type's descriptor, a constructor argument (unknown role), and the list of
// values (reserved for 16 entries by the constructor).
//
// Enum descriptors are globals built by their TU's static initialiser (which also adds the
// values), not by lazy getters. Only the constructors are generated so far
// (src/reflection_types/, tools/gen_types.py).

#include "base/reflection/CClass.h"
#include "base/reflection/TypeObject.h"

namespace base::reflection {

// Per-type facts for CEnumType's constructor (generated): size, the CType function slots
// (typed SlotFn and cast at the use site), the underlying type's getter and the out-of-line
// function that reserves the value list (one instance per element type).
template <class T>
struct EnumTypeDescription;

class CEnumType : public CType {
public:
    // One instantiation per enum (data/functions: refl_InitType<E>). `extra` is always null and
    // unused; it deduces T.
    template <class T>
    CEnumType(const base::global::CStrId& name, const void* arg, T* extra);
    ~CEnumType() override;

    const CType* m_underlying;
    const void* field_0xe8;
    base::global::CRntVector<void*> m_values;
};
static_assert(sizeof(CEnumType) == 0x118, "CEnumType layout");

template <class T>
CEnumType::CEnumType(const base::global::CStrId& name, const void* arg, T*)
    : CType(fn_7100001570("base::reflection::CEnumType", 0x1b), name, EnumTypeDescription<T>::size,
            reinterpret_cast<EqualsFn>(EnumTypeDescription<T>::equals),
            reinterpret_cast<HashFn>(EnumTypeDescription<T>::hash),
            reinterpret_cast<TypeOfFn>(EnumTypeDescription<T>::type_of), nullptr, nullptr,
            {reinterpret_cast<SlotFn>(EnumTypeDescription<T>::construct), reinterpret_cast<CopyConstructFn>(EnumTypeDescription<T>::copy_construct),
             reinterpret_cast<CopyConstructFn>(EnumTypeDescription<T>::move_construct),
             reinterpret_cast<DestroyFn>(EnumTypeDescription<T>::destroy),
             reinterpret_cast<AssignFn>(EnumTypeDescription<T>::copy_assign),
             reinterpret_cast<AssignFn>(EnumTypeDescription<T>::move_assign)},
            false),
      m_underlying(EnumTypeDescription<T>::underlying()), field_0xe8(arg) {
    EnumTypeDescription<T>::reserve(&m_values, 0x10);
}

}  // namespace base::reflection

// The enum type of EInFrustumMode (built in src/base/reflection/fn_7100b04900.cpp), declared as its
// base CType because the generated reflection sources declare it so.
enum class EInFrustumMode : u32;
template <> base::reflection::CType base::reflection::TypeObject<EInFrustumMode>::object asm("g_unk_71727952a8");
