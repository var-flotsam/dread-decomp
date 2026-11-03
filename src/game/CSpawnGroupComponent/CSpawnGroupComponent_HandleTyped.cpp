// CSpawnGroupComponent and lua_lib units: the object a handle refers to (its holder's +0x10), with
// its dynamic type descriptor refined from the class getter as refl_PointeeTyped does
// (include/base/reflection/UnkReflTypedPtr.h), or the class getter's descriptor and null when the
// handle is empty. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "types.h"

using base::reflection::UnkReflTypedPtr;

struct UnkHandleHolder {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

template <const base::reflection::CClass* (*Get)()>
static inline UnkReflTypedPtr HandleTyped(UnkHandleHolder* const* handle) {
    UnkHandleHolder* holder = *handle;
    void* obj;
    UnkReflTypedPtr r;
    if (holder && (obj = holder->object)) {
        char* t = (char*)Get();
        t = fn_710009dcd8(t, obj);
        r.type = fn_710009dcd8(t, obj);
        r.ptr = obj;
        return r;
    }
    r.type = (char*)Get();
    r.bits = 0;
    return r;
}

UnkReflTypedPtr fn_7101120428(UnkHandleHolder* const* handle) {
    return HandleTyped<fn_7101123918>(handle);
}

UnkReflTypedPtr fn_7100fc1264(UnkHandleHolder* const* handle) {
    return HandleTyped<fn_71011c3868>(handle);
}
