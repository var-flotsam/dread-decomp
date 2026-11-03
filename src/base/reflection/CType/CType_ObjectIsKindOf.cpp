// base::reflection (unit 81): whether a reflected object's class (vtable slot 0) is `type` or
// derives from it. As fn_710009d50c inlined, except that the object's class is not tested for
// null before the first comparison.
#include "base/reflection/CClass.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

bool fn_710009d760(const UnkReflObject* obj, const CType* type) {
    const CType* t = obj->vf_0x0();
    do {
        if (t == type)
            return true;
        t = t->m_parent;
    } while (t);
    return false;
}
