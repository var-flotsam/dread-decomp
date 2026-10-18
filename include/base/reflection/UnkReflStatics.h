#pragma once

// Getters of guarded function-local statics that are templates in the original: their guard
// and object are weak, so both are reached through the GOT, and the object's destructor is
// registered with __cxa_atexit. G is a tag naming one instantiation (tools/gen_statics.py);
// UnkReflStatic<G> gives its constants.

#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"

namespace base::reflection {

template <class G>
struct UnkReflStatic;

// An attribute key (e.g. "Description"): name, value type, a u32 (0xf).
template <class G>
const UnkReflAttrKey& refl_GetAttrKey() {
    static UnkReflAttrKey s_key(base::global::CStrId(UnkReflStatic<G>::name, true), UnkReflStatic<G>::type(),
                                UnkReflStatic<G>::field_0x10);
    return s_key;
}

// A string id built once from a literal.
template <class G>
const base::global::CStrId& refl_GetStaticStrId() {
    static base::global::CStrId s_id(UnkReflStatic<G>::name, true);
    return s_id;
}

// A string id copied once from a global one.
template <class G>
const base::global::CStrId& refl_GetStaticStrIdCopy() {
    static base::global::CStrId s_id(UnkReflStatic<G>::source());
    return s_id;
}

// A typed lookup by a static name: find(owner, name) returns an object, kept only if
// is_a(its class, type()) holds (a component or child of type T, by name).
template <class G>
UnkReflObject* refl_FindTyped(void* owner) {
    static base::global::CStrId s_name(UnkReflStatic<G>::name, true);
    UnkReflObject* obj = UnkReflStatic<G>::find(owner, s_name);
    return obj && UnkReflStatic<G>::is_a(obj->vf_0x0(), UnkReflStatic<G>::type()) ? obj : nullptr;
}

}  // namespace base::reflection
