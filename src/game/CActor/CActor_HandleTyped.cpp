// Copies of fn_7101120428 (src/game/CSpawnGroupComponent/CSpawnGroupComponent_HandleTyped.cpp) in the
// CActor and CCentralUnitAIComponent units: the object a handle refers to (its holder's +0x10), with its
// dynamic type descriptor refined from the class getter as refl_PointeeTyped does, or the class getter's
// descriptor and null when the handle is empty. Names and layouts are placeholders.
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

// CGameObject handle (CActor unit).
UnkReflTypedPtr fn_710023da28(UnkHandleHolder* const* handle) {
    return HandleTyped<fn_7101043688>(handle);
}

// CEntity handle (CCentralUnitAIComponent unit).
UnkReflTypedPtr fn_71004073b4(UnkHandleHolder* const* handle) {
    return HandleTyped<fn_7100d70c20>(handle);
}
