// From unit 82 of tools/tu_map.py (0x71000a16c4-0x71000a4598, base::reflection): four builders of a
// reflected value (base/reflection/UnkReflValue.h) from a type and a pointer, with fn_71000a18d4's
// body inlined after the fields are set: fn_71000a1828 and fn_71000a1978 take the pair as it is,
// fn_71000a1a24 and fn_71000a1ae4 skip a null pointer; fn_71000a1a24 calls fn_710009dcd8 on the
// pair first (its result unused), fn_71000a1ae4 takes the pointer fn_71000a16e0 gives for it.
// Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

// fn_71000a18d4 (src/base/reflection/fn_71000a18d4.cpp), inlined.
__attribute__((always_inline)) static inline void SetValue(UnkReflValue* out, const CType* type, const void* value,
                                                           u32 by_ref) {
    out->field_0x14 = by_ref;
    if (by_ref == 1) {
        out->field_0x0 = const_cast<void*>(value);
        out->type = reinterpret_cast<const char*>(type);
        return;
    }
    if (!type->m_fn_0x30)
        return;
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

__attribute__((always_inline)) static inline void MakeValue(UnkReflValue* out, const UnkTypedRef* v, u32 by_ref,
                                                            u32 kind) {
    out->field_0x0 = nullptr;
    out->type = nullptr;
    out->kind = kind;
    out->field_0x14 = by_ref;
    if (const CType* type = v->type)
        SetValue(out, type, v->ptr, by_ref);
}

__attribute__((always_inline)) static inline void MakeDynamicValue(UnkReflValue* out, const UnkTypedRef* v,
                                                                   u32 by_ref, u32 kind) {
    out->field_0x0 = nullptr;
    out->type = nullptr;
    out->kind = kind;
    out->field_0x14 = by_ref;
    const CType* type = v->type;
    if (!type)
        return;
    const void* value = v->ptr;
    if (!value)
        return;
    fn_710009dcd8(reinterpret_cast<char*>(const_cast<CType*>(type)), const_cast<void*>(value));
    SetValue(out, type, value, by_ref);
}

void fn_71000a1828(UnkReflValue* out, const UnkTypedRef* v, u32 by_ref, u32 kind) {
    MakeValue(out, v, by_ref, kind);
}

void fn_71000a1978(UnkReflValue* out, const UnkTypedRef* v, u32 by_ref, u32 kind) {
    MakeValue(out, v, by_ref, kind);
}

void fn_71000a1a24(UnkReflValue* out, const UnkTypedRef* v, u32 by_ref, u32 kind) {
    MakeDynamicValue(out, v, by_ref, kind);
}

void fn_71000a1ae4(UnkReflValue* out, const UnkTypedRef* v, u32 by_ref, u32 kind) {
    out->field_0x0 = nullptr;
    out->type = nullptr;
    out->kind = kind;
    out->field_0x14 = by_ref;
    const CType* type = v->type;
    if (!type)
        return;
    void* value = v->ptr;
    if (!value)
        return;
    UnkTypedRef ref;
    fn_71000a16e0(&ref, reinterpret_cast<const char*>(type), value);
    SetValue(out, type, ref.ptr, by_ref);
}
