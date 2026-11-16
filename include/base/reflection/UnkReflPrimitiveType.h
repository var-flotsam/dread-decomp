#pragma once

// Getters of plain CType descriptors (refl_GetType<T> for float, unsigned, bool, CStrId,
// CVector3D, CColor4F, ...): types described only by their name, size and the CType function
// slots. The kind hash is computed from the type's own name, so name and kind share one string.
// The getters are templates in the original (guard and descriptor reached through the GOT);
// generated instances live in src/reflection_types/ (tools/gen_types.py).

#include "base/reflection/CClass.h"

namespace base::reflection {

// Per-getter facts (generated): name and its length, size, and the function slots under the role
// names of base/reflection/ValueOps.h (construct, ..., type_of) plus add_attributes (CType+0x70),
// cast at the use site (null when the type lacks that function). A named plain type takes the
// value operations from ValueOpsDescription<T>.
template <class G>
struct PrimitiveTypeDescription;  // G: the getter's tag, or the type itself (PrimitiveType<T>)

// Builds the descriptor in place (a prvalue, so the static is constructed directly). The name
// is built by the caller before the kind is hashed, as in the original.
template <class G>
inline CType refl_MakePrimitiveType(const base::global::CStrId& name) {
    using Tr = PrimitiveTypeDescription<G>;
    return CType(fn_7100001570(Tr::name, Tr::length), name, Tr::size, reinterpret_cast<EqualsFn>(Tr::equals),
                 reinterpret_cast<HashFn>(Tr::hash), reinterpret_cast<TypeOfFn>(Tr::type_of),
                 reinterpret_cast<ClassSetupFn>(Tr::add_attributes), nullptr,
                 {reinterpret_cast<SlotFn>(Tr::construct), reinterpret_cast<CopyConstructFn>(Tr::copy_construct),
                  reinterpret_cast<CopyConstructFn>(Tr::move_construct),
                  reinterpret_cast<DestroyFn>(Tr::destroy), reinterpret_cast<AssignFn>(Tr::copy_assign),
                  reinterpret_cast<AssignFn>(Tr::move_assign)},
                 false);
}

// refl_GetType<T> for a plain type. G is a tag named after the getter's address. It returns a
// reference: a plain type's TypeOf (ValueOps.h) tail-calls it, which Clang does only when the
// caller's and callee's returns carry the same attributes (a pointer's TypeOf, which dereferences a
// pointer-returning getter, calls it instead).
template <class G>
const CType& refl_GetPrimitiveType() {
    static CType s_type = refl_MakePrimitiveType<G>(base::global::CStrId(PrimitiveTypeDescription<G>::name, true));
    return s_type;
}

// refl_GetType<T> for a plain type keyed by the C++ type it describes (int, float, ...), with
// PrimitiveTypeDescription<T> its facts. Not inline: a description in another file names Get through an
// explicit instantiation declaration (`extern template`), and the call stays a call, as in the original.
template <class T>
struct PrimitiveType {
    static const CType& Get();
};

template <class T>
const CType& PrimitiveType<T>::Get() {
    static CType s_type = refl_MakePrimitiveType<T>(base::global::CStrId(PrimitiveTypeDescription<T>::name, true));
    return s_type;
}

}  // namespace base::reflection
