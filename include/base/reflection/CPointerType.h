#pragma once

// base::reflection::CPointerType: the descriptor of a pointer type. The name comes from the
// kind string its constructor hashes ("base::reflection::CPointerType", length 0x1e).
// sizeof(T) is usually 8, but some pointer-like iterator types are larger (0x38).
// Layout: CType, then whether it owns its pointee, the pointee's descriptor and three accessors.
//
// Non-class type getters (refl_GetType<T>) are templates in the original: their guard and
// static descriptor are reached through the GOT, as weak symbols are under -fPIC. Generated
// instances live in src/reflection_types/ (tools/gen_types.py).

#include "base/reflection/CClass.h"

namespace base::reflection {

// Per-type facts for CPointerType's constructor (generated). Function members are typed as
// SlotFn and cast at the use site: one helper can fill slots of different types.
template <class T>
struct PointerTypeDescription;

class CPointerType : public CType {
public:
    // One instantiation per pointer type (data/functions: refl_InitType<T*>). `extra` and
    // `extra2` are always null and unused; `extra` deduces T, as in CClass's constructor.
    template <class T>
    CPointerType(const base::global::CStrId& name, T* extra, void* extra2);
    ~CPointerType() override;

    bool m_owned;  // whether the pointer owns its pointee (read by the Lua binding)
    const CType* m_pointee;
    SlotFn m_get;        // the pointee as a typed reference: its dynamic type and its address
    SlotFn m_set;        // store the address of a typed reference (a no-op for some types)
    SlotFn m_increment;  // advance by one
};
static_assert(sizeof(CPointerType) == 0x108, "CPointerType layout");

template <class T>
CPointerType::CPointerType(const base::global::CStrId& name, T*, void*)
    : CType(StringCrc64("base::reflection::CPointerType", 0x1e), name, PointerTypeDescription<T>::size,
            reinterpret_cast<EqualsFn>(PointerTypeDescription<T>::equals),
            reinterpret_cast<HashFn>(PointerTypeDescription<T>::hash),
            reinterpret_cast<TypeOfFn>(PointerTypeDescription<T>::type_of), nullptr, nullptr,
            {reinterpret_cast<SlotFn>(PointerTypeDescription<T>::construct), reinterpret_cast<CopyConstructFn>(PointerTypeDescription<T>::copy_construct),
             reinterpret_cast<CopyConstructFn>(PointerTypeDescription<T>::move_construct),
             reinterpret_cast<DestroyFn>(PointerTypeDescription<T>::destroy),
             reinterpret_cast<AssignFn>(PointerTypeDescription<T>::copy_assign),
             reinterpret_cast<AssignFn>(PointerTypeDescription<T>::move_assign)},
            false),
      m_owned(PointerTypeDescription<T>::owned), m_pointee(PointerTypeDescription<T>::pointee()),
      m_get(PointerTypeDescription<T>::get), m_set(PointerTypeDescription<T>::set),
      m_increment(PointerTypeDescription<T>::increment) {}

// refl_GetType<T> for a pointer type T whose description is keyed by T itself (tools/gen_types.py): the
// description gives the name too. data/functions/ calls Get refl_GetType<T> (asmdiff.name_aliases).
template <class T>
struct PointerType {
    static const CType* Get();
};

// Not inline: a description in another file names Get through an explicit instantiation declaration
// (`extern template`), and the call stays a call, as in the original.
template <class T>
const CType* PointerType<T>::Get() {
    static CPointerType s_type(base::global::CStrId(PointerTypeDescription<T>::name, true), static_cast<T*>(nullptr),
                               nullptr);
    return &s_type;
}

// The same for a type still keyed by address tags (its name does not parse, or two getters share it).
// Per-getter facts (generated): the type's name and its descriptor constructor's tag.
template <class G>
struct PointerTypeGetter;

// refl_GetType<T> for a pointer type. G is a tag named after the getter's address.
template <class G>
const CType* refl_GetPointerType() {
    static CPointerType s_type(base::global::CStrId(PointerTypeGetter<G>::name, true),
                               static_cast<typename PointerTypeGetter<G>::Init*>(nullptr), nullptr);
    return &s_type;
}

}  // namespace base::reflection
