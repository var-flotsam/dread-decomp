// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): the descriptor constructor of a
// reflected class built by the unit's static initialiser (fn_7100eb4510) rather than by a getter;
// the class's name is not known. Same shape as the generated T::refl_InitClass instantiations in
// src/reflection/. Names are placeholders.
#include "base/reflection/CClass.h"

DREAD_REFL_TAG(7100e7e428);
bool fn_7100ea9fe8(const void*, const void*);
s32 fn_7100ea9ff4();
const base::reflection::CType& fn_7100ea9ffc(const void* obj);
void fn_7100eaa008();
void fn_7100eaa00c(void* obj);
template <>
struct base::reflection::UnkReflClassTraits<UnkRefl_fn_7100e7e428> {
    static constexpr u64 size = 0xd64760;
    static constexpr bool abstract = false;
    static constexpr UnkFn_0x58 fn_0x58 = &fn_7100ea9fe8;
    static constexpr UnkFn_0x60 fn_0x60 = &fn_7100ea9ff4;
    static constexpr UnkFn_0x68 fn_0x68 = &fn_7100ea9ffc;
    static constexpr UnkFn fn_0x28 = &fn_7100eaa008;
    static constexpr UnkFn_0x30 fn_0x30 = nullptr;
    static constexpr UnkFn_0x30 fn_0x38 = nullptr;
    static constexpr UnkFn_0x40 fn_0x40 = &fn_7100eaa00c;
    static constexpr UnkFn_0x48 fn_0x48 = nullptr;
    static constexpr UnkFn_0x48 fn_0x50 = nullptr;
};

template base::reflection::CClass::CClass(const base::global::CStrId&, const base::reflection::CClass*,
                                          base::reflection::UnkFn_0x70, base::reflection::UnkFn_0x70,
                                          UnkRefl_fn_7100e7e428*);
