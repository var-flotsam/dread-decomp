// From unit 85 of tools/tu_map.py (0x71000e6ce0-0x71000e8728, no reflected class): where a reflected
// field of an object is, with its type. A field at a fixed offset (the word at +0x8; -1 for none)
// lies in the object's data (its +0x8) and its type (+0x10) is refined for it (fn_710009dcd8); a
// field without one is read through its accessor (+0x18, vf_0x10) into a temporary value, and the
// result points at the value's storage, which the value's destruction (fn_71000a1c60) leaves
// behind as in the original. Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "base/UnkCloneHelpers.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

struct UnkFieldOwner {
    void* vtable;
    char* data;  // +0x8
};

class UnkFieldAccessor {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkReflValue vf_0x10(UnkFieldOwner* owner, const UnkTypedRef& arg, u32 flags);
};

struct UnkReflFieldInfo {
    void* vtable;
    u32 offset;  // +0x8: 0xffffffff when the field has an accessor
    u8 pad_0xc[4];
    char* type;                  // +0x10
    UnkFieldAccessor* accessor;  // +0x18
};

UnkReflTypedPtr fn_71000e755c(const UnkReflFieldInfo* field, UnkFieldOwner* owner) {
    UnkReflTypedPtr r;
    if (field->offset == 0xffffffff) {
        UnkReflValue value = field->accessor->vf_0x10(owner, UnkTypedRef{nullptr, nullptr}, 0);
        r.type = const_cast<char*>(value.type);
        void* ptr;
        if (!value.type)
            ptr = nullptr;
        else if (value.field_0x14 == 1 || reinterpret_cast<const CType*>(value.type)->m_size >= 9)
            ptr = value.field_0x0;
        else
            ptr = &value;
        r.ptr = ptr;
        fn_71000a1c60(&value);
        return r;
    }
    void* ptr = owner->data + field->offset;
    char* type = field->type;
    if (ptr)
        type = fn_710009dcd8(type, ptr);
    r.type = type;
    r.ptr = ptr;
    return r;
}
