#pragma once

// Getters of plain CType descriptors (refl_GetType<T> for float, unsigned, bool, CStrId,
// CVector3D, CColor4F, ...): types described only by their name, size and the CType function
// slots. The kind hash is computed from the type's own name, so name and kind share one string.
// The getters are templates in the original (guard and descriptor reached through the GOT);
// generated instances live in src/reflection_types/ (tools/gen_types.py).

#include "base/reflection/CClass.h"

namespace base::reflection {

// Per-getter facts (generated): name and its length, size, and the function slots, typed as
// UnkFn and cast at the use site (null when the type lacks that function).
template <class G>
struct UnkReflPrimitiveTraits;

// Builds the descriptor in place (a prvalue, so the static is constructed directly). The name
// is built by the caller before the kind is hashed, as in the original.
template <class G>
inline CType refl_MakePrimitiveType(const base::global::CStrId& name) {
    using Tr = UnkReflPrimitiveTraits<G>;
    return CType(fn_7100001570(Tr::name, Tr::length), name, Tr::size, reinterpret_cast<UnkFn_0x58>(Tr::fn_0x58),
                 reinterpret_cast<UnkFn_0x60>(Tr::fn_0x60), reinterpret_cast<UnkFn_0x68>(Tr::fn_0x68),
                 reinterpret_cast<UnkFn_0x70>(Tr::fn_0x70), nullptr,
                 {Tr::fn_0x28, reinterpret_cast<UnkFn_0x30>(Tr::fn_0x30), reinterpret_cast<UnkFn_0x30>(Tr::fn_0x38),
                  reinterpret_cast<UnkFn_0x40>(Tr::fn_0x40), reinterpret_cast<UnkFn_0x48>(Tr::fn_0x48),
                  reinterpret_cast<UnkFn_0x48>(Tr::fn_0x50)},
                 false);
}

// refl_GetType<T> for a plain type. G is a tag named after the getter's address.
template <class G>
const CType* refl_GetPrimitiveType() {
    static CType s_type = refl_MakePrimitiveType<G>(base::global::CStrId(UnkReflPrimitiveTraits<G>::name, true));
    return &s_type;
}

}  // namespace base::reflection
