// Unit 82 (base::reflection): hands a flag-set value to a visitor by the width of its underlying
// integer, as a u32 or a u64. Names and layouts are placeholders.
#include "base/reflection/CFlagsetType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkFlagsetVisitor_l0a346c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual s32 VisitU32(u32* value);
    virtual s32 VisitU64(u64* value);
};

struct UnkTypedValue_l0a346c {
    UnkReflObject* type;
    void* ptr;
};

static inline const base::reflection::CFlagsetType* FlagsetOf(UnkTypedValue_l0a346c* value) {
    UnkReflObject* type = value->type;
    return fn_710009d50c(type->vf_0x0(), fn_710009a11c())
               ? static_cast<const base::reflection::CFlagsetType*>(reinterpret_cast<const base::reflection::CType*>(type))
               : nullptr;
}

static inline u64 FlagsetSize(UnkTypedValue_l0a346c* value) {
    return FlagsetOf(value)->m_underlying->m_size;
}

// The bits as a T, read at the width of the underlying integer.
template <class T>
static inline T FlagsetBits(UnkTypedValue_l0a346c* value) {
    switch (FlagsetSize(value)) {
    case 4:
        return static_cast<T>(*static_cast<u32*>(value->ptr));
    case 8:
        return static_cast<T>(*static_cast<u64*>(value->ptr));
    default:
        return 0;
    }
}

s32 fn_71000a346c(UnkFlagsetVisitor_l0a346c* self, UnkTypedValue_l0a346c* value) {
    if (FlagsetSize(value) == 4) {
        u32 bits = FlagsetBits<u32>(value);
        return self->VisitU32(&bits);
    }
    if (FlagsetSize(value) == 8) {
        u64 bits = FlagsetBits<u64>(value);
        return self->VisitU64(&bits);
    }
    return 0;
}
