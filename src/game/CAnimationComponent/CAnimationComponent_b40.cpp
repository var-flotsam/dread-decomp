// CAnimationComponent unit (tools/tu_map.py, the name from reflection): a reflected function that
// passes its first two arguments (null when an argument has no type; inline, or behind a pointer for
// values over 8 bytes or kind 1) to fn_710117b1c8 on the object at 8 and returns the int 1 as a
// reflected value. Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_710117b1c8(char* self, void* a, void* b);
struct UnkStruct_710117af10;
struct UnkCls_710117af10;
bool fn_710117af10(UnkStruct_710117af10* self, UnkCls_710117af10** name, bool skipBusy);
bool fn_7101177898(char* self, void* a, void* b);

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_710117bc98(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* p0 = ArgPtr_b40a3(&a[0]);
    void* p1 = ArgPtr_b40a3(&a[1]);
    fn_710117b1c8(target, p0, p1);
    s32 one = 1;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &one, false);
    return r;
}

static inline __attribute__((always_inline)) UnkReflValue ReturnBool_b40a3(bool& v) {
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}

// The same with a bool second argument (never null), returning fn_710117af10's bool.
UnkReflValue fn_710117c05c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* p0 = ArgPtr_b40a3(&a[0]);
    bool skip = *base::reflection::refl_ArgValue<bool>(&a[1]);
    bool v = fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(target), static_cast<UnkCls_710117af10**>(p0), skip);
    return ReturnBool_b40a3(v);
}

// Two nullable arguments to fn_7101177898, returning its bool.
UnkReflValue fn_710117c194(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* p0 = ArgPtr_b40a3(&a[0]);
    void* p1 = ArgPtr_b40a3(&a[1]);
    bool v = fn_7101177898(target, p0, p1);
    return ReturnBool_b40a3(v);
}
