// Unit 82 (base::reflection): assigns a typed pointer to a reflected value. When the value holds
// a compatible type by copy, the object is converted into its storage in place; otherwise the old
// value is destroyed and the value rebuilt as a copy of the object. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

// Converts the object into the value's storage (as fn_71000a1cc8 does).
static inline void AssignInPlace(UnkReflValue* value, const CType* type, void* object) {
    const char* vt = value->type;
    UnkTypedRef dst;
    dst.type = reinterpret_cast<const CType*>(vt);
    void* p;
    if (!vt)
        p = nullptr;
    else if (value->field_0x14 == 1 || reinterpret_cast<const CType*>(vt)->m_size > 8)
        p = value->field_0x0;
    else
        p = value;
    dst.ptr = p;
    UnkTypedRef src;
    fn_71000a16e0(&src, reinterpret_cast<const char*>(type), object);
    fn_710009d7a8(reinterpret_cast<const CType*>(vt), &dst, &src);
}

// Destroys the value's object if it owns one and leaves the value empty.
static inline void Reset(UnkReflValue* value) {
    if (auto* old = reinterpret_cast<const CType*>(value->type)) {
        if (value->field_0x14 == 0) {
            if (old->m_size > 8) {
                old->m_destroy(value->field_0x0);
                GetDefaultAllocator()->vf_0x48(value->field_0x0);
            } else {
                old->m_destroy(value);
            }
        }
        value->type = nullptr;
    }
}

// Makes the value a copy of the object (as fn_71000a18d4 does with by_ref 0).
static inline void InitCopy(UnkReflValue* out, const CType* type, const void* object) {
    if (!type)
        return;
    out->field_0x14 = 0;
    if (!type->m_copyConstruct)
        return;
    // Stored as a plain pointer: the original reloads the copy function after this store.
    *reinterpret_cast<const CType**>(&out->type) = type;
    u64 size = type->m_size;
    if (size > 8) {
        void* block = GetDefaultAllocator()->vf_0x40(size, 8, out->kind);
        out->field_0x0 = block;
        if (!block)
            return;
        type->m_copyConstruct(block, object);
    } else {
        type->m_copyConstruct(out, object);
    }
}

// Whether the value holds, by copy, a type the object can be assigned to in place.
static inline bool CanAssignInPlace(const UnkReflValue* value, const UnkTypedRef* ref) {
    auto* cur = reinterpret_cast<const CType*>(value->type);
    return cur && ref->ptr && value->field_0x14 == 0 &&
           (cur == ref->type ? cur->m_copyAssign != nullptr : fn_710009d52c(ref->type, cur));
}

UnkReflValue* fn_71000a1d34(UnkReflValue* self, const UnkTypedRef* ref) {
    bool in_place = CanAssignInPlace(self, ref);
    const CType* type = ref->type;
    void* object = ref->ptr;
    if (in_place) {
        AssignInPlace(self, type, object);
    } else {
        Reset(self);
        InitCopy(self, type, object);
    }
    return self;
}
