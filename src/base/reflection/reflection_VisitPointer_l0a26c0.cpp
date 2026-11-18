// base::reflection's unit (unit 82): a visitor's step into a typed pointer. In mode 1 it follows a pointer
// type's dereference function (+0xf0) to the object it points at and lets that object's type visit it with
// its dynamic type; then the visitor's own handler (vf_0xd8) runs. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkTypedRef;

struct UnkReflVisitor_l0a26c0;

struct UnkPointeeType_l0a26c0 {
    void** vtable;

    void vf_0x40(UnkTypedRef* value, UnkReflVisitor_l0a26c0* visitor) {
        reinterpret_cast<void (*)(UnkPointeeType_l0a26c0*, UnkTypedRef*, UnkReflVisitor_l0a26c0*)>(
            vtable[0x40 / 8])(this, value, visitor);
    }
};

struct UnkPointerType_l0a26c0 {
    u8 pad_0x0[0xf0];
    UnkReflTypedPtr (*deref)(void* ptr);  // 0xf0
};

struct UnkReflVisitor_l0a26c0 {
    void** vtable;

    s32 vf_0xd0() {
        return reinterpret_cast<s32 (*)(UnkReflVisitor_l0a26c0*)>(vtable[0xd0 / 8])(this);
    }
    void vf_0xd8(UnkTypedRef* value) {
        reinterpret_cast<void (*)(UnkReflVisitor_l0a26c0*, UnkTypedRef*)>(vtable[0xd8 / 8])(this, value);
    }
};

static inline UnkPointerType_l0a26c0* AsPointerType(const CType* type) {
    if (!type)
        return nullptr;
    const UnkReflObject* o = reinterpret_cast<const UnkReflObject*>(type);
    return fn_710009d50c(o->vf_0x0(), fn_710009bedc())
               ? reinterpret_cast<UnkPointerType_l0a26c0*>(const_cast<CType*>(type))
               : nullptr;
}

// What the value points at, when its type is a pointer type with a dereference function; else empty.
static inline UnkReflTypedPtr Pointee(UnkPointerType_l0a26c0* type, const UnkTypedRef* value) {
    if (type->deref && value->type == reinterpret_cast<const CType*>(type))
        return type->deref(value->ptr);
    UnkReflTypedPtr none;
    none.type = nullptr;
    none.ptr = nullptr;
    return none;
}

s32 fn_71000a26c0(UnkReflVisitor_l0a26c0* self, UnkTypedRef* value) {
    s32 mode = self->vf_0xd0();
    if (mode == 1) {
        UnkReflTypedPtr target = Pointee(AsPointerType(value->type), value);
        char* type = target.type;
        void* ptr = target.ptr;
        if (type && ptr) {
            UnkTypedRef dynamic = {reinterpret_cast<const CType*>(fn_710009dcd8(type, ptr)), ptr};
            reinterpret_cast<UnkPointeeType_l0a26c0*>(type)->vf_0x40(&dynamic, self);
        }
    }
    self->vf_0xd8(value);
    return mode;
}
