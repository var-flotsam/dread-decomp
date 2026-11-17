#pragma once

// The value operations a class template's instances declare when reflection describes them (CType+0x28..+0x68)
// and their description: tools/gen_reflection.py specialises them per instance (KeyElementValue.h, CGameLink.h).
// Static members, so they change nothing about the class's layout.
#define DREAD_REFLECTED_VALUE_MEMBERS                                                                   \
    static void refl_Construct(void* obj);                                                              \
    static void refl_CopyConstruct(void* dst, const void* src);                                         \
    static void refl_MoveConstruct(void* dst, const void* src);                                         \
    static void refl_Destroy(void* obj);                                                                \
    static void* refl_CopyAssign(void* dst, const void* src);                                           \
    static void* refl_MoveAssign(void* dst, const void* src);                                           \
    static bool refl_Equals(const void* a, const void* b);                                              \
    static s32 refl_Hash();                                                                             \
    static const base::reflection::CType& refl_TypeOf(const void* obj);                                 \
    struct refl_Description

#include "types.h"

namespace base::reflection {
class CType;
}  // namespace base::reflection
