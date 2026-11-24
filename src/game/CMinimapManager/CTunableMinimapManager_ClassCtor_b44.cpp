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
struct base::reflection::ClassTraits<UnkRefl_fn_7100e7e428> {
    static constexpr u64 size = 0xd64760;
    static constexpr bool abstract = false;
    static constexpr EqualsFn equals = &fn_7100ea9fe8;
    static constexpr HashFn hash = &fn_7100ea9ff4;
    static constexpr TypeOfFn type_of = &fn_7100ea9ffc;
    static constexpr SlotFn construct = &fn_7100eaa008;
    static constexpr CopyConstructFn copy_construct = nullptr;
    static constexpr CopyConstructFn move_construct = nullptr;
    static constexpr DestroyFn destroy = &fn_7100eaa00c;
    static constexpr AssignFn copy_assign = nullptr;
    static constexpr AssignFn move_assign = nullptr;
};

template base::reflection::CClass::CClass(const base::global::CStrId&, const base::reflection::CClass*,
                                          base::reflection::ClassSetupFn, base::reflection::ClassSetupFn,
                                          UnkRefl_fn_7100e7e428*);
