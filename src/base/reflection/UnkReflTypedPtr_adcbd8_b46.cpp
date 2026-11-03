// base::reflection unit (tools/tu_map.py unit 1635): two copies of a pointee-typed helper (as
// refl_PointeeTyped in include/base/reflection/UnkReflTypedPtr.h): the object a pointer points to,
// paired with its dynamic type descriptor (the static descriptor, fn_7100adc1d8, refined twice), or
// with the static descriptor alone when the pointer is null. The descriptor getter is called once
// per branch, through a second function the linker folded into it. Names are placeholders.
#include "base/reflection/UnkReflTypedPtr.h"
#include "types.h"

using base::reflection::UnkReflTypedPtr;

struct UnkReflType;
UnkReflType* fn_7100adc1d8();
namespace folded {
UnkReflType* fn_7100adc1d8();
}

static inline UnkReflTypedPtr PointeeTyped(void* const* p) {
    void* obj = *p;
    UnkReflTypedPtr r;
    char* t;
    if (obj) {
        t = fn_710009dcd8(reinterpret_cast<char*>(fn_7100adc1d8()), obj);
        t = fn_710009dcd8(t, obj);
    } else {
        t = reinterpret_cast<char*>(folded::fn_7100adc1d8());
    }
    r.type = t;
    r.ptr = obj;
    return r;
}

UnkReflTypedPtr fn_7100adcbd8(void* const* p) { return PointeeTyped(p); }
UnkReflTypedPtr fn_7100adce28(void* const* p) { return PointeeTyped(p); }
