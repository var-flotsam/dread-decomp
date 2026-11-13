#pragma once

// A value with its reflected type, as the blackboard's section setter (fn_7100332c80) and the other typed
// setters take one. One definition of what 79 sources had each written out with the same text
// (tools/struct_migrate.py). Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
