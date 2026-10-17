#pragma once

// base::reflection::CPointerType: the descriptor of a pointer type. The name comes from the
// kind string its constructor hashes ("base::reflection::CPointerType", length 0x1e).
// sizeof(T) is usually 8, but some pointer-like iterator types are larger (0x38).
// Layout: CType, then a bool, the pointee's descriptor and three type-specific functions.
//
// Non-class type getters (refl_GetType<T>) are templates in the original: their guard and
// static descriptor are reached through the GOT, as weak symbols are under -fPIC. Generated
// instances live in src/reflection_types/ (tools/gen_types.py).

#include "base/reflection/CClass.h"

namespace base::reflection {

// Per-type facts for CPointerType's constructor (generated). Function members are typed as
// UnkFn and cast at the use site: one helper can fill slots of different types.
template <class T>
struct UnkReflPointerTraits;

class CPointerType : public CType {
public:
    // One instantiation per pointer type (data/functions: refl_InitType<T*>). `extra` and
    // `extra2` are always null and unused; `extra` deduces T, as in CClass's constructor.
    template <class T>
    CPointerType(const base::global::CStrId& name, T* extra, void* extra2);
    ~CPointerType() override;

    bool field_0xe0;
    const CType* m_pointee;
    UnkFn field_0xf0;
    UnkFn field_0xf8;
    UnkFn field_0x100;
};
static_assert(sizeof(CPointerType) == 0x108, "CPointerType layout");

template <class T>
CPointerType::CPointerType(const base::global::CStrId& name, T*, void*)
    : CType(fn_7100001570("base::reflection::CPointerType", 0x1e), name, UnkReflPointerTraits<T>::size,
            reinterpret_cast<UnkFn_0x58>(UnkReflPointerTraits<T>::fn_0x58),
            reinterpret_cast<UnkFn_0x60>(UnkReflPointerTraits<T>::fn_0x60),
            reinterpret_cast<UnkFn_0x68>(UnkReflPointerTraits<T>::fn_0x68), nullptr, nullptr,
            {UnkReflPointerTraits<T>::fn_0x28, reinterpret_cast<UnkFn_0x30>(UnkReflPointerTraits<T>::fn_0x30),
             reinterpret_cast<UnkFn_0x30>(UnkReflPointerTraits<T>::fn_0x38),
             reinterpret_cast<UnkFn_0x40>(UnkReflPointerTraits<T>::fn_0x40),
             reinterpret_cast<UnkFn_0x48>(UnkReflPointerTraits<T>::fn_0x48),
             reinterpret_cast<UnkFn_0x48>(UnkReflPointerTraits<T>::fn_0x50)},
            false),
      field_0xe0(UnkReflPointerTraits<T>::field_0xe0), m_pointee(UnkReflPointerTraits<T>::pointee()),
      field_0xf0(UnkReflPointerTraits<T>::fn_0xf0), field_0xf8(UnkReflPointerTraits<T>::fn_0xf8),
      field_0x100(UnkReflPointerTraits<T>::fn_0x100) {}

// Per-getter facts (generated): the type's name and its descriptor constructor's tag.
template <class G>
struct UnkReflTypeGetter;

// refl_GetType<T> for a pointer type. G is a tag named after the getter's address.
template <class G>
const CType* refl_GetPointerType() {
    static CPointerType s_type(base::global::CStrId(UnkReflTypeGetter<G>::name, true),
                               static_cast<typename UnkReflTypeGetter<G>::Init*>(nullptr), nullptr);
    return &s_type;
}

}  // namespace base::reflection
