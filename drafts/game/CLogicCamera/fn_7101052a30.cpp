// CLogicCamera's unit (tools/tu_map.py unit 2655): the counted weak reference to the logic camera that a
// reflected property value holds, directly or through a pointer (the CLogicCamera copy of fn_710023b580).
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using namespace base::reflection;

// A reflected property read of `member`: the value, then 0x18 bytes the read zeroes.
struct UnkReflReadResult {
    UnkReflValue value;
    u8 unk_0x18[0x18];
};
static_assert(sizeof(UnkReflReadResult) == 0x30, "UnkReflReadResult layout");
UnkReflReadResult fn_7100ad7cc0(void* member);

// CLogicCamera's class (initialised by CLogicCamera::refl_GetClass).
extern char g_unk_71729fe020[] __attribute__((visibility("hidden")));

// Destroys the read value when the function returns.
struct UnkReadGuard_l052a30 {
    UnkReflValue* value;
    ~UnkReadGuard_l052a30() { fn_71000a1c60(value); }
};

// A pointer type's function at +0xf0: the pointee of a pointer value, its type and the object.
typedef UnkReflTypedPtr (*UnkPointeeFn_l052a30)(void* pointer);

// Where a reflected value's data is: inline when it fits in 8 bytes, else through field_0x0.
inline void* ValueData(UnkReflValue& value) {
    const char* type = value.type;
    if (!type)
        return nullptr;
    if (value.field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) >= 9)
        return value.field_0x0;
    return &value;
}

inline bool IsPointerType(const char* type) {
    return fn_710009d50c(reinterpret_cast<const UnkReflObject*>(type)->vf_0x0(), fn_710009bedc());
}

// `object` if its type is a CLogicCamera, else null.
inline UnkWeakTarget* AsLogicCamera(const char* type, void* object) {
    fn_7101050878();
    if (!fn_710009d50c(reinterpret_cast<const CType*>(type), reinterpret_cast<const CType*>(g_unk_71729fe020)))
        return nullptr;
    return static_cast<UnkWeakTarget*>(object);
}

UnkRefPtr fn_7101052a30(void* member) {
    UnkReflReadResult read = fn_7100ad7cc0(member);
    UnkReflValue& value = read.value;
    UnkReadGuard_l052a30 guard = {&value};
    UnkWeakHandle* handle = nullptr;
    if (value.type) {
        UnkWeakTarget* camera;
        bool is_pointer = IsPointerType(value.type);
        char* type = const_cast<char*>(value.type);
        if (is_pointer) {
            void* data = ValueData(value);
            char* pointer_type = type && IsPointerType(type) ? type : nullptr;
            UnkPointeeFn_l052a30 pointee_of = *reinterpret_cast<UnkPointeeFn_l052a30*>(pointer_type + 0xf0);
            UnkReflTypedPtr pointee;
            if (pointee_of && type == pointer_type) {
                pointee = pointee_of(data);
            } else {
                pointee.type = nullptr;
                pointee.bits = 0;
            }
            camera = AsLogicCamera(fn_710009dcd8(pointee.type, pointee.ptr), pointee.ptr);
        } else {
            fn_7101050878();
            bool is = fn_710009d50c(reinterpret_cast<const CType*>(type),
                                    reinterpret_cast<const CType*>(g_unk_71729fe020));
            camera = is ? static_cast<UnkWeakTarget*>(ValueData(value)) : nullptr;
        }
        handle = GetWeakHandle(camera);
    }
    UnkRefPtr result;
    result.field_0x0 = handle;
    if (handle && ++handle->ref_count == 0)
        handle->vf_0x10();
    return result;
}
