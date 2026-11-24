// Unit 85: sets a reflected field's value (through its setter, with the value as the single
// argument, or straight into the object at a byte offset). Names and layouts are placeholders.

#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/reflection.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkReflFieldObject.h"
#include "game/UnkTU85.h"

using namespace base::reflection;

// A typed reference to a value (a type and where the value is).
struct UnkReflValueArg {
    const CType* type;
    void* ptr;
};

struct UnkReflArgs {
    UnkReflValue* values;
    u64 count;
};

struct UnkFieldAccessor_l0e7a80 {
    virtual void vf_0x0() {}
    virtual void vf_0x8() {}
    virtual UnkReflValue vf_0x10(const void* obj_ref, const UnkReflArgs* args, u32 flags) = 0;
};

struct UnkReflField_l0e7a80 {
    u8 pad_0x0[8];
    u32 offset;
    const CType* type;
    UnkFieldAccessor_l0e7a80* getter;
    u8 pad_0x20[8];
    UnkFieldAccessor_l0e7a80* setter;
};

bool SetReflectedFieldValue(::UnkReflField* field_, const UnkReflFieldObject* obj, UnkReflValueArg* value) {
    UnkReflField_l0e7a80* field = reinterpret_cast<UnkReflField_l0e7a80*>(field_);
    UnkFieldAccessor_l0e7a80* setter = field->setter;
    if (setter) {
        UnkReflValue arg;
        UnkReflValue result;
        fn_71000a1978(&arg, reinterpret_cast<UnkTypedRef*>(value), true, 2);
        UnkReflArgs args = {&arg, 1};
        new (&result) UnkReflValue(setter->vf_0x10(obj, &args, 0));
        fn_71000a1c60(&result);
        fn_71000a1c60(&arg);
        return true;
    }
    if (field->offset == static_cast<u32>(-1))
        return false;
    const CType* type = field->type;
    void* ptr = static_cast<u8*>(obj->ptr) + field->offset;
    struct {
        char* type;
        void* ptr;
    } ref = {fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(type)), ptr), ptr};
    return fn_710009d7a8(type, reinterpret_cast<UnkTypedRef*>(&ref), reinterpret_cast<UnkTypedRef*>(value));
}
