// Unit 85: reads a reflected field of an object into a reflected value (through the field's
// getter, or by converting the value at a byte offset into a value of the field's type).
// Names and layouts are placeholders.

#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/reflection.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"

using namespace base::reflection;

// An object and its type, as the walk passes it on.
struct UnkReflFieldObject {
    char* type;
    void* ptr;
};

struct UnkFieldAccessor {
    virtual void vf_0x0() {}
    virtual void vf_0x8() {}
    virtual UnkReflValue vf_0x10(const void* obj_ref, const void* index, u32 flags) = 0;
};

struct UnkReflField {
    u8 pad_0x0[8];
    u32 offset;
    const CType* type;
    UnkFieldAccessor* accessor;
};

UnkReflValue fn_71000e76dc(::UnkReflField* field, const UnkReflFieldObject* obj) {
    UnkReflValue result = refl_EmptyValue();
    UnkFieldAccessor* accessor = field->accessor;
    if (accessor) {
        UnkReflValue value;
        u64 index[2] = {0, 0};
        new (&value) UnkReflValue(accessor->vf_0x10(obj, index, 0));
        fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(&result),
                      reinterpret_cast<const UnkAssignedRecord_a1ba4*>(&value));
        fn_71000a1c60(&value);
        return result;
    }
    fn_71000a2014(&result, field->type);
    UnkTypedRef src;
    const CType* type = field->type;
    void* ptr = static_cast<u8*>(obj->ptr) + field->offset;
    struct {
        char* type;
        void* ptr;
    } ref = {fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(type)), ptr), ptr};
    fn_71000a174c(&src, &ref);
    UnkTypedRef dst;
    dst.type = reinterpret_cast<const CType*>(result.type);
    void* value_ptr;
    if (!result.type)
        value_ptr = nullptr;
    else if (result.field_0x14 == 1 || *reinterpret_cast<const u64*>(result.type + 0x18) > 8)
        value_ptr = result.field_0x0;
    else
        value_ptr = &result;
    dst.ptr = value_ptr;
    fn_710009d960(type, &src, &dst);
    return result;
}
