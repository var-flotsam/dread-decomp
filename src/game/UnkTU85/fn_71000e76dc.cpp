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
#include "game/UnkTU85.h"
#include "game/UnkTunableFieldHandle.h"
#include "game/UnkTunableReflValue.h"

using namespace base::reflection;

// Reads the value instead of the field's offset when present.
struct UnkTunableFieldAccessor {
    virtual void vf_0x0() {}
    virtual void vf_0x8() {}
    virtual UnkReflValue vf_0x10(const void* obj_ref, const void* index, u32 flags) = 0;
};

UnkTunableReflValue fn_71000e76dc(UnkTunableFieldHandle* field, const UnkReflTypedPtr* obj) {
    UnkTunableReflValue result = refl_EmptyValue();
    UnkTunableFieldAccessor* accessor = field->accessor;
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
    CType_AssignOrConvertValue(type, &src, &dst);
    return result;
}
