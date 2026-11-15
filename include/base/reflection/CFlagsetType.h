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
struct FlagsetTypeDescription;

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
    : CType(fn_7100001570("base::reflection::CFlagsetType", 0x1e), name, FlagsetTypeDescription<T>::size,
            reinterpret_cast<UnkFn_0x58>(FlagsetTypeDescription<T>::equals),
            reinterpret_cast<UnkFn_0x60>(FlagsetTypeDescription<T>::hash),
            reinterpret_cast<UnkFn_0x68>(FlagsetTypeDescription<T>::type_of), nullptr, nullptr,
            {FlagsetTypeDescription<T>::construct, reinterpret_cast<UnkFn_0x30>(FlagsetTypeDescription<T>::copy_construct),
             reinterpret_cast<UnkFn_0x30>(FlagsetTypeDescription<T>::move_construct),
             reinterpret_cast<UnkFn_0x40>(FlagsetTypeDescription<T>::destroy),
             reinterpret_cast<UnkFn_0x48>(FlagsetTypeDescription<T>::copy_assign),
             reinterpret_cast<UnkFn_0x48>(FlagsetTypeDescription<T>::move_assign)},
            false),
      m_underlying(FlagsetTypeDescription<T>::underlying()), field_0xe8(FlagsetTypeDescription<T>::field_0xe8()) {}

}  // namespace base::reflection
