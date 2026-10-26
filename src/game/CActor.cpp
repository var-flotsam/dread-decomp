// A function of the CActor unit (unit 341, tools/tu_map.py): the counted weak reference to the actor
// that a reflected property value holds.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game_s2/UnkWeakHandle.h"

using namespace base::reflection;

// A reflected property read of `member` (the first member of a call's arguments).
UnkReflValue fn_7100ad7cc0(void* member);
// The class of the actor (a CClass object, initialised by CActor::refl_GetClass).
extern char g_unk_717269f450[] __attribute__((visibility("hidden")));

namespace {

// Destroys the reflected value when the function returns.
struct ValueGuard {
    UnkReflValue* value;
    ~ValueGuard() { fn_71000a1c60(value); }
};

// A pointer type's function at +0xf0: reads the pointee out of the pointer value, giving its type
// and the object (both zero for a null pointer); returned in memory.
typedef UnkReflTypedPtr (*UnkPointeeFn)(void* pointer);

}  // namespace

// The reference to the object the reflected value `member` reads as (an actor, directly or through
// a pointer), or null.
// NON_MATCHING(minor): same behaviour; the original has a 0x80-byte frame, keeps the pointee pair in
// a stack slot addressed through x23 and lays the two branches out differently.
UnkRefPtr fn_710023b580(void* member) {
    UnkReflValue value = fn_7100ad7cc0(member);
    ValueGuard guard = {&value};
    UnkRefPtr result;
    result.field_0x0 = nullptr;
    UnkWeakTarget* object = nullptr;
    if (value.type) {
        const UnkReflObject* type = reinterpret_cast<const UnkReflObject*>(value.type);
        bool is_pointer = fn_710009d50c(type->vf_0x0(), fn_710009bedc());
        if (!is_pointer) {
            fn_7100227448();
            fn_710009d50c(reinterpret_cast<const CType*>(value.type), reinterpret_cast<const CType*>(g_unk_717269f450));
            char* t = const_cast<char*>(value.type);
            void* addr = !t ? nullptr
                            : (value.field_0x14 != 1 && *reinterpret_cast<u64*>(t + 0x18) < 9) ? static_cast<void*>(&value)
                                                                                                  : value.field_0x0;
            object = static_cast<UnkWeakTarget*>(addr);
        } else {
            char* t = const_cast<char*>(value.type);
            void* addr = !t ? nullptr
                            : (value.field_0x14 != 1 && *reinterpret_cast<u64*>(t + 0x18) < 9) ? static_cast<void*>(&value)
                                                                                                  : value.field_0x0;
            char* pointer_type = nullptr;
            if (fn_710009d50c(reinterpret_cast<const UnkReflObject*>(t)->vf_0x0(), fn_710009bedc()))
                pointer_type = t;
            UnkPointeeFn read = *reinterpret_cast<UnkPointeeFn*>(pointer_type + 0xf0);
            UnkReflTypedPtr pointee;
            if (read && t == pointer_type) {
                pointee = read(addr);
            } else {
                pointee.type = nullptr;
                pointee.bits = 0;
            }
            char* refined = fn_710009dcd8(pointee.type, pointee.ptr);
            fn_7100227448();
            fn_710009d50c(reinterpret_cast<const CType*>(refined), reinterpret_cast<const CType*>(g_unk_717269f450));
            object = static_cast<UnkWeakTarget*>(pointee.ptr);
        }
        if (UnkWeakHandle* handle = GetWeakHandle(object)) {
            result.field_0x0 = handle;
            if (++handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    return result;
}
