#pragma once

// Reflected types the hand-written sources pass to the reflection registrations by address
// (a field's or a pointer's type) without knowing their kind: globals built by static
// initialisers, and the getter of one pointer type. Declared as the base CType, the type the
// generated reflection sources use for them.

#include "base/reflection/CClass.h"

// base::global::CCollectionKeyElementRefBase, the base class of the collections' key/element types.
extern base::reflection::CType g_unk_7101d5e2e0;
// Hidden: the original addresses this global directly (adrp/add on its own address), not through a
// GOT slot like the other globals here.
extern base::reflection::CType g_unk_7172794e78 __attribute__((visibility("hidden")));
// The type of the CBaseGameLink class.
extern base::reflection::CType g_unk_71727bbe68;
// refl_GetType<CLightManager*>: the generated getter of the pointer type (defined in generated code).
const base::reflection::CType* fn_7100ad6568();
