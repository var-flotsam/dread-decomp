// CType unit (81): hands a reflected value to a visitor by its type's id: primitives by pointer to their
// own visit function, strings as text (or as a typed value when the visitor declines the text), dynamic
// types with their object's class, anything else as a typed value. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CType.h"
#include "base/reflection/PrimitiveType.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::CType;
using base::reflection::PrimitiveType;
using base::reflection::TypeOfFn;
using base::reflection::UnkTypedRef;

// The string types' getters (src/reflection_types/).
namespace base::global {
class TRntString32;
class TRntString64;
class TRntString128;
class TRntString256;
class TRntString512;
}  // namespace base::global
extern template const CType& PrimitiveType<base::global::TRntString32>::Get();
extern template const CType& PrimitiveType<base::global::TRntString64>::Get();
extern template const CType& PrimitiveType<base::global::TRntString128>::Get();
extern template const CType& PrimitiveType<base::global::TRntString256>::Get();
extern template const CType& PrimitiveType<base::global::TRntString512>::Get();

// A visitor over reflected values: one function per primitive type, taking a pointer to the value.
struct UnkReflVisitor {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual u32 vf_0x10(void* value);
    virtual u32 vf_0x18(void* value);
    virtual u32 vf_0x20(void* value);
    virtual u32 vf_0x28(void* value);
    virtual u32 vf_0x30(void* value);
    virtual u32 vf_0x38(void* value);
    virtual u32 vf_0x40(void* value);
    virtual u32 vf_0x48(void* value);
    virtual u32 vf_0x50(void* value);
    virtual u32 vf_0x58(void* value);
    virtual u32 vf_0x60(void* value);
    virtual u32 vf_0x68(void* value);
    virtual u32 vf_0x70(void* value);
    virtual u32 vf_0x78(void* value);
    virtual u32 vf_0x80(void* value);
    virtual u32 vf_0x88(void* value);
    virtual u32 vf_0x90(void* value);
    virtual u32 vf_0x98(void* value);
    virtual u32 vf_0xa0(void* value);
    virtual u32 vf_0xa8(void* value);
    virtual u32 vf_0xb0(void* value);
    virtual u32 vf_0xb8(void* value);
    virtual u32 vf_0xc0(void* value);
    virtual u32 vf_0xc8(const UnkTypedRef* value);
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual u32 vf_0xe0(const UnkTypedRef* value);
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual u32 vf_0x110(const UnkTypedRef* value);
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual u32 vf_0x128(const UnkTypedRef* value);
    virtual u32 vf_0x130(const UnkTypedRef* value);
    virtual u32 vf_0x138(const UnkTypedRef* value);  // any value, with its type
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual u32 vf_0x168(const char* text);  // nonzero when it took the text
};

// The value with its object's own class (none when the type has no class getter).
inline UnkTypedRef DynamicRef(const CType* type, void* data) {
    TypeOfFn type_of = type->m_fn_0x68;
    UnkTypedRef r;
    r.type = type_of ? &type_of(data) : nullptr;
    r.ptr = data;
    return r;
}

// The value as a typed value of `type`.
inline u32 VisitTyped(UnkReflVisitor* visitor, const CType* type, void* data) {
    UnkTypedRef r;
    r.type = type;
    r.ptr = data;
    return visitor->vf_0x138(&r);
}

// A string: its text first; when the visitor does not take it, the string as a typed value.
template <auto GetType>
inline u32 VisitString(UnkReflVisitor* visitor, void* data) {
    CRntString* str = static_cast<CRntString*>(data);
    if (visitor->vf_0x168(str->data ? str->data : g_unk_710162e2ed))
        return 1;
    return VisitTyped(visitor, &GetType(), data);
}

