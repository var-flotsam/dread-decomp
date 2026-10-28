// Reflection getters (invokers) that always return true as a reflected bool, the counterpart of
// fn_7100711508 (which returns false): fn_7100254728 and fn_7100254784. Each is followed by a
// setter that ignores its argument (fn_7100254770, fn_71002547cc). Called only through their
// function descriptors.

#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

static inline UnkReflValue ReturnTrue() {
    bool value = true;
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

UnkReflValue fn_7100254728() { return ReturnTrue(); }

UnkReflValue fn_7100254784() { return ReturnTrue(); }
