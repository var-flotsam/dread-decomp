// base::reflection::CEnumRef's slot 0x58 (and its twin for the type next to it): assigns a value
// of the given primitive type to the referenced enum through fn_710009d5d4, as (type, object)
// references; the enum's own reference is built by fn_71000a174c.
#include "base/UnkCloneHelpers.h"
#include "base/reflection.h"
#include "types.h"

bool fn_710009d5d4(const char* type, base::reflection::UnkTypedRef* dst, const base::reflection::UnkTypedRef* src);

using base::reflection::CType;
using base::reflection::UnkTypedRef;

const CType* fn_7100098c9c();  // refl_GetType<base::reflection::CEnumRef>
const CType* fn_7100099b94();

struct UnkEnumRef {
    const char* type;
    void* ptr;
};

template <const CType* (*GetType)()>
static __attribute__((always_inline)) bool AssignEnumRef(const void* a, const void* b) {
    const UnkEnumRef* ref = static_cast<const UnkEnumRef*>(a);
    const char* type = ref->type;
    UnkTypedRef dst;
    fn_71000a174c(&dst, ref);
    UnkTypedRef src = {GetType(), const_cast<void*>(b)};
    return fn_710009d5d4(type, &dst, &src);
}

bool fn_7100099040(const void* a, const void* b) {
    return AssignEnumRef<fn_7100098c9c>(a, b);
}

bool fn_7100099f38(const void* a, const void* b) {
    return AssignEnumRef<fn_7100099b94>(a, b);
}