u32 fn_710009e1b0(void*, const UnkTypedRef* ref, UnkReflVisitor* visitor) {
    const CType* type = ref->type;
    switch (static_cast<s64>(type->m_kind)) {
    case 0x6a0c7bde338b1a2b:
        return visitor->vf_0x10(ref->ptr);
    case (s64)0x937459ba5ed68a51:
        return visitor->vf_0x18(ref->ptr);
    case 0x33724ee255148965:
        return visitor->vf_0x20(ref->ptr);
    case (s64)0x90a968ee29b77873:
        return visitor->vf_0x28(ref->ptr);
    case 0x2bf510999182b4a5:
        return visitor->vf_0x30(ref->ptr);
    case 0x291e3f130f064f1d:
        return visitor->vf_0x38(ref->ptr);
    case 0x2dfd9099e62c625c:
        return visitor->vf_0x40(ref->ptr);
    case 0x426ca69fe5192e7c:
        return visitor->vf_0x48(ref->ptr);
    case (s64)0xb3589d95fdbdfbd1:
        return visitor->vf_0x50(ref->ptr);
    case (s64)0xb97cd894d76cacd6:
        return visitor->vf_0x58(ref->ptr);
    case (s64)0xd8a335396f1e41ad:
        return visitor->vf_0x60(ref->ptr);
    case (s64)0xb732033f6c2b0d8d:
        return visitor->vf_0x68(ref->ptr);
    case 0x28230a7fdbf66651:
        return visitor->vf_0x70(ref->ptr);
    case 0x799781f713e7d4e0:
        return visitor->vf_0x78(ref->ptr);
    case (s64)0xbf34f79bba0deaf6:
        return visitor->vf_0x80(ref->ptr);
    case (s64)0x8c2deec131321516:
        return visitor->vf_0x88(ref->ptr);
    case 0x59924ac57dd96af4:
        return visitor->vf_0x90(ref->ptr);
    case 0x0d7b335721099bf9:
        return visitor->vf_0x98(ref->ptr);
    case 0x313df0831835525f:
        return visitor->vf_0xa0(ref->ptr);
    case 0x7c0e083af861f5a8:
        return visitor->vf_0xa8(ref->ptr);
    case 0x4048cbeec15d3c0e:
        return visitor->vf_0xb0(ref->ptr);
    case 0x3b98086a3ed2623d:
    case (s64)0xf6d656c06a983519:
        return visitor->vf_0xb8(ref->ptr);
    case (s64)0xb4f609812d5dc4bd:
        return visitor->vf_0xc0(ref->ptr);
    case 0x682df6c4d6c837fa: {
        UnkTypedRef dyn = DynamicRef(type, ref->ptr);
        return visitor->vf_0xc8(&dyn);
    }
    case 0x54887d7b68d1c872:
        if (!fn_71000e6db0(reinterpret_cast<UnkVersionEntry*>(const_cast<CType*>(type)))) {
            UnkTypedRef dyn = DynamicRef(ref->type, ref->ptr);
            return visitor->vf_0xe0(&dyn);
        }
        break;
    case (s64)0xea4a500b9e875db0: {
        UnkTypedRef dyn = DynamicRef(type, ref->ptr);
        return visitor->vf_0x110(&dyn);
    }
    case (s64)0xfc925429c459c462: {
        UnkTypedRef dyn = DynamicRef(type, ref->ptr);
        return visitor->vf_0x128(&dyn);
    }
    case (s64)0xc7058cebee667049: {
        UnkTypedRef dyn = DynamicRef(type, ref->ptr);
        return visitor->vf_0x130(&dyn);
    }
    case (s64)0xc45934b5455cb361:
        return VisitString<&PrimitiveType<base::global::TRntString32>::Get>(visitor, ref->ptr);
    case (s64)0xde99f196920e66b5:
        return VisitString<&PrimitiveType<base::global::TRntString64>::Get>(visitor, ref->ptr);
    case 0x2371dae4d1d885c4:
        return VisitString<&PrimitiveType<base::global::TRntString128>::Get>(visitor, ref->ptr);
    case 0x26de4835f38bb831:
        return VisitString<&PrimitiveType<base::global::TRntString256>::Get>(visitor, ref->ptr);
    case 0x21406426265e3dc2:
        return VisitString<&PrimitiveType<base::global::TRntString512>::Get>(visitor, ref->ptr);
    }
    return visitor->vf_0x138(ref);
}
