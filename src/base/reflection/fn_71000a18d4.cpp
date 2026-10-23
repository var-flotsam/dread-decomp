// Builds a reflected value (base::reflection::UnkReflValue, declared in UnkReflValue.h) from a
// type and a pointer to a value. By reference (mode 1) it keeps the pointer; otherwise it
// copies the value with the type's copy function (CType slot 0x30), inline when it fits in 8
// bytes, else into a block from the default allocator (allocation flags from the value's kind).
// A null type or a type without a copy function leaves the value as it is.
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflValue.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

void fn_71000a18d4(UnkReflValue* out, const CType* type, const void* value, u32 by_ref) {
    if (!type)
        return;
    out->field_0x14 = by_ref;
    if (by_ref == 1) {
        out->field_0x0 = const_cast<void*>(value);
        out->type = reinterpret_cast<const char*>(type);
        return;
    }
    if (!type->m_fn_0x30)
        return;
    // Stored as a plain pointer: the original reloads the copy function after this store.
    *reinterpret_cast<const CType**>(&out->type) = type;
    u64 size = type->m_size;
    if (size > 8) {
        void* block = fn_7100083990()->vf_0x40(size, 8, out->kind);
        out->field_0x0 = block;
        if (!block)
            return;
        type->m_fn_0x30(block, value);
    } else {
        type->m_fn_0x30(out, value);
    }
}
