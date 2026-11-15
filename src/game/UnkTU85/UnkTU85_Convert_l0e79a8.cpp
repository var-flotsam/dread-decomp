// Unit 85: gets a reflected field's value (through its accessor, or straight from the object at
// a byte offset, typed by the field's type) and converts it into the destination typed reference.
// Names and layouts are placeholders.

#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/reflection.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTU85.h"

using namespace base::reflection;

struct UnkReflFieldObject {
    char* type;
    void* ptr;
};

struct UnkReflValueArg {
    const CType* type;
    void* ptr;
};

struct UnkFieldAccessor_l0e79a8 {
    virtual void vf_0x0() {}
    virtual void vf_0x8() {}
    virtual UnkReflValue vf_0x10(const void* obj_ref, const void* index, u32 flags) = 0;
};

struct UnkReflField_l0e79a8 {
    u8 pad_0x0[8];
    u32 offset;
    const CType* type;
    UnkFieldAccessor_l0e79a8* accessor;
};

bool fn_71000e79a8(::UnkReflField* field_, const UnkReflFieldObject* obj, UnkReflValueArg* dst) {
    UnkReflField_l0e79a8* field = reinterpret_cast<UnkReflField_l0e79a8*>(field_);
    UnkFieldAccessor_l0e79a8* accessor = field->accessor;
    if (accessor) {
        UnkReflValueArg arg;
        UnkReflValue value;
        char* obj_type = obj->type;
        void* obj_ptr = obj->ptr;
        struct {
            char* type;
            void* ptr;
        } ref = {fn_710009dcd8(obj_type, obj_ptr), obj_ptr};
        u64 index[2] = {0, 0};
        new (&value) UnkReflValue(accessor->vf_0x10(&ref, index, 0));
        const CType* dst_type = dst->type;
        fn_71000a1710(&arg, &value);
        fn_710009d7a8(dst_type, reinterpret_cast<UnkTypedRef*>(dst), reinterpret_cast<UnkTypedRef*>(&arg));
        fn_71000a1c60(&value);
        return true;
    }
    UnkTypedRef src;
    const CType* type = field->type;
    void* ptr = static_cast<u8*>(obj->ptr) + field->offset;
    struct {
        char* type;
        void* ptr;
    } ref = {fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(type)), ptr), ptr};
    fn_71000a174c(&src, &ref);
    return fn_710009d960(type, &src, reinterpret_cast<UnkTypedRef*>(dst));
}
