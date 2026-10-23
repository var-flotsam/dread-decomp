// The is-kind-of test of reflected types (declared in base/reflection/UnkReflCast.h): whether
// `type` is `base` or derives from it, walking the parent chain (CType::m_parent).
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"

bool fn_710009d50c(const base::reflection::CType* type, const base::reflection::CType* base) {
    for (; type; type = type->m_parent) {
        if (type == base)
            return true;
    }
    return false;
}
