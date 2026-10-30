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

// Makes `value` a default-constructed value of `type`: the old value is destroyed first (CType slot
// 0x40; a block from the default allocator is freed after), then the type's constructor (slot 0x28)
// runs inline when the value fits in 8 bytes, else in a new block. A type without a constructor
// leaves the value empty.
void fn_71000a2014(UnkReflValue* value, const CType* type) {
    if (auto* old = reinterpret_cast<const CType*>(value->type)) {
        if (value->field_0x14 == 0) {
            if (old->m_size > 8) {
                old->m_fn_0x40(value->field_0x0);
                fn_7100083990()->vf_0x48(value->field_0x0);
            } else {
                old->m_fn_0x40(value);
            }
        }
        value->type = nullptr;
    }
    if (!type->m_fn_0x28)
        return;
    // Stored as a plain pointer: the constructor is reloaded after this store.
    *reinterpret_cast<const CType**>(&value->type) = type;
    value->field_0x14 = 0;
    u64 size = type->m_size;
    if (size > 8) {
        void* block = fn_7100083990()->vf_0x40(size, 8, value->kind);
        value->field_0x0 = block;
        if (block)
            reinterpret_cast<void (*)(void*)>(type->m_fn_0x28)(block);
    } else {
        reinterpret_cast<void (*)(void*)>(type->m_fn_0x28)(value);
    }
}

// ---- fn_71000a1cc8: assigning an object of a type into a value ----

#include "base/UnkCloneHelpers.h"
#include "base/reflection/UnkReflCast.h"

// Assigns `object` (of `type`, its dynamic type looked up through the type's getter) into the
// value's storage, converted to the value's type (fn_710009d7a8). The value's storage is its first
// word when it is stored by reference or larger than 8 bytes, else the value itself; none when it
// has no type.
void fn_71000a1cc8(base::reflection::UnkReflValue* value, const base::reflection::CType* type, const void* object) {
    using base::reflection::UnkTypedRef;
    const char* vt = value->type;
    UnkTypedRef dst;
    dst.type = reinterpret_cast<const base::reflection::CType*>(vt);
    void* p;
    if (!vt)
        p = nullptr;
    else if (value->field_0x14 == 1 || *reinterpret_cast<const u64*>(vt + 0x18) > 8)
        p = value->field_0x0;
    else
        p = value;
    dst.ptr = p;
    UnkTypedRef src;
    fn_71000a16e0(&src, reinterpret_cast<const char*>(type), const_cast<void*>(object));
    fn_710009d7a8(reinterpret_cast<const base::reflection::CType*>(vt), &dst, &src);
}
