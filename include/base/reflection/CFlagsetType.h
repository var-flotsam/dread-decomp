#pragma once

// base::reflection::CFlagsetType: the descriptor of a flag set. The name comes from the kind
// string its constructor hashes ("base::reflection::CFlagsetType", length 0x1e). Layout: CType,
// the underlying integer type's descriptor and a global descriptor (probably the flag enum's).
// Like enums, the descriptors are globals built by static initialisers; only the constructors
// are generated (src/reflection_types/, tools/gen_types.py).

#include "base/reflection/CClass.h"

namespace base::reflection {

// Per-type facts for CFlagsetType's constructor (generated): size, the CType function slots
// (typed UnkFn and cast at the use site), the underlying type's getter and the second
// descriptor.
template <class T>
struct UnkReflFlagsetTraits;

class CFlagsetType : public CType {
public:
    // One instantiation per flag set. `extra` is always null and unused; it deduces T.
    template <class T>
    CFlagsetType(const base::global::CStrId& name, T* extra);
    ~CFlagsetType() override;

    const CType* m_underlying;
    const CType* field_0xe8;
};
static_assert(sizeof(CFlagsetType) == 0xf0, "CFlagsetType layout");

template <class T>
CFlagsetType::CFlagsetType(const base::global::CStrId& name, T*)
    : CType(fn_7100001570("base::reflection::CFlagsetType", 0x1e), name, UnkReflFlagsetTraits<T>::size,
            reinterpret_cast<UnkFn_0x58>(UnkReflFlagsetTraits<T>::fn_0x58),
            reinterpret_cast<UnkFn_0x60>(UnkReflFlagsetTraits<T>::fn_0x60),
            reinterpret_cast<UnkFn_0x68>(UnkReflFlagsetTraits<T>::fn_0x68), nullptr, nullptr,
            {UnkReflFlagsetTraits<T>::fn_0x28, reinterpret_cast<UnkFn_0x30>(UnkReflFlagsetTraits<T>::fn_0x30),
             reinterpret_cast<UnkFn_0x30>(UnkReflFlagsetTraits<T>::fn_0x38),
             reinterpret_cast<UnkFn_0x40>(UnkReflFlagsetTraits<T>::fn_0x40),
             reinterpret_cast<UnkFn_0x48>(UnkReflFlagsetTraits<T>::fn_0x48),
             reinterpret_cast<UnkFn_0x48>(UnkReflFlagsetTraits<T>::fn_0x50)},
            false),
      m_underlying(UnkReflFlagsetTraits<T>::underlying()), field_0xe8(UnkReflFlagsetTraits<T>::field_0xe8()) {}

}  // namespace base::reflection
