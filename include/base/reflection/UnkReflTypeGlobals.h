#pragma once

// Reflected types the hand-written sources pass to the reflection registrations by address
// (a field's or a pointer's type) without knowing their kind: globals built by static
// initialisers, and the getter of one pointer type. Declared as the base CType, the type the
// generated reflection sources use for them.

#include "base/reflection/CClass.h"
extern base::reflection::CType g_unk_7101d5e2e0;
extern base::reflection::CType g_unk_7172794e78;
extern base::reflection::CType g_unk_71727bbe68;
const base::reflection::CType* fn_7100ad6568();
