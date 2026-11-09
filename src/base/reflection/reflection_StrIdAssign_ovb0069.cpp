// base::reflection's unit (tools/tu_map.py): assigns a string id from a typed value (the value first, its
// type at +0x8), checked to be a string id (refl_GetType<base::global::CStrId>, fn_7100009658). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;

struct UnkReflTypedValue_ovb0069 {
    u64 value;
    const CType* type;  // 0x8
};

template <class T, const CType* (*GetType)()>
static inline T* As(const UnkReflTypedValue_ovb0069* v) {
    return fn_710009d50c(v->type, GetType()) ? reinterpret_cast<T*>(const_cast<UnkReflTypedValue_ovb0069*>(v))
                                             : nullptr;
}

CStrId* fn_7100adc4c0(CStrId* self, const UnkReflTypedValue_ovb0069* v) {
    *self = *As<CStrId, fn_7100009658>(v);
    return self;
}
