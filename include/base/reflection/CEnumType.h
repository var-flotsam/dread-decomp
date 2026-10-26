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

namespace base::reflection {

// Per-type facts for CEnumType's constructor (generated): size, the CType function slots
// (typed UnkFn and cast at the use site), the underlying type's getter and the out-of-line
// function that reserves the value list (one instance per element type).
template <class T>
struct UnkReflEnumTraits;

class CEnumType : public CType {
public:
    // One instantiation per enum (data/functions: refl_InitType<E>). `extra` is always null and
    // unused; it deduces T.
    template <class T>
    CEnumType(const base::global::CStrId& name, const void* arg, T* extra);
    ~CEnumType() override;

    const CType* m_underlying;
    const void* field_0xe8;
    base::UnkVector<void*> m_values;
};
static_assert(sizeof(CEnumType) == 0x118, "CEnumType layout");

template <class T>
CEnumType::CEnumType(const base::global::CStrId& name, const void* arg, T*)
    : CType(fn_7100001570("base::reflection::CEnumType", 0x1b), name, UnkReflEnumTraits<T>::size,
            reinterpret_cast<UnkFn_0x58>(UnkReflEnumTraits<T>::fn_0x58),
            reinterpret_cast<UnkFn_0x60>(UnkReflEnumTraits<T>::fn_0x60),
            reinterpret_cast<UnkFn_0x68>(UnkReflEnumTraits<T>::fn_0x68), nullptr, nullptr,
            {UnkReflEnumTraits<T>::fn_0x28, reinterpret_cast<UnkFn_0x30>(UnkReflEnumTraits<T>::fn_0x30),
             reinterpret_cast<UnkFn_0x30>(UnkReflEnumTraits<T>::fn_0x38),
             reinterpret_cast<UnkFn_0x40>(UnkReflEnumTraits<T>::fn_0x40),
             reinterpret_cast<UnkFn_0x48>(UnkReflEnumTraits<T>::fn_0x48),
             reinterpret_cast<UnkFn_0x48>(UnkReflEnumTraits<T>::fn_0x50)},
            false),
      m_underlying(UnkReflEnumTraits<T>::underlying()), field_0xe8(arg) {
    UnkReflEnumTraits<T>::reserve(&m_values, 0x10);
}

}  // namespace base::reflection

// The enum type of EInFrustumMode (built in src/game_aca57c/fn_7100b04900.cpp), declared as its
// base CType because the generated reflection sources declare it so.
extern base::reflection::CType g_unk_71727952a8;
